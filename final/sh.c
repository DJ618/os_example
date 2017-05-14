// ==========================================================
//                    sh Program:
// YOUR sh must support
// (1). logout/exit :  terminate
// (2). simple command exceution, e.g. ls, cat, ....
// (3). I/O redirection:  e.g. cat < infile;  cat > outfile, etc.
// (4). (MULTIPLE) PIPEs: e.g. cat file | l2u | grep LINE
// ===========================================================

#include "ucode.c"

int numtokens, i;
char *cmdtokens[16], tokens[16][32];

int tokenize(char *source, char *delimiter)
{
  char str_buff[64];
  char *token;
  int i=0, total = 0;

  if(!source || !delimiter)
  {
    printf("bad inputs to tokenize\n");
    return -1;
  }
  //strtok destrosy source
  strcpy(str_buff, source);
  //tokenize!
  token = strtok(str_buff, delimiter);
  while(token)
  {
    //copy and save pointer
    strcpy(&(tokens[i][0]), token);
    cmdtokens[i] = &(tokens[i][0]);
    total++; i++;
    //get next token
    token = strtok(0, delimiter);
  }
  //null term the next string
  tokens[i][0] = 0;
  return total;
}

int scan(char *line, char target)
{
  int pos = 0, length = 0;
  //start from endof string
  length = strlen(line);
  while(length > 0)
  {
    //search for target char
    if(line[length] == target)
    {
      return length;
    }
    length--;
  }
  return length;
}

int split(char *source, char *head, char *tail, int pos)
{
  char *sp = source;
  int headindex, tailindex, len, i = 0;

  len = strlen(source);
  tailindex = pos+1;
  headindex = pos-1;

  if(!source || !head || !tail)
  {
    return -1;
  }

  //consume whitespace around split pos
  while(source[tailindex] && source[tailindex] == ' '){ tailindex++; }
  while(source[headindex] && source[headindex] == ' '){ headindex--; }

  //copy tail to tail
  while(tailindex != len)
  {
    *tail = source[tailindex++];
    tail++;
  }
  *tail = 0;
  //copy head to head
  while(i <= headindex)
  {
    *head = source[i++];
    head++;
  }
  *head = 0;
}

int do_command(char *cmd)
{
  char head[24], tail[24];
  int redirectout, redirectin;

  redirectout = scan(cmd, '>');
  redirectin = scan(cmd, '<');

  //REDIRECT INPUT FROM FILE
  if(redirectin)
  {
    split(cmd, head, tail, redirectin);
    close(0);
    if(open(tail, O_RDONLY) < 0)
    {
      printf("No such input file: %s\n", tail);
    }
    exec(head);
  }
  //REDIRECT OUTPUT TO FILE
  if(redirectout)
  {
    split(cmd, head, tail, redirectout);
    //check for appending
    if(redirectout = scan(head, '>'))
    {
      //null the arrow
      head[redirectout] = 0;
      close(1);
      if(open(tail, O_WRONLY | O_APPEND) < 0)
      {
        printf("output file could not be opened: %s\n", tail);
      }
      exec(head);
    }
    //not appending create the file
    close(1);
    if(open(tail, O_WRONLY | O_CREAT) < 0)
    {
      printf("output file could not be opened: %s\n", tail);
    }
    exec(head);
  }
  //NO OUTPUT REDIRECTION, JUST EXEC
  exec(cmd);
}

int do_pipe( char *cmdline, int *pip)
{
  int haspipe = 0;
  char head[24], tail[24];
  int lpd[2], pid;
  if (pip)
  {
    close(pip[0]);
    dup2(pip[1], 1);
    close(pip[1]); //this is no good isnt it?
  }

  //CHECK FOR PIPE IN INPUT
  haspipe = scan(cmdline, '|');
  split(cmdline, head, tail, haspipe);

  if(haspipe)
  {
    if(pipe(lpd) < 0){ printf("could not open pipe!\n"); }

    pid = fork();
    //parent process
    if(pid)
    {
      //READER ON PIPE
      close(lpd[1]);
      dup2(lpd[0], 0);
      close(lpd[0]);
      do_command(tail);
    }
    else
    {
      do_pipe(head, lpd);
    }
  }
  else
  {
    do_command(cmdline);
  }
}

int main(int argc, char *argv[])
{
  char cmd_line[64], tty[16];
  int pid, status = 0;
  //NEVER DIE
  while(1){
    //PRINT LINE AND GET INPUT
    gettty(tty);
    memset(cmd_line, 0, 64);
    while(cmd_line[0] == 0)
    {
      printf("%s# ", tty);
      gets(cmd_line);
    }
    numtokens = tokenize(cmd_line, " ");
    //HANDLE BUILT IN COMMANDS
    if(!strcmp(tokens[0], "cd"))
    {
      chdir(tokens[1]);
    }
    if(!strcmp(tokens[0], "exit") || !strcmp(tokens[0], "logout"))
    {
      exit(1);
    }

    //SETUP CHILD PROC
    pid = fork();
    //parent process
    if(pid)
    {
      pid = wait(&status);
      printf("\nChild proccess %d, dies with status %d\n", pid, status);
      continue;
    }
    else
    {
      do_pipe(cmd_line, 0);
    }
  }
}

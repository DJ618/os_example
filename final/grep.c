#include "ucode.c"

char buf[1024];
char *bp;

int main(int argc, char *argv[])
{
  char ch, *pattern;
  int bufindex = 0, lineindex = 0, search = 0, patlen;
  int fd = 0, n = 1, i, nbytes;
  //bad input?
  if(argc > 3){ printf("USAGE: grep pattern [filename]\n"); return -1;}
  //do we have a file?
  if(argc == 3)
  {
    fd = open(argv[2], O_RDONLY);
    nbytes = 1024;
    if(fd < 0)
    {
      printf("error opening file: %s\n", argv[2]);
      return -1;
    }
  }
  else //read from stdin
  {
    fd = 0;
    nbytes = 1024;
  }
  // grab pattern gor later
  pattern = argv[1];
  patlen = strlen(pattern);

  while(n > 0)
  {
    //reset
    memset(buf, 0, 1024);
    bufindex = 0;
    lineindex = 0;
    //read from input
    n = read(fd, buf, nbytes);
    while (bufindex < n)
    {
      lineindex = 0;
      memset(line, 0, 64);
      //copy a line to the lin buffer
      while(buf[bufindex] != '\n' && buf[bufindex] != '\r' && i < n && lineindex < 64)
      {
        line[lineindex++] = buf[bufindex++];
      }
      //we need more DATA
      if(bufindex == n){continue;}
      //nullterm the line
      line[lineindex] = 0;

      if(line[0] == 0){ bufindex++;}
      //search every substring for match
      search = 0;
      while(search < lineindex)
      {
        //search line for pattern
        if(!strncmp(&(line[search++]), pattern, patlen))
        {
          //if matched print the line
          printf("%s\n", line);
          break;
        }
      }
    } // while over current buffer
  } // while we have read data
}

#include "ucode.c"

#define SCREENHEIGHT 24
#define SCREENWIDTH 80

char buf[1024];
char *bp;

int main(int argc, char *argv[])
{
  char ch, tty[16];
  int numprinted=0;
  int fd = 0, n = 1, i, nbytes, ttyfd;
  //bad input?
  if(argc > 2){ printf("USAGE: more [filename]\n"); return -1;}
  //do we have a file?
  if(argc == 2)
  {
    fd = open(argv[1], O_RDONLY);
    nbytes = 1024;
    if(fd < 0)
    {
      printf("error opening file: %s\n", argv[1]);
      return -1;
    }
  }
  else{
    fd = 0;
    nbytes = 1;
  }

  //Input from tty file just to be damed sure, pipes tend to screw that up
  gettty(tty);
  ttyfd = open(tty, O_RDONLY) ;

  while(n > 0)
  {
    //reset
    memset(buf, 0, 1024);
    i = 0;
    //read from input
    n = read(fd, buf, nbytes);
    //printf("read %d bytes\n", n);
    while (i < n)
    {
      while(buf[i] != '\n' && i < n)
      {
        printf("%c", buf[i++]);
      }//while we havent seen newline
      //we need more DATA
      if(n == i){continue;}
      //print the newline
      printf("%c", buf[i]);
      // print return char just in case
      if(buf[i] == '\n')
      {
        printf("\r");
      }
      //inc lines on screen
      numprinted++; i++;
      //if we've filled the screen wait for key
      if(numprinted >= SCREENHEIGHT)
      {
        read(ttyfd, &ch, 1);
        if(ch == '\r')
        {
          numprinted--;
        }
        else if(ch == ' ')
        {
          numprinted = 0;
        }
      }
    } // while over current buffer
  } // while we have read data
  close(ttyfd);
  close(fd);
}

#include <stdio.h>      /* Standard input/output definitions */
#include <stdlib.h>     /*Standard library definition*/
#include <sys/types.h> 
#include <sys/stat.h>  
#include <fcntl.h>      /* File control definitions */
#include <termios.h>    /* POSIX terminal control definitions */
#include <unistd.h>     /* UNIX standard function definitions */
#include <strings.h>    /* String function definitions */


int main(int argc, char **argv)
{

  if(argc < 2){

    printf("usage: [Device address]\n");
    return 0;
  }

  //Open the device
  int fd;
  fd = open(argv[1], O_RDWR|O_NOCTTY|O_NDELAY);
  
  if(fd == -1)
  {    
    printf("open error : Unable to open %s\n", argv[1]);
    return 0;
  }
  else
  {
    printf("\n Open serial OK\n");
    fcntl(fd, F_SETFL, 0);
  }
    
  //setting
  struct termios oldtio, newtio;
  tcgetattr(fd, &oldtio);
  bzero(&newtio, sizeof(newtio));

  newtio.c_cflag = B9600|CS8|CLOCAL|CREAD;
  newtio.c_iflag = IGNPAR;
  newtio.c_oflag = 0;    
  newtio.c_lflag |= ~(ICANON|ECHO|ECHOE);



  //newtio.c_lflag = ~(ICANON | ECHO | ECHOE | ISIG);;
  tcflush(fd, TCIFLUSH);             // flush terminal data
  tcsetattr(fd, TCSANOW, &newtio);

  

  while(1)
  {
    int res;
    char string[256];

    printf ("Type message: ");

    fgets (string, 255 , stdin);

    if (string[strlen(string) - 1] == '\n')
    {

      string[strlen(string) - 1] = '\0';

    }
        

    if(string[0] == '@')
    {
      res = write(fd, string, strlen(string));
      printf("res: %d   slen: %d \n", res, strlen(string));
      printf ("%s have been send.\n",string);
      break;
    }
    else
    {
      res = write(fd, string, strlen(string));
      printf("res: %d   slen: %d \n", res, strlen(string));
      printf ("%s have been send.\n",string);
    }

  }

  printf("Close...\n");
  close(fd);
  tcsetattr(fd, TCSANOW, &oldtio);

  return 0;
}

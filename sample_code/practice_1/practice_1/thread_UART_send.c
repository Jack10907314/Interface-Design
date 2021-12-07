#include <stdio.h>      /* Standard input/output definitions */
#include <stdlib.h>     /*Standard library definition*/
#include <sys/types.h> 
#include <sys/stat.h>  
#include <fcntl.h>      /* File control definitions */
#include <termios.h>    /* POSIX terminal control definitions */
#include <unistd.h>     /* UNIX standard function definitions */
#include <strings.h>    /* String function definitions */

void *send_process(void *ptr);

int fd, done = 0;

int main(int argc, char **argv)
{

  if(argc < 2){

    printf("usage: [Device address]\n");
    return 0;
  }

  //Open the device
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
    

  //device setting
  struct termios oldtio, newtio;
  tcgetattr(fd, &oldtio);						//dev/ttyUSB0
  bzero(&newtio, sizeof(newtio));

  newtio.c_cflag = B9600|CS8|CLOCAL|CREAD;
  newtio.c_iflag = IGNPAR;						// setting Input mode
  newtio.c_oflag = 0;    						// setting Output mode
  newtio.c_lflag |= ~(ICANON|ECHO|ECHOE);		// setting Canonical or Non Canonical mode

  tcflush(fd, TCIFLUSH);						// clearing any input data received, but not yet read
  tcsetattr(fd, TCSANOW, &newtio);				// setting immediately "fd" value 

  pthread_t thread_send;
  pthread_create(&thread_send,NULL,send_process,NULL);

  while(1) {

    if(done==1) break;

  }

  printf("Close...\n");
  close(fd);
  tcsetattr(fd, TCSANOW, &oldtio);

  return 0;
}


void *send_process(void *ptr)
{
    while(1){

        int res;
        
        char string[256]={0};					//char array

        printf ("Type message: ");

        fgets (string, 255 , stdin);			// get string content in terminal

        if (string[strlen(string) - 1] == '\n')	
        {

          string[strlen(string) - 1] = '\0';	//stop and send

        }
            
		
        if(string[0] == '@')
        {
          res = write(fd, string, strlen(string));		// sent  string with UART 
          printf("res: %d   slen: %d \n", res, strlen(string));
          printf ("%s have been send.\n",string);
          break;
        }
        else
        {
          res = write(fd, string, strlen(string));		// sent  string with UART 
          printf("res: %d   slen: %d \n", res, strlen(string));
          printf ("%s have been send.\n",string);
        }
    }

    done = 1;

}
















//==================================================================================//
// /* rs232_tx.c */
// #include <sys/types.h>
// #include <sys/stat.h>
// #include <fcntl.h>
// #include <termios.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <pthread.h>
// #define BAUDRATE        B9600
// #define MODEMDEVICE     "/dev/ttyS1"
// #define _POSIX_SOURCE   1

// #define STOP    '@'

// int fd,res,done=0;



// void *uart_send(void *ptr)
// {
// 	static char s1[20];
// 	char ch;

// 	while( (ch=getchar() ) != STOP) 
//         {
//             s1[0]=ch;
//             res=write(fd, s1, 1);
//         }
//         /* 將收到的 '@' 字元傳送出去 */
//         /*s1[0]=ch;
//         s1[1]='\n';
//         res = write(fd, s1, 2);*/
//         done=1;
// }


// int main(int argc, char *argv[])
// {
//     struct termios  oldtio, newtio;
//     pthread_t thread1;

//     printf("Start...\n");
//     fd = open(MODEMDEVICE, O_RDWR|O_NOCTTY);

//     if (fd < 0)
//     {
//         perror(MODEMDEVICE);
//         exit(1);
//     }

//     printf("Open...\n");
//     tcgetattr(fd, &oldtio);
//     bzero(&newtio, sizeof(newtio));
//     newtio.c_cflag = BAUDRATE|CS8|CLOCAL|CREAD;
//     newtio.c_iflag = IGNPAR;
//     newtio.c_oflag = 0;
//     /* 設定為正規模式 */
//     newtio.c_lflag = ICANON;
//     tcflush(fd, TCIFLUSH);
//     tcsetattr(fd, TCSANOW, &newtio);

//     printf("Writing ...\n");

//     res=write(fd,"Uart tx test\r\n", 15);

//     //建立執行緒
//     pthread_create(&thread1,NULL,uart_send,NULL);

//     while(1) 
//     {
//         sleep(10);
// 	printf("Send data\r\n");
// 	if(done==1)
// 		break;
//     }

//     printf("Close...\n");
//     close(fd);
//     tcsetattr(fd, TCSANOW, &oldtio);

//     return 0;
// }



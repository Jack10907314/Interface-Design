#include <stdio.h>  /* Standard input/output definitions */
#include <stdlib.h> /*Standard library definition*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>   /* File control definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <strings.h> /* String function definitions */

void *recv_process(void *ptr);
void *send_process(void *ptr);

int fd, done = 0;

char str[256] = "";

int main(int argc, char **argv)
{

  if (argc < 2)
  {

    printf("usage: [Device address]\n");
    return 0;
  }

  //Open the device
  fd = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY);

  if (fd == -1)
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
  tcgetattr(fd, &oldtio); //dev/ttyUSB0
  bzero(&newtio, sizeof(newtio));

  newtio.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
  newtio.c_iflag = IGNPAR;                    // setting Input mode
  newtio.c_oflag = 0;                         // setting Output mode
  newtio.c_lflag |= ~(ICANON | ECHO | ECHOE); // setting Canonical or Non Canonical mode

  tcflush(fd, TCIFLUSH);           // clearing any input data received, but not yet read
  tcsetattr(fd, TCSANOW, &newtio); // setting immediately "fd" value

  pthread_t thread_recv;
  pthread_create(&thread_recv, NULL, recv_process, NULL);

  pthread_t thread_send;
  pthread_create(&thread_send, NULL, send_process, NULL);

  printf("Reading... \n");
  while (1)
  {
  }

  int res = write(fd, "MyNameIsJack", strlen("MyNameIsJack"));
  printf("res: %d   slen: %d \n", res, strlen(str));
  printf("%s have been send.\n", str);

  printf("Close...\n");
  close(fd);
  tcsetattr(fd, TCSANOW, &oldtio);

  return 0;
}

void *recv_process(void *ptr)
{
  while (1)
  {

    int res;

    char string[256] = {0}; //recv array

    char HelloWhoAreYou[14] = {0};

    res = read(fd, string, 256); // get data from UART

	//當接收數量不等於0時 將接收到的字元string合併在str內
	//當接收數量等於0時 顯示str內容並清空
	//當str接收到 HelloWhoAreYou時 done=1
    if (res != 0)
    {
      strncat(str, &string, 256);

      if (str[0] == 'H' && str[1] == 'e' && str[2] == 'l' &&
          str[3] == 'l' && str[4] == 'o' && str[5] == 'W' &&
          str[6] == 'h' && str[7] == 'o' && str[8] == 'A' &&
          str[9] == 'r' && str[10] == 'e' && str[11] == 'Y' &&
          str[12] == 'o' && str[13] == 'u')
      {
        done = 1;
      }
    }
    else
    {
      if (strlen(str) != 0)
      {
        printf("recieve : %s \n", str);
      }
      memset(str, 0, strlen(str));
    }

    usleep(10000); //sleep 10ms
  }
}

// send function
void *send_process(void *ptr)
{
  while (1)
  {
    int res;

    char string[256] = {0}; //char array

	//當接收端接收到 HelloWhoAreYou 時 done會變成1，就會傳送 MyNameIsJack
    if (done == 1)
    {
      done = 0;
      res = write(fd, "MyNameIsJack", strlen("MyNameIsJack"));
      printf("res: %d   slen: %d \n", res, strlen("MyNameIsJack"));
      printf("%s have been send.\n", "MyNameIsJack");
      usleep(100000);
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
//         /* �N���쪺 '@' �r���ǰe�X�h */
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
//     /* �]�w�����W�Ҧ� */
//     newtio.c_lflag = ICANON;
//     tcflush(fd, TCIFLUSH);
//     tcsetattr(fd, TCSANOW, &newtio);

//     printf("Writing ...\n");

//     res=write(fd,"Uart tx test\r\n", 15);

//     //�إ߰����
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

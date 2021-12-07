/* rs232_rx.c */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *recv_process(void *ptr);
void *send_process(void *ptr);

int fd, done = 0;

char str[256] = "";

int main(int argc, char *argv[])
{
  if (argc < 2)
  {

    printf("usage: [Device address]\n");

    return 0;
  }

  //Open the device
  fd = open(argv[1], O_RDWR | O_NOCTTY | O_NDELAY);

  //If device can't open
  if (fd == -1)
  {
    printf("open error : Unable to open %s\n", argv[1]);
    return 0;
  }
  //If device can open
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

  while (1)
  {
  }

  printf("Close...\n");
  close(fd);
  tcsetattr(fd, TCSANOW, &oldtio);

  return 0;
}

// recvice function
void *recv_process(void *ptr)
{
  while (1)
  {
    int res;

    char string[256] = {0}; //recv array

    res = read(fd, string, 256); // get data from UART

	//當接收數量不等於0時 將接收到的字元string合併在str內
	//當接收數量等於0時 顯示str內容並清空
    if (res != 0)
    {
      strncat(str, &string, 256);
    }
    else
    {
      if (strlen(str) != 0)
      {
        printf("\nrecieve : %s \n", str);
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

    printf("Type message: ");

    fgets(string, 255, stdin); // get string content in terminal

    if (string[strlen(string) - 1] == '\n')
    {

      string[strlen(string) - 1] = '\0'; //stop and send
    }

    res = write(fd, string, strlen(string)); // sent  string with UART
  }
}

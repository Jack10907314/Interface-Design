
#include <sys/ioctl.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>


int main(int argc, char **argv){

   int ret, fd;

   printf("Starting device test code example...\n");

   fd = open("/dev/myUART", O_RDWR);             // Open the device with read/write access
   if (fd < 0){
      perror("Failed to open the device...");
      return errno;
   }


   unsigned char recv_buf = 0;
   unsigned char send_buf = 28;

   printf("Send: %d\n", send_buf);

   write(fd, &send_buf, NULL); 
   printf("Press ENTER to read back from the device...\n");
   getchar();


   read(fd, &send_buf, NULL); 
   printf("Press ENTER to read back from the device...\n");
   getchar();
   printf("Receive: %d\n", send_buf);

   return 0;
}
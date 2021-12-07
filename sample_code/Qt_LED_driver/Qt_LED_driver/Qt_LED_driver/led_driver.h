
#ifndef _LED_H_ 
#define _LED_H_ 

#include <asm/ioctl.h>

#define LED_MAJOR_NUM	0x13
#define LED_IOCTL_MAGIC	LED_MAJOR_NUM
#define MAX_MINORS	2
#define LED_SET_MAGIC 	_IO(LED_IOCTL_MAGIC, 0x00)
#define LED_CLEAR_MAGIC _IO(LED_IOCTL_MAGIC, 0x01)

#endif
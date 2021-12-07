
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>         // file_operations
#include <linux/cdev.h>       // Char Device Registration
#include <linux/gpio.h>       // gpio control
#include <linux/delay.h>      // mdelay
#include <linux/types.h>      // dev_t
#include <linux/interrupt.h>
#include <linux/uaccess.h>    // copy_from_user
#include <linux/hrtimer.h>
#include <linux/sched.h>

#include <linux/spinlock_types.h>




#define BCM2837_PERI_BASE 0x3F000000
#define GPIO_BASE (BCM2837_PERI_BASE + 0x200000) // for GPIO controller 

#define GPIO_TX 4
#define GPIO_RX 2

// Module Information
#define  MAJOR_NUM   0x13
#define MAX_MINORS   2
#define MODULE_NAME "myUART"
#define GPIO_DESC           "gpio interrupt"
#define GPIO_DEVICE_DESC    "gpio interrupt"


short int irq_any_gpio = 0;

int BAUDRATE = 4800;

DEFINE_SPINLOCK(_time_lock);
DEFINE_SPINLOCK(gpioirq_lock);

#define TX_BUFFER_SIZE	256
#define RX_BUFFER_SIZE	256

unsigned char DataBit[8] = {0,0,0,0,0,0,0,0};

static struct hrtimer hrtimer_tx;
static struct hrtimer hrtimer_rx;
unsigned char TX_DATA=0;
unsigned char RX_DATA=0;
unsigned char TX_BUFFER[8];
unsigned char RX_BUFFER[RX_BUFFER_SIZE];


//The prototype functions for the driver -- must come before the struct definition
static int uart_open(struct inode *, struct file *);
static ssize_t uart_write(struct file *, const char *, size_t, loff_t *);
static ssize_t uart_read(struct file *, char *, size_t , loff_t *);

static irqreturn_t rx_irq_handler(int , void *, struct pt_regs *);

struct file_operations uart_fops =
{
 .owner          = THIS_MODULE,
 .write          = uart_write,
 .read           = uart_read,
 .open           = uart_open,
};


static struct class *uart_class = NULL;
static struct device* uartDevice = NULL;

static struct cdev uart_cdev = {
  .kobj = {.name = MODULE_NAME, },
  .owner  = THIS_MODULE,
};

volatile uint32_t * bcm2837_gpio;
volatile uint32_t * bcm2837_gpio_set;
volatile uint32_t * bcm2837_gpio_clr;
volatile uint32_t * bcm2837_gpio_fsel;
volatile uint32_t * bcm2837_gpio_GPLEVn;
volatile uint32_t * bcm2837_gpio_GPRENn;
volatile uint32_t * bcm2837_gpio_GPFENn;


//--------------------------------------------------------------------------------------
int GPIOFunction(uint8_t gpio, uint8_t mode)
{
    bcm2837_gpio_fsel = bcm2837_gpio + (gpio/10);
    *bcm2837_gpio_fsel = ((*bcm2837_gpio_fsel) & ~(0b111 << ((gpio % 10) * 3))) | ((mode << ((gpio % 10) * 3)) & (0b111 << ((gpio % 10) * 3)));
    return 0;
}

//--------------------------------------------------------------------------------------
static void GPIOOutputValueSet(int gpio, bool value)
{
  bcm2837_gpio_set = bcm2837_gpio + 7 + gpio/32;
  bcm2837_gpio_clr = bcm2837_gpio + 10 + gpio/32;
  
  if (value)
  {
      uint8_t   shift = gpio % 32;
      uint32_t  v = 1 << shift;
      *bcm2837_gpio_set = *bcm2837_gpio_set | v;
  }
  else{
    uint8_t   shift = gpio % 32;
      uint32_t  v = 1 << shift;
      *bcm2837_gpio_clr = *bcm2837_gpio_clr | v;
  }
  
}

//--------------------------------------------------------------------------------------
static bool GPIOInputValueGet(int gpio)
{
  bcm2837_gpio_GPLEVn = bcm2837_gpio + 13 + gpio/32;

  if(gpio>31)
    return ((*bcm2837_gpio_GPLEVn) & (0x00000001 << (gpio-32) )) >> (gpio-32);
  else
    return ((*bcm2837_gpio_GPLEVn) & (0x00000001 << gpio)) >> gpio;
}

void GpioSetRisingEdgeDetect(uint8_t pin, uint8_t dir)
{
    bcm2837_gpio_GPRENn = bcm2837_gpio + 19 + pin/32;

    unsigned int reg_data, bit_mask;
    unsigned char reg_idx, bit_idx;

    bit_idx = pin % 32;
    bit_mask = 1 << bit_idx;
    reg_data = (dir << bit_idx);

    *bcm2837_gpio_GPRENn = ((*bcm2837_gpio_GPRENn) & (~bit_mask)) | reg_data;
}

void GpioSetFallingEdgeDetect(uint8_t pin, uint8_t dir)
{
    bcm2837_gpio_GPFENn = bcm2837_gpio + 22 + pin/32;

    unsigned int reg_data, bit_mask;
    unsigned char reg_idx, bit_idx;

    bit_idx = pin % 32;
    bit_mask = 1 << bit_idx;
    reg_data = (dir << bit_idx);

    *bcm2837_gpio_GPFENn = ((*bcm2837_gpio_GPFENn) & (~bit_mask)) | reg_data;
}



//--------------------------------------------------------------------------------------
static enum hrtimer_restart FunctionTimerTX(struct hrtimer * unused)
{
  unsigned long flags; 
  spin_lock_irqsave(&_time_lock, flags);   //Interrupt Mask 
  static int bit=-1;

  if(bit == -1){

    int b = TX_DATA;
    int i = 0;
    int c = 0;
    while(b!=0)
    {
      i=b%2;
      TX_BUFFER[c]=i;
      c++;
      b=b/2;
    }

    bit += 1;

    int gpio = GPIO_TX;
    bcm2837_gpio_set = bcm2837_gpio + 7 + gpio/32;
    bcm2837_gpio_clr = bcm2837_gpio + 10 + gpio/32;
    int value = 0;
    if(value)
    {
      uint8_t   shift = gpio % 32;
      uint32_t  v = 1 << shift;
      *bcm2837_gpio_set = *bcm2837_gpio_set | v;
    }
    else{
      uint8_t   shift = gpio % 32;
      uint32_t  v = 1 << shift;
      *bcm2837_gpio_clr = *bcm2837_gpio_clr | v;
    }
  }
  else if(bit>=0 && bit <=7)
  {

    int gpio = GPIO_TX;
    bcm2837_gpio_set = bcm2837_gpio + 7 + gpio/32;
    bcm2837_gpio_clr = bcm2837_gpio + 10 + gpio/32;
    int value = TX_BUFFER[bit];
    if(value)
    {
      uint8_t   shift = gpio % 32;
      uint32_t  v = 1 << shift;
      *bcm2837_gpio_set = *bcm2837_gpio_set | v;
    }
    else{
      uint8_t   shift = gpio % 32;
      uint32_t  v = 1 << shift;
      *bcm2837_gpio_clr = *bcm2837_gpio_clr | v;
    }

    printk(KERN_ALERT "%d \n", bit);
    bit++;
  }
  else if(bit==8)
  {

    int gpio = GPIO_TX;
    bcm2837_gpio_set = bcm2837_gpio + 7 + gpio/32;
    bcm2837_gpio_clr = bcm2837_gpio + 10 + gpio/32;
    int value = 1;
    if(value)
    {
      uint8_t   shift = gpio % 32;
      uint32_t  v = 1 << shift;
      *bcm2837_gpio_set = *bcm2837_gpio_set | v;
    }
    else{
      uint8_t   shift = gpio % 32;
      uint32_t  v = 1 << shift;
      *bcm2837_gpio_clr = *bcm2837_gpio_clr | v;
    }


    bit=-1;

    int i=0;
    for(i=0; i<8; i++)
      TX_BUFFER[i] = 0;

    spin_unlock_irqrestore(&_time_lock, flags);
    return HRTIMER_NORESTART;
  }

  spin_unlock_irqrestore(&_time_lock, flags);
  hrtimer_forward_now(&hrtimer_tx, ktime_set(0, (1000000/BAUDRATE)*1000 ));
	return HRTIMER_RESTART;
}

//--------------------------------------------------------------------------------------
static enum hrtimer_restart FunctionTimerRX(struct hrtimer * unused)
{

  unsigned long flags; 
  spin_lock_irqsave(&gpioirq_lock, flags);   //Interrupt Mask 


  static int bit=-1;

  if(bit == -1){

    int gpio = GPIO_RX;
    bcm2837_gpio_GPLEVn = bcm2837_gpio + 13 + gpio/32;
    int pin = gpio;
    if(pin >= 32){
      pin = pin - 32;
    }
    uint8_t shift = pin % 32;
    int value = (*(bcm2837_gpio_GPLEVn)&(1<<shift));
    if(value > 0)
      value = 1;

    printk(KERN_ALERT "%d: %d", bit, value);

    if(value == 0){
      bit += 1;
    }
    
  }
  else if(bit>=0 && bit <=7)
  {

    int gpio = GPIO_RX;
    bcm2837_gpio_GPLEVn = bcm2837_gpio + 13 + gpio/32;
    int pin = gpio;
    if(pin >= 32){
      pin = pin - 32;
    }
    uint8_t shift = pin % 32;
    int value = (*(bcm2837_gpio_GPLEVn)&(1<<shift));
    if(value > 0)
      value = 1;
    RX_BUFFER[bit] = value;

    printk(KERN_ALERT "%d: %d", bit, value);

    bit++;
  }
  else if(bit==8)
  {

    bit=-1;
    RX_DATA = 0;
    int i=0;
    for(i=0; i<8; i++){
      RX_DATA += RX_BUFFER[i] << i;
    }


    int pin = GPIO_RX;
    int dir = 1;
    bcm2837_gpio_GPFENn = bcm2837_gpio + 22 + pin/32;
    unsigned int reg_data, bit_mask;
    unsigned char reg_idx, bit_idx;
    bit_idx = pin % 32;
    bit_mask = 1 << bit_idx;
    reg_data = (dir << bit_idx);
    *bcm2837_gpio_GPFENn = ((*bcm2837_gpio_GPFENn) & (~bit_mask)) | reg_data;


    spin_unlock_irqrestore(&gpioirq_lock, flags);  //Interrupt Mask 
    return HRTIMER_NORESTART;
  }


  spin_unlock_irqrestore(&gpioirq_lock, flags);  //Interrupt Mask 

  hrtimer_forward_now(&hrtimer_rx, ktime_set(0, (1000000/BAUDRATE)*1000 ));

  return HRTIMER_RESTART;
}


//--------------------------------------------------------------------------------------
static irqreturn_t rx_irq_handler(int irq, void *dev_id, struct pt_regs *regs)
{
  hrtimer_init(&hrtimer_rx, CLOCK_REALTIME, HRTIMER_MODE_REL);
  hrtimer_rx.function = FunctionTimerRX;
  hrtimer_start(&hrtimer_rx,  ktime_set(0, ((1000000/BAUDRATE)*1000))/2, HRTIMER_MODE_REL);

  GpioSetFallingEdgeDetect(GPIO_RX, 0);

  return IRQ_HANDLED;
}


//--------------------------------------------------------------------------------------
static int __init  
init_module_uart_drv(void){

  struct  cdev   *pcdev;    
  dev_t   devno;    
  int      error;

  devno = MKDEV(MAJOR_NUM, 0);      
  if (register_chrdev_region(devno, MAX_MINORS, MODULE_NAME)){
    printk(KERN_ALERT "%s: can't get major %d\n", MODULE_NAME, MAJOR_NUM);          
    return (-EBUSY);
  }       
  pcdev = &uart_cdev;      
  cdev_init(pcdev, &uart_fops);
  pcdev->owner = THIS_MODULE;


  /* Register led_drv as character device */
  error = cdev_add(pcdev, devno, MAX_MINORS);
  if (error) {
    kobject_put(&pcdev->kobj);
    unregister_chrdev_region(devno, MAX_MINORS);
    printk(KERN_ERR "error register %s device\n", MODULE_NAME);    
    return (-EBUSY);
  }

  uart_class = class_create(THIS_MODULE, MODULE_NAME);
  if(IS_ERR(uart_class))
  {
    printk(KERN_ERR MODULE_NAME " class_create failed\n");
    PTR_ERR(uart_class);
    cdev_del(&uart_cdev);
  }
  printk(KERN_INFO MODULE_NAME " Create device \n");



  bcm2837_gpio = (volatile uint32_t *)ioremap(GPIO_BASE, 16);
  GPIOFunction(GPIO_TX, 0b001); //GPIO as output
  GPIOFunction(GPIO_RX, 0b000); //GPIO as input

  GpioSetFallingEdgeDetect(GPIO_RX, 1);

  if ( (irq_any_gpio = gpio_to_irq(GPIO_RX)) < 0 ) {
    printk("GPIO to IRQ mapping faiure %s\n", GPIO_DESC);
    return;
  }

  if(request_irq(irq_any_gpio,
                (irq_handler_t ) rx_irq_handler,
                IRQF_TRIGGER_FALLING,
                GPIO_DESC,
                GPIO_DEVICE_DESC)) {
    printk("Irq Request failure\n");
    return;
  }


  uartDevice = device_create(uart_class, NULL, MKDEV(MAJOR_NUM, 0), NULL, MODULE_NAME);
  if (IS_ERR(uartDevice)){               // Clean up if there is an error
    class_destroy(uart_class);           // Repeated code but the alternative is goto statements
    unregister_chrdev(MAJOR_NUM, MODULE_NAME);
    printk(KERN_ALERT "Failed to create the device\n");
    return PTR_ERR(uartDevice);
  }
  printk(KERN_INFO "device class created correctly\n"); // Made it! device was initialized


  spin_lock_init(&_time_lock);
  spin_lock_init(&gpioirq_lock);

  return (0);
}

//--------------------------------------------------------------------------------------
static void __exit  
cleanup_module_uart_drv(void){

  free_irq(irq_any_gpio, GPIO_DEVICE_DESC);
  device_destroy(uart_class, MKDEV(MAJOR_NUM, 0));     // remove the device
  class_unregister(uart_class);                          // unregister the device class
  class_destroy(uart_class);                             // remove the device class

  cdev_del(&uart_cdev);
  unregister_chrdev_region(MKDEV(MAJOR_NUM, 0), MAX_MINORS);
  printk(KERN_ALERT "%s: removed\n", MODULE_NAME);  
}


//--------------------------------------------------------------------------------------
static int 
uart_open (struct inode *inode, struct file *filep)
{
  printk("Open UART");
  return (0);
}

//--------------------------------------------------------------------------------------
static ssize_t 
uart_write(struct file *filep, const char *buffer, size_t len, loff_t *ppos)
{

  
  int co;
  if((co=copy_from_user(&TX_DATA ,buffer , sizeof(TX_DATA)))){
      return -EFAULT;
  }
  else{
    printk("%d\n", TX_DATA);
    printk("copy from user is ok\n");
  }

  hrtimer_init(&hrtimer_tx, CLOCK_REALTIME, HRTIMER_MODE_REL);
  hrtimer_tx.function = FunctionTimerTX;
  hrtimer_start(&hrtimer_tx,  ktime_set(0, 0), HRTIMER_MODE_REL);


	return 0;
}

//--------------------------------------------------------------------------------------
static ssize_t 
uart_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
  int co;
  if((co=copy_to_user(buffer ,&RX_DATA , sizeof(RX_DATA)))){
    return -EFAULT;
  }
  else{
    printk("%d\n", RX_DATA);
    printk("copy to user is ok\n");
  } 
	return 0;
}


module_init(init_module_uart_drv);
module_exit(cleanup_module_uart_drv);
MODULE_LICENSE("GPL");
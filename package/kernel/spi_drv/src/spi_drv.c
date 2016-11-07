#include <linux/mm.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/mman.h>
#include <linux/random.h>
#include <linux/init.h>
#include <linux/raw.h>
#include <linux/tty.h>
#include <linux/capability.h>
#include <linux/ptrace.h>
#include <linux/device.h>
#include <linux/highmem.h>
#include <linux/crash_dump.h>
#include <linux/backing-dev.h>
#include <linux/bootmem.h>
#include <linux/splice.h>
#include <linux/pfn.h>
#include <linux/export.h>
#include <linux/io.h>
#include <linux/aio.h>
#include <linux/kernel.h>
#include <linux/module.h>

#include <asm/uaccess.h>
#define _DBUG_
#undef _DBUG_

#define DEVICE_NAME     "spidev"  //����ģʽ��ִ�С�cat /proc/devices����������豸���� 
#define SPI_MAJOR       0       //���豸��
//#define delays	300 
#define delays 100
#define delay 100
#define LA_GAO 1
#define LA_DI 0
#define SIM_HIGHT 2
#define SIM_LOW 3
#define SIM_ENABLE 4
#define SIM_DISABLE 5
volatile unsigned long *GPIO_CTRL_1;
volatile unsigned long *GPIO_CTRL_0;  //����������
volatile unsigned long *GPIO_POL_0;		//�����Ƿ�ת
volatile unsigned long *GPIO_DATA_0;  // ʲô����
volatile unsigned long *GPIO_DATA_1;
volatile unsigned long *GPIOMODE;
volatile unsigned long *GPIO2MODE;
volatile unsigned long *AGPIO_CFG;

#ifndef SPI_CTL
#define SPI_CTL
#endif

#ifdef SPI_CTL
volatile unsigned long *SPI_MASTER;
#define SPI_MODE_CONFIG 6
#define SPI_MODE_CONFIG_READ 7
#endif


#define _BUF_
#undef _BUF_

#ifdef _BUF_
char *buf=NULL;
#define COUNT 20
#endif

static struct class *spi_drv_class;
/*
	cs: gpio14
	clk: gpio15
	miso: gpio16
	mosi: gpio17
*/

static int spi_drv_open(struct inode *inode, struct file *file)
{
	printk("%s:Hello spi\n", __FUNCTION__);	// printk������������Ӵ�ӡ���÷���Ӧ�ó����е�printfһ��
	return 0;
}

static char readByte(void)
{
	char ch = 0;
	int i = 7;
	char mask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
#if 1
	*GPIO_DATA_0 |= (1<<14);// ����Ƭѡ
	*GPIO_DATA_0 |= (1<<15);// clk ����
	*GPIO_DATA_0 |=(1<<17);// �������
	//*GPIO_DATA_0&=~(1<<15);// clk ����
	ndelay(delays);
#endif
	
	*GPIO_DATA_0&=~(1<<14);// ����Ƭѡ
	ndelay(delays);// tsu1
#if 1
	for(i=7;i>=0;i--)
	{
		*GPIO_DATA_0 &=~(1<<15);// clk ����
		ndelay(delays);
		if((*GPIO_DATA_1>>13)&0x1)// ����16λ������ MSB
			ch|=mask[i]; // ��1
		else
			ch&=~mask[i];	// ��0	
		
		*GPIO_DATA_0|=(1<<15);// clk ����
		ndelay(delays);// th2
	}
#else
	for(i=7;i>=0;i--)
	{
		*GPIO_DATA_0|=(1<<15);// clk ����
		if((*GPIO_DATA_1>>13)&0x1)// ����16λ������ MSB
			ch|=mask[i]; // ��1
		else
			ch&=~mask[i];	// ��0	
		ndelay(delays);// th2
		*GPIO_DATA_0 &=~(1<<15);// clk ����
		ndelay(delays);				
	}
#endif
	*GPIO_DATA_0|=(1<<14);// ����ȡ��Ƭѡ
	*GPIO_DATA_0 |=(1<<17);// �������
	ndelay(delays); // tcs
	return ch;
}


static ssize_t spi_drv_read(struct file *file, char __user *buff, size_t size, loff_t *ppos)
{
#ifndef _BUF_
	char *buf = NULL;
	buf = (char *)kmalloc(size + 1, GFP_KERNEL);
	if (!buf)
		return 0;
#else
	memset(buf,0x0,COUNT);
#endif
    int i=0;
    char j=0;  
   
    for(i=0;i<size;i++)
    {
		buf[i] = readByte();
    }

    j = copy_to_user((char *)buff,buf,size);
    if(j)
		printk("copy error\n");
#ifndef _BUF_
	kfree(buf);
#endif
    return size;
}

static void writeByte(const char c)
{
	int i = 7;
	char mask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
#if 1
	*GPIO_DATA_0 |= (1<<14);// ����Ƭѡ
	*GPIO_DATA_0 |= (1<<15);// clk ����
	*GPIO_DATA_0 |=(1<<17);// �������
	//*GPIO_DATA_0&=~(1<<15);// clk ����
	ndelay(delays);
#endif
	
	*GPIO_DATA_0&=~(1<<14);// ����Ƭѡ
	ndelay(delays);
#if 1
	for(i=7;i>=0;i--)
	{
		*GPIO_DATA_0&=~(1<<15);// clk ����
		if(c&mask[i])
		{
			*GPIO_DATA_0|=(1<<17);// ������д��1
		}
		else
		{
			*GPIO_DATA_0&=~(1<<17);// ������д��0
		}	
		ndelay(delays);//��ʱdelaysns
		*GPIO_DATA_0|=(1<<15);// clk ����
		ndelay(delays);//��ʱdelaysns	
	}
#else
	for(i=7;i>=0;i--)
	{
		*GPIO_DATA_0|=(1<<15);// clk ����
		if(c&mask[i])
		{
			*GPIO_DATA_0|=(1<<17);// ������д��1
		}
		else
		{
			*GPIO_DATA_0&=~(1<<17);// ������д��0
		}	
		ndelay(delays);//��ʱdelaysns	
		*GPIO_DATA_0&=~(1<<15);// clk ����		
		ndelay(delays);//��ʱdelaysns		
	}
#endif
	*GPIO_DATA_0|=(1<<14);// ����ȡ��Ƭѡ
	*GPIO_DATA_0 |=(1<<17);// �������
	ndelay(delays);
	return ;
}

static ssize_t spi_drv_write(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos)
{
//	printk("%s:Hello spi\n", __FUNCTION__);	// printk������������Ӵ�ӡ���÷���Ӧ�ó����е�printfһ��
    int i=0;
#ifndef _BUF_
	char *buf = NULL;
	buf=(char *)kmalloc(count + 1,GFP_KERNEL);
	if (!buf)
		return 0;
#else
	memset(buf,0x0,COUNT);		
#endif
    if(copy_from_user(buf,(char *)ubuf,count))
    {
        printk("no enough memory!\n");
        return -1;
    }
    for(i=0;i<count;i++)
    {
#ifdef _DBUG_
		printk("spi write 0x%02X!\n",*buf);
#endif
        writeByte(buf[i]);
        //buf++;
    }
#ifndef _BUF_
	kfree(buf);
#endif
    return count;
}

static long gpio_rst__drv_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	switch(cmd)
	{
		case LA_GAO:
			*GPIO_DATA_0 |=(1<<5);
			printk("%s: LA GAO\n", __FUNCTION__);
			break;
		case LA_DI:
			*GPIO_DATA_0 &=~(1<<5);
			printk("%s:LA DI\n", __FUNCTION__);
			break;
		case SIM_HIGHT:
			*GPIO_DATA_0 |=(1<<4);
			printk("%s:SIM_HIGHT\n", __FUNCTION__);
			break;
		case SIM_LOW:
			*GPIO_DATA_0 &=~(1<<4);
			printk("%s:SIM_LOW\n", __FUNCTION__);
			break;
		case SIM_ENABLE:
			//*GPIO_DATA_1 |= (1<<14);
			*GPIO_DATA_1 |= (1<<8);
			printk("%s:SIM_ENABLE\n", __FUNCTION__);
			break;
		case SIM_DISABLE:
			//*GPIO_DATA_1 &= ~(1<<14);
			*GPIO_DATA_1 &= ~(1<<8);
			printk("%s:SIM_DISABLE\n", __FUNCTION__);
			break;
		case SPI_MODE_CONFIG:
			*SPI_MASTER |= (3<<4);
			printk("%s:SPI_MODE_CONFIG: 0x%x \n", __FUNCTION__, *SPI_MASTER);
			break;
		case SPI_MODE_CONFIG_READ:
			printk("%s:SPI_MODE_CONFIG: 0x%x \n", __FUNCTION__, *SPI_MASTER);
			break;
		default:
			break;
	}
	return 0;
}

static struct file_operations spi_drv_fops = {
	.owner  	= THIS_MODULE,    /* ����һ���꣬�������ģ��ʱ�Զ�������__this_module���� */
	.open   	= spi_drv_open,     
	.read		= spi_drv_read,	   
	.write		= spi_drv_write,	   
	.unlocked_ioctl = gpio_rst__drv_ioctl,

};

int major;

static int __init spi_drv_init(void)
{
	major = register_chrdev(SPI_MAJOR, DEVICE_NAME, &spi_drv_fops);
	if (major < 0)
	{
		printk(DEVICE_NAME " can't register major number\n");
		return major;
	}

	spi_drv_class = class_create(THIS_MODULE, "spi");
	device_create(spi_drv_class, NULL, MKDEV(major, 0), NULL, "spi");		// /dev/spi

	printk("%s:Hello spi\n", __FUNCTION__);	// printk������������Ӵ�ӡ���÷���Ӧ�ó����е�printfһ��

	AGPIO_CFG=(volatile unsigned long *)ioremap(0x1000003c,4);
	GPIOMODE=(volatile unsigned long *)ioremap(0x10000060,4);
	GPIO2MODE=(volatile unsigned long *)ioremap(0x10000064,4);
	GPIO_CTRL_0=(volatile unsigned long *)ioremap(0x10000600,4);
	GPIO_CTRL_1=(volatile unsigned long *)ioremap(0x10000604,4);
	GPIO_DATA_0=(volatile unsigned long *)ioremap(0x10000620,4);
	GPIO_DATA_1=(volatile unsigned long *)ioremap(0x10000624,4);
#ifdef SPI_CTL
	SPI_MASTER = (volatile unsigned long *)ioremap(0x10000b28, 4);
#endif

	*AGPIO_CFG|=(1<<20)|(1<<19)|(1<<18)|(1<<17);//ȫ������Ϊ1
	*GPIOMODE&=~(0x3<<2);
	*GPIO2MODE &=~(0x3<<8);
	*GPIO2MODE |= (1<<8);
	//*GPIOMODE&=~(0x3<<24);
	*GPIOMODE|=(1<<2); // ����GPIO ģʽ���� 	|(1<<24);
	
	*GPIOMODE&=~(0x3<<20);// set gpio 5 rest
	*GPIOMODE|=(1<<20);// 

	*GPIO_CTRL_0|=(1<<14)|(1<<15)|(1<<17);// GPIO 14 15 17 Ϊ���
	//*GPIO_CTRL_0&=~(1<<16);// GPIO 16 Ϊ����
	*GPIO_CTRL_1&=~(1<<13);	// 45 spi ����
	//*GPIO_CTRL_1|=(1<<14);		// 46 ����Ϊ��� simģ���ϵ�����
	*GPIO_CTRL_1 |= (1<<8);// set output 
	*GPIO_DATA_0 |=(1<<15);
	//*GPIO_DATA_0|=(1<<14);
	*GPIO_CTRL_0|=(1<<5)|(1<<4);// set gpio 5 gpio 4 output 
#ifdef _BUF_
	buf=(char *)kmalloc(COUNT,GFP_KERNEL);
#endif _BUF_
	//*GPIO_DATA_1 |= (1<<14);
	*GPIO_DATA_1 |= (1<<8); 
	printk("%s:SIM_ENABLE\n", __FUNCTION__);
	return 0;
}

static void __exit spi_drv_exit(void)
{
	unregister_chrdev(major, "spi");		// ����ں�����register_chrdev�������ʹ��
	device_destroy(spi_drv_class, MKDEV(major, 0));	// ����ں�����device_create�������ʹ��
	class_destroy(spi_drv_class);	// ����ں�����class_create�������ʹ��
	iounmap(GPIOMODE);
	iounmap(AGPIO_CFG);
	iounmap(GPIO_CTRL_0);
	iounmap(GPIO_CTRL_1);
	iounmap(GPIO_DATA_1);
	iounmap(GPIO_DATA_0);
	printk("%s:Hello spi\n", __FUNCTION__);	// printk������������Ӵ�ӡ���÷���Ӧ�ó����е�printfһ��
#ifdef _BUF_
	kfree(buf);
#endif
}

module_init(spi_drv_init);
module_exit(spi_drv_exit);

MODULE_AUTHOR("http://www.spi.com");
MODULE_VERSION("0.1.0");
MODULE_DESCRIPTION("RT5350 FIRST Driver");
MODULE_LICENSE("GPL");


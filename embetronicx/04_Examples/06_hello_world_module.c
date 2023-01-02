/********Dynamic allocation of Major & Minor  Number & automatic creation of device file in /dev folder***********************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>   //THIS_MODULE
#include<linux/fs.h>       //Added for file_operation structure
#include<linux/types.h>
#include<linux/device.h>    //Added for automatically creating device file
#include<linux/kdev_t.h>    //Added for automatically creating device file
#include <linux/cdev.h>    //Added for cdev_init(),cdev_add() and cdev_del();
/**************1st. dev_t structure variable*************************************/
dev_t dev = 0;
static struct class *dev_class;
static struct device *device;
static struct cdev c_dev;  //Used to bind cdev structure to file_operation structure
int etx_open(struct inode *in, struct file *fp);
ssize_t etx_read(struct file *fp, char __user *buff, size_t size, loff_t *off);
ssize_t etx_write(struct file *fp, const char __user *buff, size_t size, loff_t *off);
int etx_release(struct inode *in, struct file *fp);
static struct file_operations fops =
{
	.owner          = THIS_MODULE,
	.read           = etx_read,
	.write          = etx_write,
	.open           = etx_open,
	.release        = etx_release,
};

int etx_open(struct inode *in, struct file *fp)
{
	printk(KERN_INFO"Opening /dev/etx_device....\n");
	return 0;
}

ssize_t etx_read(struct file *fp, char __user *buff, size_t size, loff_t *off)
{
	printk(KERN_INFO"Reading from /dev/etx_device......\n");
	return 0;
}

ssize_t etx_write(struct file *fp, const char __user *buff, size_t size, loff_t *off)
{
	printk(KERN_INFO"Writting to /dev/etx_device......\n");
	return size;
}

int etx_release(struct inode *in, struct file *fp)
{
	printk(KERN_INFO"Closing /dev/etx_device....\n");
	return 0;
}
/**************2nd. Dynamically allocating the major and minor number*************/
static int __init hello_world_init(void)
{
    /*Allocating Major number*/
    if((alloc_chrdev_region(&dev, 0, 1, "Embetronicx_Dev")) <0)//Dynamically create Major and Minor Number(check in proc/devices)
    {
        printk(KERN_INFO "Cannot allocate major number for device 1\n");
        return -1;
    }
    /*Creating cdev structure*/
    cdev_init(&c_dev,&fops);

    /*Adding character device to the system*/
    if(cdev_add(&c_dev,dev,1) < 0)
    {
	printk(KERN_INFO"Can not add dvice to the system\n");
	goto r_class;
    }
    /*Device class creation*/
    if((dev_class = class_create(THIS_MODULE,"ETX_Device")) == NULL)//sys/class deice class creation
    {
        printk(KERN_INFO"Device class creation Unsuccessful\n");
        goto r_class;
    }
    /*Creating Device*/
    if((device = device_create(dev_class,NULL,dev,NULL,"etx_device")) == NULL)//create device file in /dev folder
    {
        printk(KERN_INFO"Device creation in /dev/ is unsuccessful\n");
        goto r_device;
    }
    printk(KERN_INFO"Major Number:%d & Minor Number:%d\n",MAJOR(dev),MINOR(dev));
    printk(KERN_INFO"Kernel module inserted successfully.\n");
    return 0;
r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev,1);//Unregister the major and minor number of the device(proc/devices)
    return -1;
}
/*************3rd. Deallocate the device while module is removed****************/
void __exit hello_world_exit(void)
{
    device_destroy(dev_class,dev);//Destroy Device file from /dev folder
    class_destroy(dev_class);     //Destroy the device class from /sys/class folder
    cdev_del(&c_dev);
    unregister_chrdev_region(dev,1);//Unregister the major and minor number of the device(proc/devices)
    printk(KERN_INFO"Kernel module removed successfully and the device is deallocated.\n");
}
module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Linux device driver (File Operations)");
MODULE_AUTHOR("Sunil Kumar Sahu <mr.sahu39@gmail.com>");
MODULE_VERSION("2:1.5");

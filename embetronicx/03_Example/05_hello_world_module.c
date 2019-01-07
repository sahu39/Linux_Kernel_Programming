/********Dynamic allocation of Major & Minor  Number & automatic creation of device file in /dev folder***********************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/types.h>
#include<linux/device.h>    //Added for automatically creating device file
#include<linux/kdev_t.h>    //Added for automatically creating device file

/**************1st. dev_t structure variable*************************************/
dev_t dev = 0;
static struct class *dev_class;
static struct device *device;

/**************2nd. Statically allocating the major and minor number*************/
static int __init hello_world_init(void)
{
    /*Allocating Major number*/
    if((alloc_chrdev_region(&dev, 0, 1, "Embetronicx_Dev")) <0)//Dynamically create Major and Minor Number(check in proc/devices)
    {
        printk(KERN_INFO "Cannot allocate major number for device 1\n");
        return -1;
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
    printk(KERN_INFO"Kernel Module inserted successfully\n");
    return 0;
r_device:
    class_destroy(dev_class);
r_class:
    unregister_chrdev_region(dev,1);//Unregister the major and minor number of the device(proc/devices)

    printk(KERN_INFO"Major Number:%d & Minor Number:%d\n",MAJOR(dev),MINOR(dev));
    printk(KERN_INFO"Kernel module inserted successfully.\n");
    return 0;
}
/*************3rd. Deallocate the device while module is removed****************/
void __exit hello_world_exit(void)
{
    device_destroy(dev_class,dev);//Destroy Device file from /dev folder
    class_destroy(dev_class);     //Destroy the device class from /sys/class folder
    unregister_chrdev_region(dev,1);//Unregister the major and minor number of the device(proc/devices)
    printk(KERN_INFO"Kernel module removed successfully and the device is deallocated.\n");
}
module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello world character driver with major and minor number creation dynamically.");
MODULE_AUTHOR("Sunil Kumar Sahu <mr.sahu39@gmail.com>");
MODULE_VERSION("2:1.5");

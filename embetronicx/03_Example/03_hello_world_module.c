/********Static allocation of Major & Minor  Number***********************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/types.h>

/**************1st. dev_t structure variable*************************************/
dev_t dev = MKDEV(235,0);

/**************2nd. Statically allocating the major and minor number*************/
static int __init hello_world_init(void)
{
    register_chrdev_region(dev, 1, "Embetronicx_Device");//allocating the user device in /proc/devices entry with major and minor number
    printk(KERN_INFO"Major Number:%d & Minor Number:%d\n",MAJOR(dev),MINOR(dev));
    printk(KERN_INFO"Kernel module inserted successfully.\n");
    return 0;
}
/*************3rd. Deallocate the device while module is removed****************/
void __exit hello_world_exit(void)
{
    unregister_chrdev_region(dev,1);
    printk(KERN_INFO"Kernel module removed successfully and the device is deallocated.\n");
}
module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello world character driver with major and minor number creation statically.");
MODULE_AUTHOR("Sunil Kumar Sahu <mr.sahu39@gmail.com>");
MODULE_VERSION("2:1.2");

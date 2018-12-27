/***********************Dynamic allocation of major and minor number**************************/
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/fs.h>
#include<linux/types.h>

dev_t dev = 0;
static int __init hello_world_init(void)
{
        /*****************Allocating major number*************************/
        if(alloc_chrdev_region(&dev, 0,1, "Embetronicx_Device")<0)
                {
                            printk(KERN_INFO"Can not allocate Device Number for the Device\n");
                                    return -1;
                                        }
            printk(KERN_INFO"Major Number:%d & Minor Number:%d\n",MAJOR(dev),MINOR(dev));
                printk(KERN_INFO"Module Inserted successfully\n");
                    return 0;
}

void __exit hello_world_exit(void)
{
        unregister_chrdev_region(dev,1);
            printk("Module Removed successfully & device deallocated\n");
}
module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_AUTHOR("Sunil Kumar Sahu <mr.sahu39@gmail.com>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hello world driver with dynamically generating major and minor number\n");
MODULE_VERSION("2:1.3");

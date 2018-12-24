#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>

static int __init hello_world_init(void)
{
    printk(KERN_INFO "Welcome to Embetronics Kernel programming\n");
    printk(KERN_INFO "This is a simple Module\n");
    printk(KERN_INFO "Kernel Module Inderted Successfully...\n");
    return 0;
}

void __exit hello_world_exit(void)
{
    printk(KERN_INFO "KERNEl module removed Successfully....\n");
}
module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sunil kumar Sahu <mr.sahu39@gmail.com>");
MODULE_DESCRIPTION("Simple Hello world Driver");
MODULE_VERSION("2:1.0");

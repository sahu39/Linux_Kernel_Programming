#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>

#define SET_VALUE _IOW('q', 1, char *)

static dev_t dev;
static struct cdev c_dev;
static struct class *cl;
char value[60]={"\0"};

static int my_open(struct inode *i, struct file *f)
{
        printk(KERN_INFO"Driver opened...!!!\n");
        return 0;
}
static int my_close(struct inode *i, struct file *f)
{
        printk(KERN_INFO"Driver closed...!!!\n");
        return 0;
}
static long my_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    switch(cmd)
    {
        case SET_VALUE:
            copy_from_user(&value,(char *)arg,sizeof(value));
            printk(KERN_INFO"ioctl request processing.......\n");
            printk(KERN_INFO"%s\n",value);
            break;
        default:
            printk(KERN_INFO"Invalid Ioctl request\n");
            return -EINVAL;
    }
    return 0;
}

static struct file_operations my_fops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .release = my_close,
    .unlocked_ioctl = my_ioctl,
};

static int __init query_ioctl_init(void)
{
    int ret;
    struct device *dev_ret;
             
             
    if ((ret = alloc_chrdev_region(&dev, 0, 1, "query_ioctl")) < 0)
    {
        return ret;
    }

    cdev_init(&c_dev, &my_fops);

    if ((ret = cdev_add(&c_dev, dev, 1)) < 0)
    {
        return ret;
    }

    if((cl = class_create(THIS_MODULE, "char")) == NULL)
    {
        printk(KERN_INFO "Cannot create the struct class\n");
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, 1);
        return PTR_ERR(cl);
    }

    if (IS_ERR(dev_ret = device_create(cl, NULL, dev, NULL, "query")))
    {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev, 1);
        return PTR_ERR(dev_ret);
    }

    return 0;

}


static void __exit query_ioctl_exit(void)
{
    device_destroy(cl, dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev, 1);
}


module_init(query_ioctl_init);
module_exit(query_ioctl_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sunil Kumar Sahu");
MODULE_DESCRIPTION("Query ioctl() Char Driver");


#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>


static int my_misc_open(struct inode *inode,struct file *file){
	pr_info("%s:%d\n",__func__,__LINE__);
	return 0;
}


static int my_misc_close(struct inode *inode,struct file *file){
	pr_info("%s:%d\n",__func__,__LINE__);
	return 0;
}

static ssize_t my_misc_write(struct file *file, const char __user *buf,
               size_t len, loff_t *ppos)
{
	pr_info("%s:%d\n",__func__,__LINE__);
	return len; 
}

static ssize_t my_misc_read(struct file *filp, char __user *buf,
                    size_t count, loff_t *f_pos)
{
	pr_info("%s:%d\n",__func__,__LINE__);
	return 0;
}

static const struct file_operations fops = {
    .owner          = THIS_MODULE,
    .write          = my_misc_write,
    .read           = my_misc_read,
    .open           = my_misc_open,
    .release        = my_misc_close,
    .llseek         = no_llseek,
};

/*
struct miscdevice {
  int minor;
  const char *name;
  struct file_operations *fops;
  struct miscdevice *next, *prev;
};

*/

//Misc device structure
struct miscdevice my_misc_device = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "simple_etx_misc",
    .fops = &fops,
};


static int __init misc_init(void)
{
    int error;
//register misc device 
    error = misc_register(&my_misc_device);
    if (error) {
        pr_err("misc_register failed!!!\n");
        return error;
    }
 
    pr_info("misc_register init done!!!\n");
    return 0;
}


static void __exit misc_exit(void)
{
//unregister misc device
    misc_deregister(&my_misc_device);
    pr_info("misc_register exit done1!!!\n");
}


module_init(misc_init)
module_exit(misc_exit)

MODULE_LICENSE("GPL V2");
MODULE_AUTHOR("Sunil Kumar Sahu <mr.sahu39@gmail.com>");
MODULE_DESCRIPTION("A simple device driver - Misc Driver");
MODULE_VERSION("1.29");

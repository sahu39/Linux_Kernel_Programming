#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <asm/uaccess.h>
#include <linux/ioctl.h>
#include <linux/kthread.h>             //kernel threads
#include <linux/sched.h>               //task_struct 
#include <linux/delay.h>
#include <linux/mutex.h>

struct mutex my_mutex;
unsigned long my_global_variable = 0;

static dev_t dev;
static struct cdev c_dev;
static struct class *cl;

static struct task_struct *my_thread1;
static struct task_struct *my_thread2;

int thread_function1(void *pv)
{
    while(!kthread_should_stop())
    {
        mutex_lock(&my_mutex);
        my_global_variable++;
        printk(KERN_INFO"my_global_variable = %lu......incremented 1 by thread_1\n",my_global_variable);
        mutex_unlock(&my_mutex);
        msleep(1000);
    }
    return 0;
}
int thread_function2(void *pv)
{
    while(!kthread_should_stop())
    {
        mutex_lock(&my_mutex);
        my_global_variable = my_global_variable+2;
        printk(KERN_INFO"my_global_variable = %lu......incremented 2 by thread_2\n",my_global_variable);
        mutex_unlock(&my_mutex);
        msleep(1000);
    }
    return 0;
}

static int my_open(struct inode *i, struct file *f)
{
    printk(KERN_INFO"Driver opened...!!!\n");
    return 0;
}
static ssize_t my_read(struct file *f,char __user *buf,size_t len,loff_t *off)
{
    printk(KERN_INFO"Driver opened for read...!!!\n");
    return 0;
}
static ssize_t my_write(struct file *f,const char __user *buf,size_t len,loff_t *off)
{
    printk(KERN_INFO"Driver opened for write...!!!\n");
    return 0;
}
static int my_close(struct inode *i, struct file *f)
{
    printk(KERN_INFO"Driver closed...!!!\n");
    return 0;
}


static struct file_operations my_fops =
{
    .owner = THIS_MODULE,
    .open = my_open,
    .read = my_read,
    .write = my_write,
    .release = my_close,
                                                   // .unlocked_ioctl = my_ioctl,
};


static int __init my_driver_init(void)
{
    int ret;
    struct device *dev_ret;

    /*Allocate major number and minor number*/
    if((ret=alloc_chrdev_region(&dev,0,1,"my_proc_dev")) < 0)
    {
        printk(KERN_INFO"proc file system can't be created\n");
        return -1;
    }

    /*creating cdev structure*/
    cdev_init(&c_dev, &my_fops);
    
    /*Adding character device to the system*/
    if((ret = cdev_add(&c_dev,dev,1)) <0)
    {

        printk(KERN_INFO"Device can't be added\n");
        return ret;
    }

    /*Creating struct class*/
    if(((cl = class_create(THIS_MODULE,"char")) == NULL))
    {
        printk(KERN_INFO"Can bot create struct class\n");
        cdev_del(&c_dev);
        unregister_chrdev_region(dev,1);
        return PTR_ERR(cl);
    }

    /*Creating Device*/
    if(IS_ERR(dev_ret = device_create(cl,NULL,dev,NULL,"mutex_device")))
    {
        class_destroy(cl);
        cdev_del(&c_dev);
        unregister_chrdev_region(dev,1);
        return PTR_ERR(dev_ret);
    }

    /*Initialize mutex*/
    mutex_init(&my_mutex);

    /*Creating Thread 1*/
    my_thread1 = kthread_run(thread_function1,NULL,"thread_1");
    if(my_thread1)
    {
        printk(KERN_INFO"Thread 1 created succesfully....\n");
    }
    else
    {
        printk(KERN_INFO"Thread 1 not created!!!!!\n");
        class_destroy(cl);
        return PTR_ERR(my_thread1);
    }
    /*Creating Thread 2*/
    my_thread2 = kthread_run(thread_function2,NULL,"thread_2");
    if(my_thread2)
    {
        printk(KERN_INFO"Thread 2 created succesfully....\n");
    }
    else
    {
        printk(KERN_INFO"Thread 2 not created!!!!!\n");
        class_destroy(cl);
        return PTR_ERR(my_thread2);
    }
    printk(KERN_INFO"Device Driver Insert Done......\n");
    return 0;
}

static void __exit my_driver_exit(void)
{
    kthread_stop(my_thread1);
    kthread_stop(my_thread2);
    device_destroy(cl,dev);
    class_destroy(cl);
    cdev_del(&c_dev);
    unregister_chrdev_region(dev,1);
    printk(KERN_INFO"Device Driver Remove Done......\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sunil Kumar Sahu");
MODULE_DESCRIPTION("Mutex example with thread");

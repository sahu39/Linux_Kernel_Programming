#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>

int valueETX,arr_valueETX[4];
char *nameETX;
int cb_valueETX;

module_param(valueETX,int,S_IRUSR | S_IWUSR);
module_param(nameETX,charp,S_IRUSR | S_IWUSR);
module_param_array(arr_valueETX,int,NULL,S_IRUSR | S_IWUSR);
/*****module parameter callback***************************/
int notify_param(const char *val,const struct kernel_param *kp)
{
    int ret = param_set_int(val,kp);
    if(ret == 0)
    {
        printk(KERN_INFO"callback function called\n");
        printk(KERN_INFO"New value of cb_valueETX=%d\n",cb_valueETX);
        return 0;
    }
    return -1;
}
const struct kernel_param_ops my_param_ops =
{
    .set=&notify_param,
    .get=&param_get_int,
};
module_param_cb(cb_valueETX,&my_param_ops,&cb_valueETX,S_IRUGO | S_IWUSR );

/*****module parameter callback***************************/
static int __init hello_world_init(void)
{
    int i;
    printk(KERN_INFO"valueETX=%d\n",valueETX);
    printk(KERN_INFO"cb_valueETX=%d\n",cb_valueETX);
    printk(KERN_INFO"nameETX=%s\n",nameETX);
    for(i=0;i<=3;i++)
    {
        printk(KERN_INFO"arr_valueETX[%d]=%d\n",i,arr_valueETX[i]);
    }
    return 0;
}

void __exit hello_world_exit(void)
{
    printk(KERN_INFO"Kernel Module removed successfully\n");

}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Sunil Kumar Sahu <mr.sahu39@gmail.com>");
MODULE_DESCRIPTION("A simple hello world driver");
MODULE_VERSION("2:1.1");


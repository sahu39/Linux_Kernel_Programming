#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

 
#define SET_VALUE _IOW('q', 1, char *)
#define PROC_SET_VALUE _IOW('q', 2, char *)

int main()
{
    int fd;
    char msg[60] = "\0";//"TeamF1 module has been executed through IOCTL call.";
    strcpy(msg,"TeamF1 module has been executed through IOCTL call.");
    
    fd = open("/dev/query",O_RDWR);
    if(fd<0)
    {
        printf("Can not open device file\n");
        return -1;
    }
    printf("Writting to driver query...\n");
    ioctl(fd,SET_VALUE,(char *)msg);
    printf("closing Driver query...\n");
    
    fd = open("/proc/my_msg",O_RDWR);
    if(fd<0)
    {
        printf("Can not open device file\n");
        return -1;
    }
    printf("Writting to driver kmsg...\n");
    ioctl(fd,PROC_SET_VALUE,(char *)msg);
    printf("closing Driver kmsg...\n");
    return 0;
}


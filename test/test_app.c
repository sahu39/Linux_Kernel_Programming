#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>

 
#define SET_VALUE _IOW('q', 1, char *)

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
    printf("Writting to driver...\n");
    ioctl(fd,SET_VALUE,(char *)msg);
    printf("closing Driver...\n");
    return 0;
}


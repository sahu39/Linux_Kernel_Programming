obj-m := 07_hello_world_module.o

ifdef ARCH
	KDIR=/home/km/KM_GITHUB/beagleboneblack-kernel
else
	KDIR=/lib/modules/$(shell uname -r)/build #uname -r = 5.4.0-135-generic
endif

all:
ifdef ARCH
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C ${KDIR} M=$(shell pwd) modules
else
	make -C ${KDIR} M=$(shell pwd) modules
endif

clean:
ifdef ARCH
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C ${KDIR} M=$(shell pwd) clean
else
	make -C ${KDIR} M=$(shell pwd) clean
endif



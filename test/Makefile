obj-m := test_driver_2.o
KDIR=/lib/modules/$(shell uname -r)/build #uname -r = 3.13.0-62-generic

all:
	make -C ${KDIR} M=$(shell pwd) modules

clean:
	make -C ${KDIR} M=$(shell pwd) clean


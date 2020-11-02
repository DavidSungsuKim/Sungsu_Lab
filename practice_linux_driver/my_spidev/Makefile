KERNELDIR = /lib/modules/$(shell uname -r)/build

obj-m = spidev.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.ko
	rm -rf *.mod
	rm -rf *.mod.*
	rm -rf *.cmd
	rm -rf *.o
	rm -rf *.order
	rm -rf *.symvers
	rm -rf *.a
	
clean_but_ko:
	rm -rf *.mod
	rm -rf *.mod.*
	rm -rf *.cmd
	rm -rf *.o
	rm -rf *.order
	rm -rf *.symvers
	rm -rf *.a	

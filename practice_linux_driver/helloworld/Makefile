KERNELDIR = /home/pi/git_repository/Kernel/linux-4.19.114

obj-m = helloworld.o

KDIR := /home/pi/git_repository/Kernel/linux-4.19.114
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) SUBDIRS=$(PWD) modules
	
clean:
	rm -rf *.ko
	rm -rf *.mod.*
	rm -rf .*.cmd
	rm -rf *.o

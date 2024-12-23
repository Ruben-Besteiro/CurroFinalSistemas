ifneq ($(KERNELRELEASE),)

	obj-m := hello.o

else

	KERN_DIR ?= /usr/src/linux-headers-$(shell uname -r)/
	PWD := $(shell pwd)

all:

	$(MAKE) -C $(KERN_DIR) M=$(PWD) modules

endif

obj-m   := misc.o
KDIR    := /lib/modules/$(shell uname -r)/build
PWD     := $(shell pwd)
all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules
	cc use.c -o use
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -rf use

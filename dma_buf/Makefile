KERNELDIR:=/lib/modules/$(shell uname -r)/build
CURRENT_PATH := $(shell pwd)

obj-m := exporter-page.o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
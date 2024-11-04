
all:  bootloader kernel userland image
buddy: bootloader kernel_buddy userland_buddy image_buddy

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

kernel_buddy:
	cd Kernel; make buddy

userland:
	cd Userland; make all

userland_buddy:
	cd Userland; make buddy

image: kernel bootloader userland
	cd Image; make all

image_buddy: kernel_buddy bootloader userland_buddy
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Userland; make clean

.PHONY: bootloader image image_buddy collections kernel kernel_buddy userland userland_buddy all clean buddy

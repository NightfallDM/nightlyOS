#boot.bin : boot.elf
#	objcopy -O binary -j .text boot.elf boot.bin
#	objdump -D boot.elf>temp


#boot.elf: boot1.o
#	ld -g -Ttext 0x7c00 -e start boot1.o -o boot.elf

#boot1.o: boot1.s
#	as -g boot1.s -o boot1.o

#boot1.s : boot1.S
#	cpp -I../ boot1.S -o boot1.s # !!!!!!!!!!!!!!!!

#clean:
#	rm -f boot1.o boot.bin boot.elf boot1.d boot1.s boot.img

#qemu:
#	dd if=/dev/zero of=boot.img bs=512 count=1
#	dd if=boot.bin of=boot.img bs=512 count=1
#	xxd boot.img>check
#	qemu-system-x86_64 -s -S -hda boot.img
CC = gcc
CPP = cpp
AS = as
LD = ld
OBJCOPY = objcopy
OBJDUMP = objdump
CFLAGS = -fno-pic -static -fno-builtin -fno-omit-frame-pointer
CFLAGS += -fno-stack-protector -I../ -g -nostdinc

target := setup.elf setup.bin vmnight.bin vmnight.img
setup-obj-y := header.S 
build = tools/build

setup.elf: $(setup-obj-y)
	$(CC) $(CFLAGS) $(setup-obj-y) -o $@

vmnight.img: boot
	dd if=/dev/zero of=vmnight.img bs=1024 count=1024
	dd if=boot of=vmnight.img conv=notrunc
	dd if=vmnight of=vmnight.img seek=1 conv=notrunc

boot: boot.S
	$(CPP) -I../ boot.S -o boot.s
	$(AS) -g boot.s -o boot.o
	$(LD) -e start -Ttext 0x7c00 boot.o -o boot.elf
	$(OBJCOPY) -O binary -j .text boot.elf boot
#	$(OBJDUMP) -D boot.elf>temp_boot1

#load: load.c
#	$(CC) $(CFLAGS) -c load.c -o load.o
#	$(LD) -e loadmain -Ttext 0x7e00 load.o -o load.elf
#	$(OBJCOPY) -O binary -j .text load.elf load
#	$(OBJDUMP) -D load.elf>temp_load

#boot: load.o boot1.o
#	$(LD) -e start -Ttext 0x7c00 boot1.o load.o -o boot.elf
#	$(OBJDUMP) -D boot>temp
#	$(OBJCOPY) -O binary -j .text boot.elf boot
qemu:
#	xxd vmnight.img >check
	qemu-system-x86_64 -m 1G -hda vmnight.img

qemu-debug:
	xxd vmnight.img>check
	qemu-system-x86_64 -s -S -m 1G -hda vmnight.img

clean:
	rm -f vmnight.img boot.s boot.o boot.elf boot $(target)

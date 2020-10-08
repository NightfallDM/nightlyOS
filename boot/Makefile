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
boot1.img: boot1 load
	dd if=/dev/zero of=boot1.img count=4
	dd if=boot1 of=boot1.img conv=notrunc
#	dd if=load of=boot1.img seek=1 conv=notrunc

boot1: boot1.S load.o
	$(CPP) -I../ boot1.S -o boot1.s
	$(AS) -g boot1.s -o boot1.o
	$(LD) -e start -Ttext 0x7c00 boot1.o load.o -o boot1.elf
	$(OBJCOPY) -O binary -j .text boot1.elf boot1
	$(OBJDUMP) -D boot1.elf>temp_boot1

load: load.c
	$(CC) $(CFLAGS) -c load.c -o load.o
	$(LD) -e loadmain -Ttext 0x7e00 load.o -o load.elf
	$(OBJCOPY) -O binary -j .text load.elf load
	$(OBJDUMP) -D load.elf>temp_load

#boot: load.o boot1.o
#	$(LD) -e start -Ttext 0x7c00 boot1.o load.o -o boot.elf
#	$(OBJDUMP) -D boot>temp
#	$(OBJCOPY) -O binary -j .text boot.elf boot
qemu:
	xxd boot1.img >check
	qemu-system-x86_64 -m 1G -hda boot1.img

qemu-debug:
	xxd boot1.img>check
	qemu-system-x86_64 -s -S -m 1G -hda boot1.img

clean:
	rm -f boot1.img boot1.s boot1.o load.o load.elf boot1.elf boot1 load

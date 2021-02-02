CC = gcc
CPP = cpp
AS = as
LD = ld
OBJCOPY = objcopy
OBJDUMP = objdump
CFLAGS = -fno-pic -static -fno-builtin -fno-omit-frame-pointer
CFLAGS += -fno-stack-protector -I. -g -nostdinc
boot.img: boot
	dd if=/dev/zero of=boot.img count=4
	dd if=boot of=boot.img conv=notrunc
#	dd if=load of=boot1.img seek=1 conv=notrunc

boot: test_video_set-bios.S
	$(AS) --32 test_video_set-bios.S -o target.o
	$(LD) -melf_i386 -e _start -Ttext 0x7c00 target.o -o boot.elf
	$(OBJCOPY) -O binary -j .text boot.elf boot
	$(OBJDUMP) -D boot.elf>temp_boot

#boot: load.o boot1.o
#	$(LD) -e start -Ttext 0x7c00 boot1.o load.o -o boot.elf
#	$(OBJDUMP) -D boot>temp
#	$(OBJCOPY) -O binary -j .text boot.elf boot
qemu:
	xxd boot.img >check
	qemu-system-i386 -m 1G -hda boot.img

clean:
	rm -f boot.img target.o boot.elf boot check temp_boot

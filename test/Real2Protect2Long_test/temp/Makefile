test64.img: test64 setup
	dd if=/dev/zero of=test64.img bs=512 count=4
	dd if=test64 of=test64.img bs=512 count=1 conv=notrunc
	dd if=setup of=test64.img seek=1 conv=notrunc

test64: test64.S
	as -g test64.S -o test64.o
#	as -g loader.S -o loader.o
	ld -e main -Ttext 0x7c00 test64.o -o test64.elf
	objdump -D test64.elf >temp
	objcopy -O binary test64.elf test64

setup: setup.c
	gcc -c -g -nostdlib -nostdinc -fno-pic -fno-builtin -fno-omit-frame-pointer setup.c -o setup.o
	ld -e setup -Ttext 0x7e00 setup.o -o setup.elf
	objcopy -O binary -j .text setup.elf setup

qemu-debug:
	xxd test64.img>check
	qemu-system-x86_64 -s -S -hda test64.img -m 2G

qemu:
	xxd test64.img > check
	qemu-system-x86_64 -hda test64.img -m 2G

clean:
	rm -f test64.o test64.img test64 test64.elf setup loader loader.o setup.elf setup.o

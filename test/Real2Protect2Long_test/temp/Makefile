test64.img: test64 loader
	dd if=/dev/zero of=test64.img bs=512 count=4
	dd if=test64 of=test64.img bs=512 count=1
	dd if=loader of=test64.img seek=1 bs=512 count=1

test64: test64.S
	as -g test64.S -o test64.o
	ld -e main -Ttext 0x7c00 test64.o -o test64.elf
	objcopy -O binary test64.elf test64

loader: setup.c
	gcc -nostdlib -nostdinc -Wl,-esetup setup.c -o setup
	objcopy -O binary setup loader

qemu-debug:
	qemu-system-x86_64 -s -S -hda test64.img

qemu:
	qemu-system-x86_64 -hda test64.img

clean:
	rm -f test64.o test64.img test64 test64.elf setup loader

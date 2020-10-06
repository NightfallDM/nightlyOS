test.img: test
	dd if=/dev/zero of=test.img bs=512 count=1
	dd if=test of=test.img bs=512 count=1
test: mode_switch.S
	cpp -I../ mode_switch.S -o test.s
	as --32 -g test.s -o test.o
	ld -melf_i386 -e start -Ttext 0x0 test.o -o test.elf
	objcopy -O binary test.elf test

qemu: test.img
	qemu-system-i386 -hda test.img

clean:
	rm -f test*

boot.img: mode_switch
	dd if=/dev/zero of=boot.img bs=512 count=1
	dd if=mode_switch of=boot.img bs=512 count=1

mode_switch: mode_switch.S
	cpp -I../ mode_switch.S -o mode_switch.s
	as --32 -g mode_switch.s -o mode_switch.o
	ld -melf_i386 -e start -Ttext 0x0 mode_switch.o -o mode_switch.elf
	objcopy -O binary mode_switch.elf mode_switch

qemu:
	qemu-system-i386 -s -S -hda boot.img &
	sleep 1
	cgdb -x gdbinit

clean:
	rm -f boot.img mode_switch.elf mode_switch.o mode_switch.s mode_switch

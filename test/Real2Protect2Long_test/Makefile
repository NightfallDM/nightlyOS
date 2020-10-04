boot.img: mode_switch
	dd if=/dev/zero of=boot.img bs=512 count=1024
	dd if=test of=boot.img bs=524288 count=1

mode_switch: mode_switch.S
	cpp -I../ mode_switch.S -o mode_switch.s
	cpp -I../ protected2long.S -o protected2long.s
	as -g mode_switch.s -o mode_switch.o
	as -g protected2long.s -o protected2long.o
	as -g long_mode_program.S -o long_mode_program.o
	ld -e start -Ttext 0x0 mode_switch.o protected2long.o long_mode_program.o -o test.elf
	objcopy -O binary test.elf test
	#ld -melf_i386 -e start -Ttext 0x0 mode_switch.o -o mode_switch.elf
	#objcopy -O binary mode_switch.elf mode_switch

qemu:
	qemu-system-x86_64 -s -S -hda boot.img $
	sleep 1
	cgdb -x gdbinit

clean:
	rm -f boot.img mode_switch.o mode_switch.s mode_switch test* *.o *.s *.elf

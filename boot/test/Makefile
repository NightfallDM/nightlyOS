stage2_OBJS = 	main.o\
				pm.o\
				
boot1: ../boot1.S ../load.c
	cpp -I ../ ../boot1.S -o boot1.s
	as -g --32 boot1.s -o boot1.o
	gcc -c -m32 ../load.c -o load.o
	ld -melf_i386 -e start -Ttext 0x7c00 boot1.o load.o -o boot1.elf
	objcopy -O binary -j .text boot1.elf boot1

boot.img: ../boot.S ../header.S
	cpp -I ../ ../boot.S -o boot.s
	cpp -I ../ ../header.S -o header.s
	as -g --32 boot.s -o boot.o
	as -g --32 header.s -o header.o
	as -g --32 ../copy.S -o copy.o
	gcc -Wall -ffreestanding -Wstrict-prototypes -mregparm=3 -g -O2 -m16 -static -fno-strict-aliasing -fno-builtin -fno-pic -fno-stack-protector -fno-omit-frame-pointer -c test.c -o test.o
	gcc -Wall -ffreestanding -Wstrict-prototypes -mregparm=3 -g -O2 -m16 -static -fno-strict-aliasing -fno-builtin -fno-pic -fno-stack-protector -fno-omit-frame-pointer -c ../tty.c -o tty.o
	#ld -melf_i386 -e start_boot -Ttext 0x7c00 boot.o -o boot.elf
	ld -melf_i386 -Tsetupbl.ld boot.o -o boot.elf
	ld -melf_i386 -Tsetup.ld header.o test.o copy.o tty.o -o header.elf
	#objcopy -O binary -j .text boot.elf boot
	objcopy -O binary -j .bltext boot.elf boot
	objcopy -O binary header.elf header
	dd if=/dev/zero of=boot.img bs=1024 count=1024
	dd if=boot of=boot.img conv=notrunc
	dd if=header of=boot.img bs=1 seek=512 count=4096 conv=notrunc
	objdump -DS boot.elf > boot_dump
	objdump -DS header.elf > header_dump
	#dd if=header of=boot.img bs=1 skip=47 seek=1010 count=512 conv=notrunc
	#dd if=vmnight of=vmnight.img seek=1 conv=notrunc

qemu: boot.img
	qemu-system-x86_64 boot.img -monitor tcp:127.0.0.1:55555,server,nowait;
	#qemu-system-x86_64 boot.img -nographic -amonitor tcp:127.0.0.1:55555,server,nowait;

qemu_step: boot.img
	qemu-system-x86_64 boot.img  -s -S -monitor tcp:127.0.0.1:55555,server,nowait;
	#qemu-system-x86_64 boot.img -nographic -s -S -monitor tcp:127.0.0.1:55555,server,nowait;

clean:
	rm -rf *.o *.elf *.s boot1 boot *.img header *_dump

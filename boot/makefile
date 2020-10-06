boot.bin : boot.elf
	objcopy -O binary boot.elf boot.bin


boot.elf: boot1.o
	ld -Ttext 0x0 -e start -melf_i386 boot1.o -o boot.elf

boot1.o: boot1.s
	as --32 -g boot1.s -o boot1.o

boot1.s : boot1.S
	cpp -I../ boot1.S -o boot1.s # !!!!!!!!!!!!!!!!

clean:
	rm -f boot1.o boot.bin boot.elf boot1.d boot1.s boot.img

qemu:
	dd if=/dev/zero of=boot.img bs=512 count=1
	dd if=boot.bin of=boot.img bs=512 count=1
	qemu-system-i386 -s -S -hda boot.img

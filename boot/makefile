objs := boot.o boot.elf boot.bin

boot.img : $(objs)
	dd if=boot.bin of=boot.img bs=512 count=1

boot.o : boot.S
	as --32 boot.S -o boot.o

boot.elf : boot.o
	ld -Ttext 0x0 -e start_boot -melf_i386 boot.o -o boot.elf

boot.bin : boot.elf
	objcopy -O binary boot.elf boot.bin

clean:
	rm -f $(objs)

objs := boot.o boot.elf boot.bin setup.o setup.elf setup.bin boot.img

boot.img : $(objs)
	dd if=/dev/zero of=boot.img bs=512 count=5
	dd if=boot.bin of=boot.img bs=512 count=1
	dd if=setup.bin of=boot.img seek=1 bs=512 count=4


boot.o : boot.S
	as --32 boot.S -o boot.o

boot.elf : boot.o
	ld -Ttext 0x0 -e start_boot -melf_i386 boot.o -o boot.elf

boot.bin : boot.elf
	objcopy -O binary boot.elf boot.bin

setup.o : setup.S
	as --32 setup.S -o setup.o

setup.elf : setup.o
	ld -Ttext 0x0 -e start_setup -melf_i386 setup.o -o setup.elf

setup.bin : setup.elf
	objcopy -O binary setup.elf setup.bin

clean:
	rm -f $(objs)

start:
	qemu-system-i386 -hda boot.img

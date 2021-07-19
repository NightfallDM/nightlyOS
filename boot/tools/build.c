#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdarg.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include"le_byteshift.h"

/*
 * Use "setup.bin" "vmnight.bin" to gen disk image. 
 * */

#define SETUP_SECT_MAX 64

unsigned char buf[SETUP_SECT_MAX*512];

static void dir(const char *str, ...){
	va_list args;
	va_start(args, str);
	vfprintf(stderr, str, args);
	va_end(args);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char **argv){
	unsigned int setup_sectors, kernel_size;
	int c;
	void *kernel;
	unsigned int padding;
	unsigned char *padding_buf;
	struct stat sb;
	FILE *file, *dest;
	int fd;
	
	if(argc != 4)
		dir("wrong way to use build.c");
	
	file = fopen(argv[1], "r");
	if (!file)
		dir("Unable to open '%s'", argv[1]);

	dest = fopen(argv[3], "w");
	if(!dest)
		dir("Unable to open '%s'", argv[3]);
	c = fread(buf, 1, sizeof(buf), file);
	fclose(file);
	setup_sectors = c / 512 + (c%512)? 1: 0;
	buf[0x0] = setup_sectors;

	fd = open(argv[2], 0RD_ONLY);
	if(fd<0)
		die("Unable to open '%s'", argv[2]);
	if (fstat(fd, &sb))
		dir("Unable to stat '%s'", argv[2]);
	kernel_size = sb.st_size;
	kernel_sectors = kernel_size / 512 + (kernel_size%512)? 1: 0;
	padding = kernel_sectors * 512 - kernel_size;
	if (fwrite(buf, 1, setup_sectors*512, dest) != setup_sectors*512)
		dir("Writing setup failed");
	/* file = fopen(argv[2], "r");
	if (!file)
		die("Unable to open '%s'", argv[2]);
	unsigned char *buf_kernel = malloc(kernel_sectors*512);
	unsigned int padding = kernel_sectors*512 - kernel_size;
	if (fwrite(buf_kernel)) */
	
	kernel = mmap(NULL, kernel_size, PROT_READ, MAP_SHARED, fd, 0);
	if(kernel == MAP_FAILED)
		die("Unable to mmap '%s'", argv[2]);
	padding_buf = malloc(padding);
	memset(padding_buf, 0, padding);
	if (fwrite(kernel, 1, kernel_size, dest) != kernel_size)
		die("Writing kernel failed");
	if (fwrite(padding_buf, 1, padding, dest) != padding)
		die("Writing padding failed");
	if (fclose(dest))
		die("Writing image failed");
	close(fd);
	free(padding_buf);
	return 0;

}

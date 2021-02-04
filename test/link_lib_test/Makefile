target: libadd.a main.c
	gcc main.c -L. -ladd -o target

libadd.a: add.c
	gcc -c add.c -o add.o
	ar crs libadd.a add.o

clean:
	rm -f libadd.a add.o target

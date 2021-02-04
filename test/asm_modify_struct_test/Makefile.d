target: test_df_struct.c libchange.a
	gcc -L. -lchange -ldl test_df_struct.c -o target
	#gcc test_df_struct.c modify.o -o target


libchange.a: modify.S
	as modify.S -o modify.o
	ar crs libchange.a modify.o

clean:
	rm -f libchange.a modify.o target

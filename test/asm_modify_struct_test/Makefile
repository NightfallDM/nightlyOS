target: test_df_struct.c modify.o
	gcc test_df_struct.c modify.o -o target


modify.o: modify.S
	as modify.S -o modify.o
	#ar crs libchange.a modify.o

clean:
	rm -f libchange.a modify.o target

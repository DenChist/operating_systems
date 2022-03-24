all: sprog.out dprog.out

sprog.out: libs1.a
	gcc smain.c -L. -ls1 -Wall -o sprog.out -lm

libs1.a: lib1.o
	ar rc libs1.a lib1.o
	ranlib libs1.a

dprog.out: libd1.so libd2.so
	gcc dmain.c -ldl -o dprog.out -lm

libd1.so: lib1.o
	gcc lib1.o -lm -shared -o libd1.so -lm

libd2.so: lib2.o
	gcc lib2.o -shared -o libd2.so -lm

lib1.o:
	gcc lib1.c -c -Wall -Werror -fpic -o lib1.o -lm

lib2.o:
	gcc lib2.c -c -Wall -Werror -fpic -o lib2.o -lm

clean:
	rm -rf *.a *.so *.o sprog.out dprog.out

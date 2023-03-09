libs: bigint.c
	gcc -c bigint.c
	ar -rcs libbigint.a bigint.o
	rm bigint.o
main: main.c
	gcc main.c -L. -lbigint -o testing
buildandrunstuff: main.c bigint.c bigint.h
	make libs
	make main
	./testing
cleanstuff:
	rm testing
	rm libbigint.a
dumblib: stuff.c
	gcc -c stuff.c
	ar -rcs libstuff.a stuff.o
	rm stuff.o
dumbmain: main.c
	gcc main.c -L. -lstuff -o dumb
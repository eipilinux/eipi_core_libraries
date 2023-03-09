libs: bigint.c
	gcc -c bigint.c
	ar -rcs libbigint.a bigint.o
	rm bigint.o
test: tests.c
	gcc tests.c -L. -lbigint -o testing
all: tests.c bigint.c bigint.h
	make libs
	make test
	./testing
clean:
	rm testing
	rm libbigint.a

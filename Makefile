all: bloom_test

bloom_test: main.o MurmurHash3.o bloom.o
	gcc main.o MurmurHash3.o bloom.o -o bloom_test

main.o: main.c
	gcc -c main.c

MurmurHash3.o: MurmurHash3.c
	gcc -c MurmurHash3.c

bloom.o: bloom.c
	gcc -c bloom.c

clean:
	rm -f *o *~ bloom_test

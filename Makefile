all: bloom_test

bloom_test: bloom_test.o MurmurHash3.o bloom.o
	gcc bloom_test.o MurmurHash3.o bloom.o -o bloom_test

bloom_test.o: bloom_test.c
	gcc -c bloom_test.c

MurmurHash3.o: MurmurHash3.c
	gcc -c MurmurHash3.c

bloom.o: bloom.c
	gcc -c bloom.c

clean:
	rm -f *o *~ bloom_test

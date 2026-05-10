CC=gcc
CFLAGS=-Wall -O3

.PHONY: all clean test test-c11

all: test test-c11

test:
	$(CC) $(CFLAGS) -std=c99 test.c -o test
	./test

test-c11:
	$(CC) $(CFLAGS) -std=c11 -DSKIPLIST_USE_PRINT=1 test.c -o test-c11
	./test-c11

clean:
	rm -rf test test-c11 *.o

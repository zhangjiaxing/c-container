.PHONY: all clean
CC=clang
CFLAGS=-Wall -O3

all: skiplist

skiplist: skiplist.c test.c
	$(CC) $(CFLAGS) $^ -o $@ 

clean:
	rm -rf skiplist


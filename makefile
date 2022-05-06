all: skiplist
.PHONY: all

skiplist: skiplist.c test.c
	$(CC) $^ -o $@ 

clean:
	rm -rf skiplist

.PHONY: clean

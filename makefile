all: skiplist
.PHONY: all

ranklist: skiplist.c
	$(CC) $^ -o $@ 

clean:
	rm -rf skiplist

.PHONY: clean

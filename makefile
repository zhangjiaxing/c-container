all: ranklist
.PHONY: all

ranklist: rank_skiplist.c
	$(CC) $^ -o $@ 

clean:
	rm -rf ranklist

.PHONY: clean

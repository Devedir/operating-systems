CC = gcc
CFLAGS = -Wall -std=c17 -g
Executables = nforks

.PHONY: all
all: $(Executables)

nforks: nforks.c
	$(CC) $(CFLAGS) nforks.c -o nforks

.PHONY: test
test: all
	./nforks 5

.PHONY: clean
clean:
	rm -f *.o *.a *.so *.so.* $(Executables)

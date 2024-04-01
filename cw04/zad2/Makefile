CC = gcc
CFLAGS = -Wall -std=c17 -g
Executables = forky_ls

.PHONY: all
all: $(Executables)

forky_ls: forky_ls.c
	$(CC) $(CFLAGS) forky_ls.c -o forky_ls

.PHONY: test
test: all
	./forky_ls .

.PHONY: clean
clean:
	rm -f *.o *.a *.so *.so.* $(Executables)

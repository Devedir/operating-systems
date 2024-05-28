CC = gcc
CFLAGS = -Wall -std=c17 -g
Executables = santa

.PHONY: all
all: $(Executables)

santa: santa.c
	$(CC) santa.c -o santa -lpthread

.PHONY: test
test: $(Executables)
	./santa

.PHONY: clean
clean:
	rm -f *.o *.a *.so *.so.* *.fifo $(Executables)

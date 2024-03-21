CC = gcc
CFLAGS = -Wall -std=c17 -g
Executables = flipper

.PHONY: all
all: $(Executables)

flipper: flipper.c
	$(CC) flipper.c -o flipper

.PHONY: clean
clean:
	rm -f *.o *.a *.so *.so.* $(Executables)

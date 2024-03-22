CC = gcc
CFLAGS = -Wall -std=c17 -g
Executables = flipper

.PHONY: all
all: $(Executables)

flipper: flipper.c
	$(CC) $(CFLAGS) flipper.c -o flipper

.PHONY: test
test: all
	./flipper ./art ./flipped

.PHONY: clean
clean:
	rm -rf flipped*
	rm -f *.o *.a *.so *.so.* $(Executables)

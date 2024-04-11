CC = gcc
CFLAGS = -Wall -std=c17 -g
Executables = reactor

.PHONY: all
all: $(Executables)

reactor: reactor.c
	$(CC) $(CFLAGS) reactor.c -o reactor

.PHONY: test
test: all
	./reactor handler

.PHONY: clean
clean:
	rm -f *.o *.a *.so *.so.* $(Executables)

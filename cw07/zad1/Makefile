CC = gcc
CFLAGS = -Wall -std=c17 -g
Executables = server client

.PHONY: all
all: $(Executables)

server: server.c msgdefs.h
	$(CC) $(CFLAGS) server.c -o server -lrt

client: client.c msgdefs.h
	$(CC) $(CFLAGS) client.c -o client -lrt

# 'make test' doesn't make much sense here

.PHONY: clean
clean:
	rm -f *.o *.a *.so *.so.* *.fifo $(Executables)

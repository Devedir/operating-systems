CC = gcc
CFLAGS = -Wall -std=c17 -g
Executables = server client

.PHONY: all
all: $(Executables)

libcommon.a: common.c
	$(CC) $(CFLAGS) -c common.c
	ar rcs libcommon.a common.o

server: server.c libcommon.a
	$(CC) $(CFLAGS) server.c -o server -lrt -L. -lcommon -lpthread

client: client.c libcommon.a
	$(CC) $(CFLAGS) client.c -o client -lrt -L. -lcommon

# 'make test' doesn't make much sense here

.PHONY: clean
clean:
	rm -f *.o *.a *.so *.so.* *.fifo $(Executables)

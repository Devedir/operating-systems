CC = gcc
CFLAGS = -Wall -std=c17 -g -O0
Executables = static_exe shared_exe dynamic_exe

.PHONY: all
all: $(Executables)

static_exe: libcollatz.a client.o
	$(CC) client.o -L. -lcollatz -o static_exe

libcollatz.a: collatz.c
	$(CC) $(CFLAGS) -c collatz.c
	ar rcs libcollatz.a collatz.o

shared_exe: libcollatz.so client.o
	$(CC) client.o -L. -lcollatz -Wl,-rpath,. -o shared_exe

libcollatz.so: collatz.c
	$(CC) $(CFLAGS) -fPIC -c collatz.c
	$(CC) collatz.o -shared -o libcollatz.so

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

dynamic_exe: client.c
	$(CC) $(CFLAGS) client.c -o dynamic_exe -ldl -D DYNAMIC

.PHONY: clean
clean:
	rm -f *.o *.a *.so *.so.* $(Executables)

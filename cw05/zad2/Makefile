CC = gcc
CFLAGS = -Wall -std=c17 -g
Executables = catcher sender

.PHONY: all
all: $(Executables)

catcher: catcher.c
	$(CC) $(CFLAGS) catcher.c -o catcher

sender: sender.c
	$(CC) $(CFLAGS) sender.c -o sender

# 'make test' doesn't make much sense here -> I tried and failed to make it reasonably well :)

.PHONY: clean
clean:
	rm -f *.o *.a *.so *.so.* $(Executables)

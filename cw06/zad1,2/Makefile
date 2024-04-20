CC = gcc
CFLAGS = -Wall -std=c17 -g
Executables = integrator integrate

.PHONY: all
all: $(Executables)

integrator: integrator.c
	$(CC) $(CFLAGS) integrator.c -o integrator

integrate: integrate.c
	$(CC) $(CFLAGS) integrate.c -o integrate

# 'make test' doesn't make much sense here

.PHONY: clean
clean:
	rm -f *.o *.a *.so *.so.* *.fifo $(Executables)

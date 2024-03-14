CC = gcc
CFLAGS = -Wall -std=c17 -g
# Powyzsze flagi: Wszystkie warningi, standard C17, symbole do debugowania w binarce
Executables = countdown

.PHONY: all
all: $(Executables)

countdown: countdown.c
	$(CC) $(CFLAGS) -o countdown countdown.c

.PHONY: clean
clean:
	rm -f *.o $(Executables)

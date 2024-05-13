CC = gcc
CFLAGS = -Wall -std=c17 -g
Executables = controller

.PHONY: all
all: $(Executables)

controller: controller.c
	$(CC) $(CFLAGS) controller.c -o controller

.PHONY: test
test: $(Executables)
	./controller 5 3

.PHONY: clean
clean:
	rm -f *.o *.a *.so *.so.* *.fifo $(Executables)

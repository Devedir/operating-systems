CC = gcc
CFLAGS = -Wall -std=c17 -g
Executables = controller user

.PHONY: all
all: $(Executables)

libqueue.a: queue.c
	$(CC) $(CFLAGS) -c queue.c
	ar rcs libqueue.a queue.o

controller.o: controller.c
	$(CC) $(CFLAGS) -c controller.c

controller: controller.o libqueue.a
	$(CC) controller.o -o controller -lrt -L. -lqueue -pthread

user: user.o libqueue.a
	$(CC) user.o -o user -lrt -L. -lqueue -pthread

user.o: user.c
	$(CC) $(CFLAGS) -c user.c

.PHONY: test
test: $(Executables)
	./controller 5 3

.PHONY: clean
clean:
	rm -f *.o *.a *.so *.so.* *.fifo $(Executables)

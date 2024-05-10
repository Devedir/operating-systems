#include "zad3.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void readwrite(int pd, size_t block_size);

void createpipe(size_t block_size)
{
    /* Utwórz potok nienazwany */
    int fd[2];
    pipe(fd);

    /* Odkomentuj poniższe funkcje zamieniając ... na deskryptory potoku */
    check_pipe(fd);
    check_write(fd, block_size, readwrite);
}

void readwrite(int write_pd, size_t block_size)
{
    /* Otworz plik `unix.txt`, czytaj go po `block_size` bajtów
    i w takich `block_size` bajtowych kawałkach pisz do potoku `write_pd`.*/
    int file = open("unix.txt", O_RDONLY);
    char* buf = malloc(block_size);
    size_t bytes = read(file, buf, block_size);
    while (bytes > 0) {
        write(write_pd, buf, bytes);
        bytes = read(file, buf, block_size);
    }

    /* Zamknij plik */
    close(file);
    free(buf);
}

int main()
{
    srand(42);
    size_t block_size = rand() % 128;
    createpipe(block_size);

    return 0;
}
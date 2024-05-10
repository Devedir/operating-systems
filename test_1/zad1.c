#include "zad1.h"
#include <stdio.h>

void read_file(const char *file_path, char *str)
{
    /* Wczytaj tekst pliku file_path do bufora str */
    /* Pamiętaj aby dodać do str znak zakończenia string-a '\0' */

    FILE* file = fopen(file_path, "r");
    char byte;
    size_t i = 0;
    while (fread(&byte, sizeof(char), 1, file) == 1) {
        str[i] = byte;
        i++;
    }
    fclose(file);
    str[i] = '\0';
}

void write_file(const char *file_path, char *str)
{
    /* Zapisz tekst z bufora str do pliku file_path */
    /* Zapisz tylko tyle bajtów ile potrzeba. */
    /* Bufor może być większy niż tekst w nim zawarty*/
    size_t len = strlen(str);
    FILE* file = fopen(file_path, "w");
    fwrite(str, sizeof(char), len, file);
    fclose(file);
}

int main(int arc, char **argv)
{
    char buffer[256];
    read_file(IN_FILE_PATH, buffer);
    check_buffer(buffer);

    char response[] = "What's the problem?";
    write_file(OUT_FILE_PATH, response);
    check_file();
}

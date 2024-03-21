#include <stdio.h>
#include <dirent.h>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Two arguments required: paths to the source and target directories.");
        return 1;
    }
    char* source_dir = argv[1];
    char* target_dir = argv[2];
    return 0;
}
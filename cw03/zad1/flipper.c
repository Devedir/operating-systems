#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

#define MAX_PATH_LEN 256
#define MAX_LINE_LEN 1024

void flip(FILE* input, FILE* output) {
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Two arguments required: paths to the source and target directories.");
        return 1;
    }
    const char* source_dir_path = argv[1];
    const char* target_dir_path = argv[2];

    int caught_result;

    DIR* source_dir_handle = opendir(source_dir_path);
    if (source_dir_handle == NULL) {
        perror("Directory opening error");
        return errno;
    }

    struct dirent* entry;
    while((entry = readdir(source_dir_handle))) {
        size_t namelen = strlen(entry->d_name);
        if (namelen < 4) continue;
        char* format = entry->d_name + namelen - 4;
        if (strncmp(format, ".txt", 4) != 0) continue;
        char filepath[MAX_PATH_LEN];
        sprintf(filepath, "%s/%s", source_dir_path, entry->d_name);
        FILE* to_reverse = fopen(filepath, "r");
        if (to_reverse == NULL) {
            perror("Source file opening error");
            return errno;
        }
        struct stat status;
        if (stat(target_dir_path, &status) != 0 || !S_ISDIR(status.st_mode)) {
            caught_result = mkdir(target_dir_path, S_IRWXU | S_IRWXG | S_IXOTH);
            if (caught_result) {
                perror("Target directory creating error");
                return errno;
            }
        }
        sprintf(filepath, "%s/flipped_%s", target_dir_path, entry->d_name);
        FILE* target_file = fopen(filepath, "w");
        if (target_file == NULL) {
            perror("Target file opening error");
            return errno;
        }

        flip(to_reverse, target_file);

        caught_result = fclose(to_reverse);
        if (caught_result == EOF) {
            perror("Source file closing error");
            return errno;
        }
        caught_result = fclose(target_file);
        if (caught_result == EOF) {
            perror("Target file closing error");
            return errno;
        }
        printf("Found: %s\n", entry->d_name);
    }

    caught_result = closedir(source_dir_handle);
    if (caught_result) {
        perror("Directory closing error");
        return errno;
    }

    return 0;
}
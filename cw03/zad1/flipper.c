#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

#define MAX_PATH_LEN 256
#define MAX_LINE_LEN 1024

void reverse_string(char string[], size_t size) {
    size_t left_ptr = 0;
    size_t right_ptr = size - 1;
    while (left_ptr < right_ptr) {
        char temp = string[left_ptr];
        string[left_ptr] = string[right_ptr];
        string[right_ptr] = temp;
        left_ptr++;
        right_ptr--;
    }
}

void flip(FILE* input, FILE* output) {
    char buffer[MAX_LINE_LEN] = {0};
    while (fgets(buffer, MAX_LINE_LEN, input)) {
        size_t line_len = strcspn(buffer, "\r\n");
        reverse_string(buffer, line_len);
        size_t written = fwrite(buffer, sizeof(char), line_len, output);
        if (written != line_len) {
            fprintf(stderr, "An error occurred trying to write a flipped line.\n");
        }
        if (buffer[line_len] == '\n')
            fwrite("\n", sizeof(char), 1, output);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Two arguments required: paths to the source and target directories.\n");
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
    }

    caught_result = closedir(source_dir_handle);
    if (caught_result) {
        perror("Directory closing error");
        return errno;
    }

    return 0;
}
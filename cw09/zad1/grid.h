#pragma once
#include <stdbool.h>

typedef struct grids_t {
    char* foreground;
    char* background;
} grids_t;

char* create_grid();
void destroy_grid(char *grid);
void draw_grid(char *grid);
void init_grid(char *grid);
bool is_alive(int row, int col, char *grid);
void update_grid(char* dst, char* src);
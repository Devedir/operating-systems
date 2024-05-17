#pragma once
#include <stdbool.h>

#define GRID_WIDTH 30
#define GRID_HEIGHT 30
#define GRID_SIZE GRID_WIDTH * GRID_HEIGHT

typedef struct grids_t {
    char* foreground;
    char* background;
} grids_t;

typedef struct updating_data_t {
    grids_t* grids;
    int start_idx;
    int size;
} updating_data_t;

char* create_grid();
void destroy_grid(char *grid);
void draw_grid(const char *grid);
void init_grid(char *grid);
bool is_alive(int idx, const char *grid);
void update_grid(updating_data_t* data);

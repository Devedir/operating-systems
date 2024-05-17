#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

char *create_grid() {
    return malloc(sizeof(char) * GRID_SIZE);
}

void destroy_grid(char *grid) {
    free(grid);
}

void draw_grid(const char *grid) {
    for (int i = 0; i < GRID_HEIGHT; ++i) {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < GRID_WIDTH; ++j) {
            if (grid[i * GRID_WIDTH + j]) {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

void init_grid(char *grid) {
    for (int i = 0; i < GRID_SIZE; ++i)
        grid[i] = rand() % 2 == 0;
}

bool is_alive(int idx, const char *grid) {
    int row = idx / GRID_WIDTH;
    int col = idx % GRID_WIDTH;
    int count = 0;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0)
                continue;
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= GRID_HEIGHT || c < 0 || c >= GRID_WIDTH)
                continue;
            if (grid[GRID_WIDTH * r + c])
                count++;
        }
    }

    if (grid[idx])
        return count == 2 || count == 3;
    else
        return count == 3;
}

void update_grid(updating_data_t* data) {
    grids_t* grids = data->grids;
    int idx = data->start_idx;
    for (int i = 0; i < data->size; i++) {
        grids->background[idx] = (char) is_alive(idx, grids->foreground);
        idx++;
    }
}

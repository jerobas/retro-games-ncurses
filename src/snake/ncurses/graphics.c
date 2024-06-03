#include "graphics.h"

void print_seed_ncurses(int new_seed_coordinates[2])
{
    mvaddch(new_seed_coordinates[1], new_seed_coordinates[0], 'X');
}

void render_move_snake_ncurses(bool grow, int removing_snake_segment[2], int adding_snake_segment[2])
{
    if (!grow)
        mvaddch(removing_snake_segment[1], removing_snake_segment[0], ' ');

    mvaddch(adding_snake_segment[1], adding_snake_segment[0], '0');
}
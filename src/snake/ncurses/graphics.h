#include <ncurses.h>

void print_seed_ncurses(int new_seed_coordinates[2]);
void render_move_snake_ncurses(bool grow, int removing_snake_segment[2], int adding_snake_segment[2]);
#include <ncurses.h>
#include "../../../arena.h"

extern int MAX_X,
    MAX_Y;

void print_seed_ncurses(int new_seed_coordinates[2]);
void render_move_snake_ncurses(bool grow, int removing_snake_segment[2], int adding_snake_segment[2]);
void update_score_ncurses(arena arena, int snake_length);
void score_string_ncurses();
int game_did_end_ncurses(int winner);
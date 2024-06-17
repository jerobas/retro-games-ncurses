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

void update_score_ncurses(arena arena, int snake_length)
{
    mvprintw(1, arena.right_x - 3, "%02d", snake_length - 2);
}

void score_string_ncurses()
{
    mvprintw(1, MAX_X - 10, "Score: 00");
}

int game_did_end_ncurses(int winner)
{
    endwin();
}
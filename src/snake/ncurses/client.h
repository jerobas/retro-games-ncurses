#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#include "../ncurses-handler.h"

extern int (*SNAKE)[MAX_SCORE + 2][2],
    *SNAKE_LENGTH,
    *MOVEMENT_DIRECTION,
    SEED[2],
    KEYS_OF_EACH_PLAYER[2][4];

extern arena ARENA;

int opposite_directions(int direction);

void initialize_game(int num_players);

void end_game();

bool move_snake(bool grow, int player);

int check_directions(int ch, int num_players);

void game_loop_client(int num_players, char *playerId);

void pause_game();

void cleanup(int result);
#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#include "../../ncurses-handler.h"

int opposite_directions_client(int direction);

void end_game_client();

void move_snake_client(bool grow, int player, int **new_snake, int snake_length);

int check_directions_client(int ch);

void game_loop_client(int num_players, char *playerId);

void pause_game_client();

void cleanup_client(int result);
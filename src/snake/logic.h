#include <ncurses.h>
#include <signal.h>
#include <poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "../ncurses-handler.h"
#include "../arena.h"

#define MAX_SCORE 100
#define TICKS_PER_SECOND 5

extern int MAX_X,
    MAX_Y,
    (*SNAKE)[MAX_SCORE + 2][2],
    *SNAKE_LENGTH,
    *MOVEMENT_DIRECTION,
    SEED[2],
    KEYS_OF_EACH_PLAYER[2][4];

extern arena ARENA;

extern void (*print_seed)(int[2]);
extern void (*render_move_snake)(bool, int[2], int[2]);
extern int (*tick_control)(int *tick_control_params);

int opposite_directions(int direction);

void update_score();

void score_string();

void initialize_game(int num_players);

bool collision_check(int player);

bool check_all_players(int num_players);

void end_game();

bool is_there_seed(int check_location[2]);

bool is_there_seed_in_snake(int player);

bool move_snake(bool grow, int player);

void new_seed(int num_players);

int check_directions(int ch, int num_players);

void game_loop(int num_players);

void pause_game();

void cleanup(int result);

int tick_control_client_side(int *start);

int tick_control_server_side(int *params);
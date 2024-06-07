#ifndef LOGIC
#define LOGIC
#include <ncurses.h>
#include <signal.h>
#include <poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "../ncurses-handler.h"
#include "../arena.h"

#define MAX_SCORE 100
#define TICKS_PER_SECOND 13

extern int (*SNAKE)[MAX_SCORE + 2][2],
    *SNAKE_LENGTH,
    *MOVEMENT_DIRECTION,
    SEED[2],
    KEYS_OF_EACH_PLAYER[2][4];

extern arena ARENA;

extern void (*print_seed)(int[2]);
extern void (*render_move_snake)(bool, int[2], int[2]);
extern int (*tick_control)(int *tick_control_params);
extern int (*game_did_end)(int winner);
extern int (*char_read)();
extern void (*set_char_read_timeout)();
extern int (*screen_update)();
extern void (*update_score)();
extern void (*score_string)();

int opposite_directions(int direction);

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
#endif
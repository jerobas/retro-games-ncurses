#include "logic.h"
#include "../utils/client.h"

int (*SNAKE)[MAX_SCORE + 2][2],
    *SNAKE_LENGTH,
    *MOVEMENT_DIRECTION,
    SEED[2];

int KEYS_OF_EACH_PLAYER[2][4] = {{KEY_UP, KEY_LEFT, KEY_DOWN, KEY_RIGHT}, {'w', 'a', 's', 'd'}};

arena ARENA;

struct player_move
{
    int player_id;
    int player_command;
};

typedef struct player_move player_move;

void (*print_seed)(int[2]);
void (*render_move_snake)(bool, int[2], int[2]);
void (*update_score)();
void (*score_string)();
int (*char_read)();
void (*set_char_read_timeout)();
int (*screen_update)();

int (*game_did_end)(int);
int (*tick_control)(int *tick_control_params);

// "almost" ready
int opposite_directions(int direction)
{
    switch (direction)
    {
    case KEY_UP:
        return KEY_DOWN;
    case KEY_DOWN:
        return KEY_UP;
    case KEY_LEFT:
        return KEY_RIGHT;
    case KEY_RIGHT:
        return KEY_LEFT;
    case 'w':
        return 's';
    case 's':
        return 'w';
    case 'a':
        return 'd';
    case 'd':
        return 'a';
    default:
        return -1;
    }
}

void initialize_game(int num_players)
{
    SNAKE_LENGTH = (int *)malloc(num_players * sizeof(int));
    SNAKE = (int(*)[MAX_SCORE + 2][2]) malloc(num_players * sizeof(int[MAX_SCORE + 2][2]));
    MOVEMENT_DIRECTION = (int *)malloc(num_players * sizeof(int));

    if (SNAKE_LENGTH == NULL || SNAKE == NULL || MOVEMENT_DIRECTION == NULL)
        exit(EXIT_FAILURE);

    for (int i = 0; i < num_players; i++)
    {
        SNAKE_LENGTH[i] = 2;
        for (int j = 0; j < SNAKE_LENGTH[i]; j++)
        {
            SNAKE[i][j][0] = 3 - j;
            SNAKE[i][j][1] = 2 + (2 * i);
        }
    }
}

/*
    ALMOST ALL OF THESE ARE READY
*/

bool collision_check(int player)
{
    // Check self-collision
    for (int i = 1; i < SNAKE_LENGTH[player]; i++)
    {
        if (SNAKE[player][0][0] == SNAKE[player][i][0] && SNAKE[player][0][1] == SNAKE[player][i][1])
            return true;
    }

    // Check collision with arena boundaries
    if (SNAKE[player][0][0] <= ARENA.left_x || SNAKE[player][0][0] >= ARENA.right_x ||
        SNAKE[player][0][1] <= ARENA.top_y || SNAKE[player][0][1] >= ARENA.bottom_y)
        return true;

    return false;
}

bool check_all_players(int num_players)
{
    for (int player = 0; player < num_players; player++)
    {
        if (collision_check(player))
        {
            return true;
        }

        for (int other_players = player + 1; other_players < num_players; other_players++)
        {
            for (int i = 0; i < SNAKE_LENGTH[other_players]; i++)
            {
                if (SNAKE[player][0][0] == SNAKE[other_players][i][0] && SNAKE[player][0][1] == SNAKE[other_players][i][1])
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool is_there_seed(int check_location[2])
{
    bool x_check = check_location[0] == SEED[0];
    bool y_check = check_location[1] == SEED[1];

    return x_check && y_check;
}

bool is_there_seed_in_snake(int player)
{
    for (int i = 0; i < SNAKE_LENGTH[player]; i++)
    {
        if (is_there_seed(SNAKE[player][i]))
            return true;
    }
    return false;
}

// should separate the return of this function from it
bool move_snake(bool grow, int player)
{
    // exclusive for segment-based printing methods
    int old_last_snake_segment[2] = {SNAKE[player][SNAKE_LENGTH[player] - 1][0], SNAKE[player][SNAKE_LENGTH[player] - 1][1]};

    if (grow)
    {
        SNAKE_LENGTH[player]++;
        update_score();
    }

    for (int i = SNAKE_LENGTH[player] - 1; i > 0; i--)
    {
        SNAKE[player][i][0] = SNAKE[player][i - 1][0];
        SNAKE[player][i][1] = SNAKE[player][i - 1][1];
    }

    // I CAN DO THIS BETTER
    SNAKE[player][0][0] += MOVEMENT_DIRECTION[player] == KEYS_OF_EACH_PLAYER[player][1] ? -1 : MOVEMENT_DIRECTION[player] == KEYS_OF_EACH_PLAYER[player][3] ? 1
                                                                                                                                                            : 0;
    SNAKE[player][0][1] += MOVEMENT_DIRECTION[player] == KEYS_OF_EACH_PLAYER[player][0] ? -1 : MOVEMENT_DIRECTION[player] == KEYS_OF_EACH_PLAYER[player][2] ? 1
                                                                                                                                                            : 0;

    // exclusive for segment-based printing methods
    int new_first_snake_segment[2] = {SNAKE[player][0][0], SNAKE[player][0][1]};

    render_move_snake(grow, old_last_snake_segment, new_first_snake_segment);

    return is_there_seed(SNAKE[player][0]);
}

void new_seed(int num_players)
{
    int there_is_seed_in_snake = false;

    do
    {
        SEED[0] = rand() % (ARENA.right_x - 2) + 1;
        SEED[1] = rand() % (ARENA.bottom_y - 2) + 1;

        for (int i = 0; i < num_players; i++)
        {
            there_is_seed_in_snake = is_there_seed_in_snake(i) || there_is_seed_in_snake;
        }
    } while (there_is_seed_in_snake);

    print_seed(SEED);
}

int check_directions(int ch, int num_players)
{
    for (int i = 0; i < num_players; i++)
    {
        bool is_player_key = false;
        for (int j = 0; j < 4; j++)
        {
            if (ch == KEYS_OF_EACH_PLAYER[i][j])
            {
                is_player_key = true;
                break;
            }
        }

        if (is_player_key)
        {
            return (opposite_directions(ch) == MOVEMENT_DIRECTION[i]) ? -1 : i;
        }
    }
}

/*
    GREAT PROBLEMS AHEAD
*/

int tick_control_client_side(int *start)
{
    int end, diff_in_ms;

    end = (int)(clock());
    diff_in_ms = (int)(((double)(end - *start) / CLOCKS_PER_SEC) * 1000);

    if (diff_in_ms < (1000 / TICKS_PER_SECOND))
        return true;

    *start = (int)(clock());
    return false;
}

void game_loop_cs_player_control(int num_players, int *possible_newch)
{
    int ch = 0;
    int tick_control_params;

    tick_control_params = (int)clock();
    do
    {
        ch = char_read();

        if (ch == 'q')
            end_game();

        if (ch == 'p')
            pause_game();

        int check = check_directions(ch, num_players);
        if (check != -1)
            possible_newch[check] = ch;

    } while (tick_control(&tick_control_params));
}

// int *game_loop_ss_player_control_server(int num_players)
// {
//     int possible_newch[num_players];
//     int tick_control_params;

//     tick_control_params = (int)clock();

//     // broadcast start of tick
//     do
//     {
//         player_move player_move = check_message(num_players);
//         if (player_move.player_id > 0 && player_move.player_id < num_players)
//             possible_newch[player_move.player_id] = player_move.player_command;
//     } while (tick_control(&tick_control_params));
//     // broadcast end of tick

//     return possible_newch;
// }

void game_loop(int num_players)
{
    srand(time(0));
    new_seed(num_players);

    bool generate_new_seed = false;
    bool grow[num_players];
    int checked_collision = false;

    int possible_newch[num_players];

    for (int i = 0; i < num_players; i++)
    {
        MOVEMENT_DIRECTION[i] = KEYS_OF_EACH_PLAYER[i][3];
        possible_newch[i] = KEYS_OF_EACH_PLAYER[i][3];
        grow[i] = false;
    }

    do
    {
        generate_new_seed = false;
        if (check_all_players(num_players))
            end_game();

        game_loop_cs_player_control(num_players, possible_newch);

        for (int i = 0; i < num_players; i++)
        {
            MOVEMENT_DIRECTION[i] = possible_newch[i];
            grow[i] = move_snake(grow[i], i);
            generate_new_seed = grow[i] || generate_new_seed;
        }
        if (generate_new_seed)
            new_seed(num_players);

        screen_update();
    } while (true);
}

/*
    PRETTY NECESSARY
    TIMEOUT AND ENDWIN SHOULD BE REMOVED, AS THEY ARE NCURSES EXCLUSIVE
*/

void end_game()
{
    set_char_read_timeout(-1);
    screen_update();
    int c = char_read();
    cleanup(0);
}

void pause_game()
{
    set_char_read_timeout(-1);
    screen_update();
    int c = char_read();
    set_char_read_timeout(0);
    screen_update();
}

void cleanup(int result)
{
    free(SNAKE);
    free(SNAKE_LENGTH);
    free(MOVEMENT_DIRECTION);
    game_did_end(0);
    exit(0);
}

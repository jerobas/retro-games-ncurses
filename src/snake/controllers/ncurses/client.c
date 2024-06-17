#include "../client.h"
#include "client.h"
#include "../logic.h"
#include "graphics.h"

int KEYS_OF_EACH_PLAYER_CLIENT[4] = {KEY_UP, KEY_LEFT, KEY_DOWN, KEY_RIGHT};

void move_snake_client(bool grow, int player, int **new_snake, int snake_length)
{
    // exclusive for segment-based printing methods
    int old_last_snake_segment[2] = {SNAKE[snake_length - 1][0], SNAKE[snake_length - 1][1]};

    if (grow)
    {
        // update_score();
        print_seed_ncurses();
    }

    // exclusive for segment-based printing methods
    int new_first_snake_segment[2] = {new_snake[0][0], new_snake[0][1]};

    render_move_snake(grow, old_last_snake_segment, new_first_snake_segment);
}

void game_loop_ss_player_control_client(char *playerId)
{
    int ch;
    int possible_newch;

    do
    {
        ch = char_read();

        int check = check_directions(ch, 1);
        if (check != -1)
            possible_newch = ch;

    } while (checkConnection() != 0);

    switch (possible_newch)
    {
    case KEY_UP:
        sendDirection("UP", playerId);
    case KEY_DOWN:
        sendDirection("DOWN", playerId);
    case KEY_LEFT:
        sendDirection("LEFT", playerId);
    case KEY_RIGHT:
        sendDirection("RIGHT", playerId);
    }
}

void game_loop_client(int num_players, char *playerId)
{
    bool grow[num_players];
    for (int i = 0; i < num_players; i++)
    {
        grow[i] = false;
    }

    do
    {
        game_loop_ss_player_control_client(playerId);

        for (int i = 0; i < num_players; i++)
        {
            move_snake_client(grow[i], i);
        }
    } while (true);

    end_game_client();
}

void end_game_client()
{
    set_char_read_timeout(-1);
    screen_update();
    int c = char_read();
    cleanup_client(0);
}

void cleanup_client(int result)
{
    free(MOVEMENT_DIRECTION);
    game_did_end(0);
    exit(0);
}
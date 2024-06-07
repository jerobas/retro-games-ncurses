#include "../client.h"
#include "client.h"
#include "../logic.h"

int KEYS_OF_EACH_PLAYER[4] = {KEY_UP, KEY_LEFT, KEY_DOWN, KEY_RIGHT};

int opposite_directions_client(int direction)
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

int check_directions_client(int ch)
{
    bool is_player_key = false;
    for (int j = 0; j < 4; j++)
    {
        if (ch == KEYS_OF_EACH_PLAYER[j])
        {
            return (opposite_directions(ch) == MOVEMENT_DIRECTION[0]) ? -1 : 0;
            break;
        }
    }
}

void move_snake_client(bool grow, int player, int **new_snake, int snake_length)
{
    // exclusive for segment-based printing methods
    int old_last_snake_segment[2] = {SNAKE[snake_length - 1][0], SNAKE[snake_length - 1][1]};

    if (grow)
    {
        // update_score();
        render_new_seed();
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

        int check = check_directions_client(ch, 1);
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
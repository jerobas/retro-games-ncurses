#include "../client.h"
#include "client.h"

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

int check_directions(int ch)
{
    bool is_player_key = false;
    for (int j = 0; j < 4; j++)
    {
        if (ch == KEYS_OF_EACH_PLAYER[j])
        {
            return (opposite_directions(ch) == MOVEMENT_DIRECTION[i]) ? -1 : i;
            break;
        }
    }
}

void move_snake_client(bool grow, int player, int *new_snake, int snake_length)
{
    // exclusive for segment-based printing methods
    int old_last_snake_segment[2] = {SNAKE[snake_length - 1][0], SNAKE[snake_length - 1][1]};

    if (grow)
    {
        update_score();
        render_new_seed();
    }

    // exclusive for segment-based printing methods
    int new_first_snake_segment[2] = {new_snake[0][0], new_snake[0][1]};

    render_move_snake(grow, old_last_snake_segment, new_first_snake_segment);
}

void game_loop_ss_player_control_client(bool (*does_tick_ended)(), void(*ch_broadcast))
{
    int ch;
    int possible_newch;

    do
    {
        ch = char_read();

        int check = check_directions(ch, 1);
        if (check != -1)
            possible_newch = ch;

    } while (does_tick_ended());

    ch_broadcast(possible_newch);
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
        game_loop_ss_player_control_client(num_players);

        do
        {
            int server_response = awaiting_server_response();
        } while (no_server_response());

        for (int i = 0; i < num_players; i++)
        {
            move_snake(grow[i], i);
        }
    } while (check_win_client());

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
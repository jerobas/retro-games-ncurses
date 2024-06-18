#include "../client.h"
#include "client.h"
#include "../logic.h"
#include "graphics.h"

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
        sendDirection(0, playerId);
    case KEY_DOWN:
        sendDirection(1, playerId);
    case KEY_LEFT:
        sendDirection(2, playerId);
    case KEY_RIGHT:
        sendDirection(3, playerId);
    }
}

void game_loop_client(char *playerId)
{
    await_initial_state();

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
    game_did_end(0);
    exit(0);
}
#include "../logic.h"
#include "../client.h"
#include "client.h"
#include "graphics.h"

int MAX_X,
    MAX_Y;

int main()
{
    int num_players = 1;
    int is_single_player = 1;
    char *playerId = 0;

    print_seed = &print_seed_ncurses;
    render_move_snake = &render_move_snake_ncurses;
    update_score = &update_score_ncurses;
    score_string = &score_string_ncurses;
    screen_update = &refresh;
    game_did_end = &game_did_end_ncurses;
    char_read = &getch;
    set_char_read_timeout = &timeout;

    tick_control = &tick_control_client_side;

    playerId = handle_connection();

    initialize_ncurses_screen(&MAX_X, &MAX_Y);
    timeout(0);

    ARENA = arena_create(0, MAX_X - 1, 0, MAX_Y - 1);
    arena_node score = arena_node_create(MAX_X - 10, 1, update_score, score_string);
    ARENA.nodes = &score;
    arena_render_nodes(ARENA);
    rectangle(ARENA.left_x, ARENA.right_x, ARENA.top_y, ARENA.bottom_y);

    /*
        THINGS START CHANGING HERE
    */

    if (is_single_player)
    {
        initialize_game(num_players);
        refresh();
        game_loop(num_players);
    }
    else
    {
        initialize_game_client(num_players);
        refresh();
        game_loop_client(num_players, playerId);
    }

    return 0;
}
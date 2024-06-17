#include "../../services/logic.h"
#include "../../utils/client.h"
#include "client.h"
#include "graphics.h"

int MAX_X,
    MAX_Y;

int main()
{
    int num_players = 1;
    int is_single_player = 0;
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

    /*
        THINGS START CHANGING HERE
    */

    if (is_single_player)
    {
        initialize_ncurses_screen(&MAX_X, &MAX_Y);
        timeout(0);

        ARENA = arena_create(0, MAX_X - 1, 0, MAX_Y - 1);
        arena_node score = arena_node_create(MAX_X - 10, 1, update_score, score_string);
        ARENA.nodes = &score;
        arena_render_nodes(ARENA);
        rectangle(ARENA.left_x, ARENA.right_x, ARENA.top_y, ARENA.bottom_y);
        initialize_game(num_players);
        refresh();
        game_loop(num_players);
    }
    else
    {
        playerId = handle_connection();

        initialize_ncurses_screen(&MAX_X, &MAX_Y);
        timeout(0);

        int i = 0;
        while (1)
        {
            int server_response = checkConnection();
            if (server_response < 0)
            {
                // error
            }

            if (server_response == 0)
                break;
            if (server_response == 1)
            {
                mvprintw(0, 0, "waiting for connection");
                for (int j = 0; j < 3; j++)
                {
                    mvprintw(0, 22 + j, j <= i ? "." : " ");
                }
                i = (i + 1) % 3;
                refresh();
            }
        }

        ARENA = arena_create(0, MAX_X - 1, 0, MAX_Y - 1);
        arena_node score = arena_node_create(MAX_X - 10, 1, update_score, score_string);
        ARENA.nodes = &score;
        arena_render_nodes(ARENA);
        rectangle(ARENA.left_x, ARENA.right_x, ARENA.top_y, ARENA.bottom_y);
        refresh();
        game_loop_client(num_players, playerId);
        endwin();
    }

    return 0;
}
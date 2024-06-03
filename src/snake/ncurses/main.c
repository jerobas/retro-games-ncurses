#include "../logic.h"
#include "graphics.h"

int main()
{
    int num_players, is_single_player;

    printf("Quantos players?: ");
    scanf("%d", &num_players);

    printf("\nSingle player?: ");
    scanf("%d", &is_single_player);

    print_seed = &print_seed_ncurses;
    render_move_snake = &render_move_snake_ncurses;
    tick_control = is_single_player == 1 ? &tick_control_client_side : &tick_control_server_side;

    initialize_ncurses_screen(&MAX_X, &MAX_Y);
    initialize_game(num_players);

    game_loop(num_players);

    endwin();
    return 0;
}
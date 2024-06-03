#include "logic.h"
#include "ncurses/graphics.h"

int main()
{
    int num_players;

    print_seed = &print_seed_ncurses;
    render_move_snake = &render_move_snake_ncurses;

    printf("Quantos players?: ");
    scanf("%d", &num_players);

    initialize_ncurses_screen(&MAX_X, &MAX_Y);
    initialize_game(num_players);

    game_loop(num_players);

    endwin();
    return 0;
}
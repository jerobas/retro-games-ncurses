#include "main.h"

int MAX_X,
    MAX_Y,
    BOARD_HEIGHT,
    BOARD_WIDTH,
    BOARD_BEGIN,
    BOARD_END,
    BOARD_THICKNESS = 1,
    DESIRED_BOARD_WIDTH = 10;

arena ARENA;
tetris_piece CURRENT_PIECE, NEXT_PIECE;

int TRANSFORMED_PIECE[4][2];

bool **TETRIS_BOARD;

void update_score()
{
    mvprintw(1, BOARD_WIDTH - 3, "%02d", 2);
}
void score_string()
{
    mvprintw(1, BOARD_WIDTH - 10, "Score: 00");
}
/* division */

void update_piece(bool first_time)
{
    if (!first_time)
    {
        for (int i = 0; i < 4; i++)
        {
            mvaddch(CURRENT_PIECE.shape[i][1], CURRENT_PIECE.shape[i][0], ' ');
        }

        for (int i = 0; i < 4; i++)
        {
            mvaddch(TRANSFORMED_PIECE[i][1], TRANSFORMED_PIECE[i][0], '0');
        }

        for (int i = 0; i < 4; i++)
        {
            CURRENT_PIECE.shape[i][0] = TRANSFORMED_PIECE[i][0];
            CURRENT_PIECE.shape[i][1] = TRANSFORMED_PIECE[i][1];
        }
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {
            mvaddch(CURRENT_PIECE.shape[i][1], CURRENT_PIECE.shape[i][0], '0');
        }
    }

    refresh();
}
bool wall_collision_check()
{
    for (int i = 0; i < 4; i++)
        if (TRANSFORMED_PIECE[i][0] < BOARD_BEGIN + BOARD_THICKNESS || TRANSFORMED_PIECE[i][0] >= BOARD_BEGIN + BOARD_WIDTH + BOARD_THICKNESS)
            return true;

    for (int i = 0; i < 4; i++)
        if (TETRIS_BOARD[TRANSFORMED_PIECE[i][0] - BOARD_BEGIN - BOARD_THICKNESS][TRANSFORMED_PIECE[i][1]])
            return true;

    return false;
}
bool floor_collision_check()
{
    for (int i = 0; i < 4; i++)
        if (TETRIS_BOARD[CURRENT_PIECE.shape[i][0] - BOARD_BEGIN - BOARD_THICKNESS][CURRENT_PIECE.shape[i][1] + 1])
            return true;
}
/* division */

int operation(int ch)
{
    int op_x = 0, op_y = 0;

    switch (ch)
    {
    case KEY_LEFT:
        op_x = -1;
        break;
    case KEY_RIGHT:
        op_x = +1;
        break;
    case KEY_DOWN:
        op_y = +1;
        break;
    }

    for (int i = 0; i < 4; i++)
    {
        TRANSFORMED_PIECE[i][0] = CURRENT_PIECE.shape[i][0] + op_x;
        TRANSFORMED_PIECE[i][1] = CURRENT_PIECE.shape[i][1] + op_y;
    }

    if (!wall_collision_check())
        update_piece(false);
    else
        return false;

    return true;
}
void rotate_with_check()
{
    CURRENT_PIECE.rotate(&CURRENT_PIECE, TRANSFORMED_PIECE);

    if (!wall_collision_check())
        update_piece(false);
}
/* division */

void new_queue()
{
    CURRENT_PIECE = NEXT_PIECE;
    NEXT_PIECE = init_tetris_piece(rand() % 7, floor(BOARD_WIDTH / 2) + BOARD_BEGIN, 2);
}
void first_queue()
{
    NEXT_PIECE = init_tetris_piece(rand() % 7, floor(BOARD_WIDTH / 2) + BOARD_BEGIN, 2);
    CURRENT_PIECE = init_tetris_piece(rand() % 7, floor(BOARD_WIDTH / 2) + BOARD_BEGIN, 2);

    update_piece(true);
}
/* division */

void game_handler(int ch)
{
    if (ch == 'r' || ch == 'R')
        rotate_with_check();
    if (ch == KEY_LEFT || ch == KEY_RIGHT) // || ch == KEY_DOWN)
        operation(ch);
}
void control_handler(int ch, int *game_state)
{
    switch (ch)
    {
    case 'q':
        *game_state = 2;
        break;

    case 'p':
        *game_state = *game_state == 0 ? 1 : 0;
        mvprintw(MAX_Y / 2, MAX_X / 2 - 2, *game_state == 1 ? "PAUSE" : "      ");
        break;

    default:
        break;
    }
}
int time_handler(int *start)
{
    int end, diff_in_ms;
    end = (int)(clock());
    diff_in_ms = (int)(((double)(end - *start) / CLOCKS_PER_SEC) * 1000);

    int return_value = diff_in_ms > (1000 / TICKS_PER_SECOND);

    if (return_value)
        *start = (int)(clock());
    return return_value;
}
/* division */

void initialize_game()
{
    BOARD_HEIGHT = MAX_Y;
    BOARD_WIDTH = MAX_X > DESIRED_BOARD_WIDTH + 2 * BOARD_THICKNESS ? DESIRED_BOARD_WIDTH : MAX_X - BOARD_THICKNESS * 2;
    BOARD_BEGIN = (MAX_X - BOARD_WIDTH - 1) / 2;
    BOARD_END = (MAX_X - BOARD_WIDTH - 1) / 2;

    TETRIS_BOARD = (bool **)malloc(BOARD_WIDTH * sizeof(bool *));

    for (int i = 0; i < BOARD_WIDTH; i++)
    {
        TETRIS_BOARD[i] = (bool *)malloc(BOARD_HEIGHT * sizeof(bool));
        for (int j = 0; j < BOARD_HEIGHT; j++)
            TETRIS_BOARD[i][j] = (j == BOARD_HEIGHT - 1 ? 1 : 0);
    }

    ARENA = arena_create(BOARD_BEGIN, BOARD_BEGIN + BOARD_WIDTH + BOARD_THICKNESS, 0, BOARD_HEIGHT - 1);
    arena_node score = arena_node_create(BOARD_WIDTH - 10, 1, &update_score, &score_string);
    ARENA.nodes = &score;
    arena_render_nodes(ARENA);
    rectangle(ARENA.left_x, ARENA.right_x, ARENA.top_y, ARENA.bottom_y);

    srand(time(0));
    timeout(0);
    first_queue();
}
int game_loop()
{
    int game_state = 0, // 0: running, 1: paused, 2: manual quit
        start = (int)clock();

    do
    {
        int ch = getch();

        control_handler(ch, &game_state);
        game_handler(ch);

        if (game_state == 0)
        {
            if (time_handler(&start))
                if (floor_collision_check())
                {
                    for (int i = 0; i < 4; i++)
                        TETRIS_BOARD[CURRENT_PIECE.shape[i][0] - BOARD_BEGIN - BOARD_THICKNESS][CURRENT_PIECE.shape[i][1]] = 1;

                    int first_full = 0;
                    for (int i = 0; i < 4; i++)
                    {
                        int full = 1;
                        for (int j = 0; j < BOARD_WIDTH; j++)
                        {
                            if (TETRIS_BOARD[j][CURRENT_PIECE.shape[i][1]] == 0)
                            {
                                full = 0;
                                break;
                            }
                        }

                        if (full && CURRENT_PIECE.shape[i][1] > first_full)
                        {
                            first_full = CURRENT_PIECE.shape[i][1];
                        }
                    }

                    refresh();
                    int full_count = 1;
                    // int empty_count = 0;
                    for (int i = first_full; i > 0; i--)
                    {
                        int full_found = 1;
                        do
                        {
                            for (int j = 0; (j < BOARD_WIDTH) && full_found; j++)
                                if (i - full_count >= 0)
                                {
                                    if (TETRIS_BOARD[j][i - full_count] == 0)
                                        full_found = 0;
                                }
                                else
                                    full_found = 0;

                            if (full_found)
                                full_count++;

                        } while (full_found);

                        for (int j = 0; j < BOARD_WIDTH; j++)
                        {
                            if (i - full_count >= 0)
                                TETRIS_BOARD[j][i] = TETRIS_BOARD[j][i - full_count];
                            else
                                TETRIS_BOARD[j][i] = 0;

                            mvprintw(i, j + BOARD_BEGIN + BOARD_THICKNESS, TETRIS_BOARD[j][i] ? "0" : " ");
                        }
                    }

                    new_queue();
                    update_piece(true);
                }
                else
                    operation(KEY_DOWN);
        }

        refresh();

    } while (game_state != 2);

    return 0;
}

/* division */
void free_memory()
{
    for (int i = 0; i < BOARD_WIDTH; i++)
        free(TETRIS_BOARD[i]);

    free(TETRIS_BOARD);
}
int main()
{
    initialize_ncurses_screen(&MAX_X, &MAX_Y);
    initialize_game();

    game_loop();

    endwin();
    free_memory();

    return 0;
}
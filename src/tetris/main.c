#include <ncurses.h>
#include <signal.h>
#include <poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "../ncurses-handler.h"
#include "../arena.h"
#include "main.h"

// #define MAX_SCORE 100
#define TICKS_PER_SECOND 4

int MAX_X,
    MAX_Y;

int BOARD_HEIGHT;
int BOARD_WIDTH;
int DESIRED_BOARD_WIDTH = 20;
int BOARD_BEGIN;
int BOARD_END;
int BOARD_THICKNESS = 1;

tetris_piece CURRENT_PIECE;
tetris_piece NEXT_PIECE;

int TRANSFORMED_PIECE[4][2];

bool **TETRIS_BOARD;

arena ARENA;

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
}

void operation(int ch)
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
}

void update_score()
{
    mvprintw(1, BOARD_WIDTH - 3, "%02d", 2);
}

void score_string()
{
    mvprintw(1, BOARD_WIDTH - 10, "Score: 00");
}

void initialize_game()
{
    for (int i = 0; i < BOARD_WIDTH; i++)
        for (int j = 0; j < BOARD_HEIGHT; j++)
            TETRIS_BOARD[i][j] = (j == BOARD_HEIGHT - 1 ? 1 : 0);

    ARENA = arena_create(BOARD_BEGIN, BOARD_BEGIN + BOARD_WIDTH + BOARD_THICKNESS, 0, BOARD_HEIGHT - 1);

    arena_node score = arena_node_create(BOARD_WIDTH - 10, 1, &update_score, &score_string);
    ARENA.nodes = &score;

    arena_render_nodes(ARENA);
    rectangle(ARENA.left_x, ARENA.right_x, ARENA.top_y, ARENA.bottom_y);

    refresh();
}

void rotate_with_check()
{
    CURRENT_PIECE.rotate(&CURRENT_PIECE, TRANSFORMED_PIECE);

    for (int i = 0; i < 4; i++)
    {
        if (TRANSFORMED_PIECE[i][0] < BOARD_BEGIN + BOARD_THICKNESS || TRANSFORMED_PIECE[i][0] >= BOARD_BEGIN + BOARD_WIDTH + BOARD_THICKNESS)
        {
            return;
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (TETRIS_BOARD[TRANSFORMED_PIECE[i][0] - BOARD_BEGIN - BOARD_THICKNESS][TRANSFORMED_PIECE[i][1]])
            return;
    }

    update_piece(false);
}

bool wall_collision_check(int ch)
{
    operation(ch);

    for (int i = 0; i < 4; i++)
    {
        if (TRANSFORMED_PIECE[i][0] < BOARD_BEGIN + BOARD_THICKNESS || TRANSFORMED_PIECE[i][0] >= BOARD_BEGIN + BOARD_WIDTH + BOARD_THICKNESS)
        {
            return true;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        if (TETRIS_BOARD[TRANSFORMED_PIECE[i][0] - BOARD_BEGIN - BOARD_THICKNESS][TRANSFORMED_PIECE[i][1]])
            return true;
    }

    return false;
}

bool floor_collision_check()
{
    for (int i = 0; i < 4; i++)
    {
        if (TETRIS_BOARD[CURRENT_PIECE.shape[i][0] - BOARD_BEGIN - BOARD_THICKNESS][CURRENT_PIECE.shape[i][1] + 1])
            return true;
    }

    return false;
}

bool move_piece(int ch)
{
    if (wall_collision_check(ch))
        return false;

    update_piece(false);
    return true;
}

void new_queue()
{
    CURRENT_PIECE = NEXT_PIECE;
    NEXT_PIECE = init_tetris_piece(rand() % 7, floor(BOARD_WIDTH / 2) + BOARD_BEGIN, 2);
}

void first_queue()
{
    NEXT_PIECE = init_tetris_piece(rand() % 7, floor(BOARD_WIDTH / 2) + BOARD_BEGIN, 2);
    CURRENT_PIECE = init_tetris_piece(rand() % 7, floor(BOARD_WIDTH / 2) + BOARD_BEGIN, 2);
}

int game_loop()
{
    int ch = 0;

    srand(time(0));
    first_queue();
    update_piece(true);
    timeout(0);

    bool pause = false;
    int start = (int)clock();
    int end, diff_in_ms;

    do
    {
        ch = getch();
        if (ch == 'q' || ch == 'Q')
            return 0;

        if ((ch == 'r' || ch == 'R') && !pause)
            rotate_with_check();

        if (ch == 'p' || ch == 'P')
        {
            pause = !pause;
            if (pause)
            {
                mvprintw(MAX_Y / 2, MAX_X / 2 - 2, "PAUSE");
            }
            else
            {
                mvprintw(MAX_Y / 2, MAX_X / 2 - 2, "      ");
            }
        }

        if (!pause && (ch == KEY_LEFT || ch == KEY_RIGHT || ch == KEY_DOWN))
        {
            move_piece(ch);

            end = (int)(clock());
            diff_in_ms = (int)(((double)(end - start) / CLOCKS_PER_SEC) * 1000);

            if (diff_in_ms > (1000 / TICKS_PER_SECOND))
            {
                start = (int)clock();
                if (floor_collision_check())
                {
                    for (int i = 0; i < 4; i++)
                    {
                        TETRIS_BOARD[CURRENT_PIECE.shape[i][0] - BOARD_BEGIN - BOARD_THICKNESS][CURRENT_PIECE.shape[i][1]] = 1;
                    }
                    new_queue();
                    update_piece(true);
                }
                else if (!pause)
                {
                    operation(KEY_DOWN);
                    update_piece(false);
                }
            }
        }

        refresh();

    } while (true);
}

int main()
{
    initialize_ncurses_screen(&MAX_X, &MAX_Y);

    BOARD_HEIGHT = MAX_Y;
    BOARD_WIDTH = MAX_X > DESIRED_BOARD_WIDTH + 2 * BOARD_THICKNESS ? DESIRED_BOARD_WIDTH : MAX_X - BOARD_THICKNESS * 2;
    BOARD_BEGIN = (MAX_X - BOARD_WIDTH - 1) / 2;
    BOARD_END = (MAX_X - BOARD_WIDTH - 1) / 2;

    TETRIS_BOARD = (bool **)malloc(BOARD_WIDTH * sizeof(bool *));
    for (int i = 0; i < BOARD_WIDTH; i++)
        TETRIS_BOARD[i] = (bool *)malloc(BOARD_HEIGHT * sizeof(bool));

    initialize_game();

    game_loop();
    endwin();

    for (int i = 0; i < BOARD_WIDTH; i++)
        free(TETRIS_BOARD[i]);

    free(TETRIS_BOARD);

    return 0;
}
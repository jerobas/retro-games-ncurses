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

int CURRENT_PIECE[4][2];
int NEXT_PIECE[4][2];

bool **TETRIS_BOARD;

arena ARENA;

void update_piece(bool first_time)
{
    if (!first_time)
        for (int i = 0; i < 4; i++)
        {
            mvaddch(CURRENT_PIECE[i][1], CURRENT_PIECE[i][0], ' ');
        }
    for (int i = 0; i < 4; i++)
    {
        mvaddch(NEXT_PIECE[i][1], NEXT_PIECE[i][0], '0');
    }

    for (int i = 0; i < 4; i++)
    {
        CURRENT_PIECE[i][0] = NEXT_PIECE[i][0];
        CURRENT_PIECE[i][1] = NEXT_PIECE[i][1];
    }
}

void operation(int ch)
{
    int op_x, op_y = 0;

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
        NEXT_PIECE[i][0] = CURRENT_PIECE[i][0] + op_x;
        NEXT_PIECE[i][1] = CURRENT_PIECE[i][1] + op_y;
    }
}

void update_score()
{
    mvprintw(1, MAX_X - 3, "%02d", 2);
}

void score_string()
{
    mvprintw(1, MAX_X - 10, "Score: 00");
}

void initialize_game()
{
    for (int i = 0; i < BOARD_WIDTH; i++)
        for (int j = 0; j < BOARD_HEIGHT; j++)
            TETRIS_BOARD[i][j] = (j == BOARD_HEIGHT - 1 ? 1 : 0);

    ARENA = arena_create(0, MAX_X - 1, 0, MAX_Y - 1);

    arena_node score = arena_node_create(MAX_X - 10, 1, &update_score, &score_string);
    ARENA.nodes = &score;

    arena_render_nodes(ARENA);
    rectangle(ARENA.left_x, ARENA.right_x, ARENA.top_y, ARENA.bottom_y);

    refresh();
}

bool wall_collision_check(int ch)
{
    operation(ch);

    for (int i = 0; i < 4; i++)
    {
        if (NEXT_PIECE[i][0] < 0 || NEXT_PIECE[i][0] >= BOARD_WIDTH)
            return true;
    }

    return false;
}

bool floor_collision_check()
{
    for (int i = 0; i < 4; i++)
    {
        if (TETRIS_BOARD[CURRENT_PIECE[i][0]][CURRENT_PIECE[i][1] + 1])
            return true;
    }

    return false;
}

bool move_piece(int ch)
{
    if (ch == KEY_LEFT || ch == KEY_RIGHT)
    {
        if (wall_collision_check(ch))
            return false;
    }
    else
    {
        return false;
    }

    update_piece(false);
    return true;
}

void new_queue()
{
    new_piece(NEXT_PIECE, rand() % 7, floor(MAX_X / 2), 2);
}

int game_loop()
{
    int ch = 0;

    srand(time(0));
    new_queue();
    update_piece(true);
    timeout(0);

    int start = (int)clock();
    int end, diff_in_ms;
    do
    {
        ch = getch();
        if (ch == 'q')
        {
            endwin();
            return 0;
        }
        move_piece(ch);

        end = (int)(clock());
        diff_in_ms = (int)(((double)(end - start) / CLOCKS_PER_SEC) * 1000);

        if (diff_in_ms > (1000 / TICKS_PER_SECOND))
        {
            start = (int)clock();
            if (floor_collision_check())
                new_queue();
            else
                operation(KEY_DOWN);
            update_piece(false);
        }

        refresh();
    } while (true);
}

int main()
{
    initialize_ncurses_screen(&MAX_X, &MAX_Y);

    BOARD_HEIGHT = MAX_Y - 2;
    BOARD_WIDTH = MAX_X - 2;

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
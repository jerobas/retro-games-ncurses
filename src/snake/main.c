#include <ncurses.h>
#include <signal.h>
#include <poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "../ncurses-handler.h"

#define MAX_SCORE 100
#define TICKS_PER_SECOND 10

int MAX_X, MAX_Y;
int SNAKE_LENGTH = 1;
int SNAKE[MAX_SCORE][2];
int SEED[2];

int MOVEMENT_DIRECTION = 0;
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
    default:
        return -1;
    }
}

void initialize_game()
{
    for (int i = 0; i < MAX_SCORE; i++)
    {
        SNAKE[i][0] = 2;
        SNAKE[i][1] = 2;
    }
}

bool collision_check()
{
    for (int i = 1; i < SNAKE_LENGTH; i++)
    {
        if (SNAKE[0][0] == SNAKE[i][0] && SNAKE[0][1] == SNAKE[i][1])
            return true;
    }

    if (SNAKE[0][0] == 1 || SNAKE[0][0] == MAX_X - 1 || SNAKE[0][1] == 1 || SNAKE[0][1] == MAX_Y - 1)
        return true;

    return false;
}

bool is_there_seed(int check_location[2])
{
    bool x_check = check_location[0] == SEED[0];
    bool y_check = check_location[1] == SEED[1];

    return x_check && y_check;
}

bool is_there_seed_in_snake()
{
    for (int i = 0; i < SNAKE_LENGTH; i++)
    {
        if (is_there_seed(SNAKE[i]))
            return true;
    }
    return false;
}

bool move_snake(bool grow)
{
    if (grow)
        SNAKE_LENGTH++;

    mvaddch(SNAKE[SNAKE_LENGTH - 1][1], SNAKE[SNAKE_LENGTH - 1][0], ' ');

    for (int i = SNAKE_LENGTH - 1; i > 0; i--)
    {
        SNAKE[i][0] = SNAKE[i - 1][0];
        SNAKE[i][1] = SNAKE[i - 1][1];
    }

    SNAKE[0][0] += MOVEMENT_DIRECTION == KEY_LEFT ? -1 : MOVEMENT_DIRECTION == KEY_RIGHT ? 1
                                                                                         : 0;
    SNAKE[0][1] += MOVEMENT_DIRECTION == KEY_UP ? -1 : MOVEMENT_DIRECTION == KEY_DOWN ? 1
                                                                                      : 0;

    mvprintw(1, MAX_X - 3, "%02d", SNAKE_LENGTH);

    mvaddch(SNAKE[0][1], SNAKE[0][0], '0');
    return is_there_seed(SNAKE[0]);
}

void new_seed()
{
    do
    {
        SEED[0] = rand() % (MAX_X - 2) + 1;
        SEED[1] = rand() % (MAX_Y - 2) + 1;
    } while (is_there_seed_in_snake());

    mvaddch(SEED[1], SEED[0], 'X');
}

void game_loop()
{
    int y = 0, x = 0;
    bool grow = false;
    int ch = KEY_RIGHT;
    int possible_newch = 0;
    srand(time(NULL));
    new_seed();
    timeout(0);
    MOVEMENT_DIRECTION = KEY_RIGHT;

    do
    {
        if (collision_check())
        {
            endwin();
            exit(0);
        }

        int start = (int)clock();
        int end, diff_in_ms;
        do
        {
            end = (int)(clock());

            possible_newch = getch();
            if (possible_newch != 0 && possible_newch != ERR && possible_newch != opposite_directions(MOVEMENT_DIRECTION) && possible_newch != ch)
                ch = possible_newch;

            diff_in_ms = (int)(((double)(end - start) / CLOCKS_PER_SEC) * 1000);
        } while (diff_in_ms < (1000 / TICKS_PER_SECOND));

        if (ch == 'q')
        {
            endwin();
            exit(0);
        }
        else if (opposite_directions(MOVEMENT_DIRECTION) != -1)
            MOVEMENT_DIRECTION = ch;

        grow = move_snake(grow);
        if (grow)
            new_seed();

        refresh();
    } while (true);
}

int main()
{
    initialize_ncurses_screen(&MAX_X, &MAX_Y);
    rectangle(0, MAX_X - 1, 0, MAX_Y - 1);
    mvprintw(1, MAX_X - 10, "Score: 00");
    refresh();

    initialize_game();
    game_loop();

    endwin();
    return 0;
}
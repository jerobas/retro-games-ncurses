#include <ncurses.h>
#include <signal.h>
#include <poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include "../ncurses-handler.h"

#define MAX_SCORE 100
#define TICKS_PER_SECOND 8

int MAX_X, MAX_Y;
int SNAKE_LENGTH = 1;
int MOVEMENT_DIRECTION[] = {1, 0};
int SNAKE[MAX_SCORE][2];
int SEED[2];

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

    SNAKE[0][0] += MOVEMENT_DIRECTION[0];
    SNAKE[0][1] += MOVEMENT_DIRECTION[1];

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
    int ch, possible_newch = 0;
    srand(time(NULL));
    new_seed();
    timeout(0);

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
            if (possible_newch != 0 && possible_newch != ERR)
                ch = possible_newch;

            diff_in_ms = (int)(((double)(end - start) / CLOCKS_PER_SEC) * 1000);
        } while (diff_in_ms < (1000 / TICKS_PER_SECOND));

        switch (ch)
        {
        case 'q':
            endwin();
            exit(0);
            break;
        case KEY_UP:
            if (MOVEMENT_DIRECTION[1] != 1)
            {
                MOVEMENT_DIRECTION[0] = 0;
                MOVEMENT_DIRECTION[1] = -1;
            }
            break;
        case KEY_DOWN:
            if (MOVEMENT_DIRECTION[1] != -1)
            {
                MOVEMENT_DIRECTION[0] = 0;
                MOVEMENT_DIRECTION[1] = 1;
            }
            break;
        case KEY_LEFT:
            if (MOVEMENT_DIRECTION[0] != 1)
            {
                MOVEMENT_DIRECTION[0] = -1;
                MOVEMENT_DIRECTION[1] = 0;
            }
            break;
        case KEY_RIGHT:
            if (MOVEMENT_DIRECTION[0] != -1)
            {
                MOVEMENT_DIRECTION[0] = 1;
                MOVEMENT_DIRECTION[1] = 0;
            }
            break;

        default:
            break;
        }

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
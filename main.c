#include <ncurses.h>
#include <signal.h>
#include <poll.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SCORE 100

int MAX_X, MAX_Y;
int SNAKE_LENGTH = 1;
int MOVEMENT_DIRECTION[] = {1, 0};
int SNAKE[MAX_SCORE][2];
int SEED[2];

void initialize_game()
{
    initscr();

    getmaxyx(stdscr, MAX_Y, MAX_X);
    // if (y < 40 || x < 40)
    // {
    //     endwin();
    //     printf("Terminal too small\n");
    //     exit(1);
    // }

    for (int i = 0; i < 100; i++)
    {
        SNAKE[i][0] = 2;
        SNAKE[i][1] = 2;
    }

    noecho();
    raw();
    keypad(stdscr, TRUE);
    curs_set(0);
    refresh();
}

void rectangle(int y1, int x1, int y2, int x2)
{
    mvhline(y1, x1, 0, x2 - x1);
    mvhline(y2, x1, 0, x2 - x1);
    mvvline(y1, x1, 0, y2 - y1);
    mvvline(y1, x2, 0, y2 - y1);
    mvaddch(y1, x1, ACS_ULCORNER);
    mvaddch(y2, x1, ACS_LLCORNER);
    mvaddch(y1, x2, ACS_URCORNER);
    mvaddch(y2, x2, ACS_LRCORNER);
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
    srand(time(NULL));
    new_seed();

    do
    {
        if (collision_check())
        {
            endwin();
            exit(0);
        }

        int ch = 0;

        struct pollfd mypoll = {STDIN_FILENO, POLLIN | POLLPRI};
        int start = (int)clock();
        if (poll(&mypoll, 1, 1000))
        {
            ch = getch();
        }

        // mvprintw(MAX_Y - 2, 1, "time: %d", clock() - start);

        int end, diff;
        do
        {
            end = (int)clock();
            diff = (int)(((end - start) / CLOCKS_PER_SEC) * 1000);
        } while (end < start + 1000);

        if (ch == 'q')
        {
            endwin();
            exit(0);
        }
        if (ch == KEY_UP)
        {
            MOVEMENT_DIRECTION[0] = 0;
            MOVEMENT_DIRECTION[1] = -1;
        }
        if (ch == KEY_DOWN)
        {
            MOVEMENT_DIRECTION[0] = 0;
            MOVEMENT_DIRECTION[1] = 1;
        }
        if (ch == KEY_LEFT)
        {
            MOVEMENT_DIRECTION[0] = -1;
            MOVEMENT_DIRECTION[1] = 0;
        }
        if (ch == KEY_RIGHT)
        {
            MOVEMENT_DIRECTION[0] = 1;
            MOVEMENT_DIRECTION[1] = 0;
        }

        grow = move_snake(grow);
        if (grow)
            new_seed();

        refresh();
    } while (true);
}

int main()
{
    initialize_game();

    rectangle(0, 0, MAX_Y - 1, MAX_X - 1);
    mvprintw(1, MAX_X - 10, "Score: ");
    refresh();

    game_loop();

    endwin();

    return 0;
}
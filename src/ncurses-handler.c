#include <ncurses.h>

#include "ncurses-handler.h"

// function to initialize the ncurses screen
int initialize_ncurses_screen(int *external_x, int *external_y)
{
    int x, y;

    initscr();
    getmaxyx(stdscr, y, x);
    // if (y < 40 || x < 40)
    // {
    //     endwin();
    //     printf("Terminal too small\n");
    //     exit(1);
    // }

    noecho();
    raw();
    keypad(stdscr, TRUE);
    curs_set(0);
    refresh();

    *external_x = x;
    *external_y = y;

    return 0;
}

void rectangle(int x1, int x2, int y1, int y2)
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
#include <ncurses.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#include "../ncurses-handler.h"
#include "../arena.h"

#define TICKS_PER_SECOND 5

typedef struct tetris_piece
{
    int shape[4][2];
    int rotation;
    int (*rotate)(struct tetris_piece *self, int transform[4][2]);
    int type;
} tetris_piece;

tetris_piece init_tetris_piece(int type, int x, int y);
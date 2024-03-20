#include "main.h"
#include <ncurses.h>

/*
    0:
        _ _ 4
        1 2 3
    1:
        1 _ _
        2 3 4
    2:
        1 2 _
        _ 3 4
    3:
        _ 3 4
        1 2 _
    4:
        1 2 3 4
    5:
        _ 1 _
        2 3 4
    6:
        1 2
        3 4
*/

int do_nothing(tetris_piece *piece, int transform[4][2])
{
    return 0;
}

int block_centralized_rotate(tetris_piece *piece, int transform[4][2])
{
    int center_block;

    switch (piece->type)
    {
    case 0:
        center_block = 1;
        break;
    case 3:
        center_block = 1;
        break;
    case 4:
        center_block = 1;
        break;
    default:
        center_block = 2;
        break;
    }

    int(*piece_array)[2] = piece->shape;

    for (int i = 0; i < 4; i++)
    {
        int x = piece_array[i][0];
        int y = piece_array[i][1];

        transform[i][0] = piece_array[center_block][0] - (y - piece_array[center_block][1]);
        transform[i][1] = piece_array[center_block][1] + (x - piece_array[center_block][0]);
    }

    return 0;
}

int i_rotate(tetris_piece *piece, int transform[4][2])
{
    block_centralized_rotate(piece, transform);

    //+x +y -x -y
    int signal = piece->rotation > 1 ? -1 : 1;
    int if_true_x = piece->rotation % 2 ? 0 : 1;

    for (int i = 0; i < 4; i++)
        if (if_true_x)
            transform[i][0] = piece->shape[i][0] + signal;
        else
            transform[i][1] = piece->shape[i][1] + signal;

    mvprintw(2, 2, "rotation: %d", piece->rotation);
    piece->rotation = (piece->rotation + 1) % 4;
    return 0;
}

int (*define_rotate(int type))(tetris_piece *, int[4][2])
{
    if (type / 7 > 0)
        return do_nothing;

    switch (type)
    {
    case 4:
        return i_rotate;
    case 6:
        return do_nothing;
    default:
        return block_centralized_rotate;
    }
}

int new_piece(int piece_array[4][2], int type, int midx, int starty)
{
    switch (type)
    {
    case 0:
        piece_array[0][0] = midx - 1;
        piece_array[1][0] = midx;
        piece_array[2][0] = midx + 1;
        piece_array[3][0] = midx + 1;
        piece_array[0][1] = starty + 1;
        piece_array[1][1] = starty + 1;
        piece_array[2][1] = starty + 1;
        piece_array[3][1] = starty;
        break;
    case 1:
        piece_array[0][0] = midx - 1;
        piece_array[1][0] = midx - 1;
        piece_array[2][0] = midx;
        piece_array[3][0] = midx + 1;
        piece_array[0][1] = starty;
        piece_array[1][1] = starty + 1;
        piece_array[2][1] = starty + 1;
        piece_array[3][1] = starty + 1;
        break;
    case 2:
        piece_array[0][0] = midx - 1;
        piece_array[1][0] = midx;
        piece_array[2][0] = midx;
        piece_array[3][0] = midx + 1;
        piece_array[0][1] = starty;
        piece_array[1][1] = starty;
        piece_array[2][1] = starty + 1;
        piece_array[3][1] = starty + 1;
        break;
    case 3:
        piece_array[0][0] = midx - 1;
        piece_array[1][0] = midx;
        piece_array[2][0] = midx;
        piece_array[3][0] = midx + 1;
        piece_array[0][1] = starty + 1;
        piece_array[1][1] = starty + 1;
        piece_array[2][1] = starty;
        piece_array[3][1] = starty;
        break;
    case 4:
        piece_array[0][0] = midx - 1;
        piece_array[1][0] = midx;
        piece_array[2][0] = midx + 1;
        piece_array[3][0] = midx + 2;
        piece_array[0][1] = starty;
        piece_array[1][1] = starty;
        piece_array[2][1] = starty;
        piece_array[3][1] = starty;
        break;
    case 5:
        piece_array[0][0] = midx;
        piece_array[1][0] = midx - 1;
        piece_array[2][0] = midx;
        piece_array[3][0] = midx + 1;
        piece_array[0][1] = starty;
        piece_array[1][1] = starty + 1;
        piece_array[2][1] = starty + 1;
        piece_array[3][1] = starty + 1;
        break;
    case 6:
        piece_array[0][0] = midx;
        piece_array[1][0] = midx + 1;
        piece_array[2][0] = midx;
        piece_array[3][0] = midx + 1;
        piece_array[0][1] = starty;
        piece_array[1][1] = starty;
        piece_array[2][1] = starty + 1;
        piece_array[3][1] = starty + 1;
        break;
    }
}

tetris_piece init_tetris_piece(int type, int x, int y)
{
    tetris_piece piece;

    piece.rotation = 0;
    piece.rotate = define_rotate(type);
    piece.type = type;
    new_piece(piece.shape, type, x, y);

    return piece;
}
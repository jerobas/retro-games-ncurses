#include "main.h"

// void rotate(tetris_piece *piece)
// {
//     piece->rotation = (piece->rotation + 1) % 4;
// }

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
int do_nothing(tetris_piece *piece)
{
    return 0;
}

int (*define_rotate(int type))(tetris_piece *)
{
    if (type / 7 > 0)
        return do_nothing;

    switch (type)
    {
    case 6:
        return do_nothing;
    }
}

tetris_piece init_tetris_piece(int type, int x, int y)
{
    tetris_piece piece;

    piece.rotation = 0;
    piece.rotate = define_rotate(type);

    return piece;
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
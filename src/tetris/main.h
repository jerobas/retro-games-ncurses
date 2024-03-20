typedef struct tetris_piece
{
    int shape[4][2];
    int rotation;
    int (*rotate)(struct tetris_piece *self);
} tetris_piece;

tetris_piece init_tetris_piece(int type, int x, int y);
int new_piece(int piece_array[4][2], int type, int midx, int starty);
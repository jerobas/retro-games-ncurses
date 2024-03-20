typedef struct tetris_piece
{
    int shape[4][2];
    int rotation;
    int (*rotate)(struct tetris_piece *self, int transform[4][2]);
    int type;
} tetris_piece;

tetris_piece init_tetris_piece(int type, int x, int y);
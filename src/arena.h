#ifndef ARENA_ALREADY_DEFINED
#define ARENA_ALREADY_DEFINED

struct arena_node
{
    int x;
    int y;
    void (*update)();
    void (*render)();
    struct arena_node *next;
};

typedef struct arena_node arena_node;

struct arena
{
    int left_x;
    int right_x;
    int top_y;
    int bottom_y;
    arena_node *nodes;
};

typedef struct arena arena;

/// @brief
/// @param x
/// @param y
/// @param update
/// @return
arena_node arena_node_create(int x, int y, void (*update)(), void (*render)());

arena arena_create(int left_x, int right_x, int top_y, int bottom_y);

void arena_render_nodes(arena arena);

#endif
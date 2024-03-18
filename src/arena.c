#include "arena.h"

arena_node arena_node_create(int x, int y, void (*update)(), void (*render)())
{
    arena_node node;
    node.x = x;
    node.y = y;
    node.update = update;
    node.render = render;
    node.next = 0;

    return node;
}

arena arena_create(int left_x, int right_x, int top_y, int bottom_y)
{
    arena new_arena;
    new_arena.left_x = left_x;
    new_arena.right_x = right_x;
    new_arena.top_y = top_y;
    new_arena.bottom_y = bottom_y;
    new_arena.nodes = 0;

    return new_arena;
}

void arena_render_nodes(arena arena)
{
    arena_node *current_node = arena.nodes;
    while (current_node != 0)
    {
        current_node->render();
        current_node = current_node->next;
    }
}

/*
void arena_add_node(arena *arena, arena_node *node)
{
    node->next = arena->nodes;
    arena->nodes = node;
}
*/
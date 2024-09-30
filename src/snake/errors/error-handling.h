#include <stdio.h>
#include <stdlib.h>

typedef struct cleanup_function_node
{
    void (*cleanup)(void *);
    struct cleanup_function *next;
} cleanup_function_node;

extern cleanup_function_node *cleanup_functions;
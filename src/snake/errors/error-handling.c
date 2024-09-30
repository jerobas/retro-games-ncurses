#include "error-handling.h"

cleanup_function_node *cleanup_functions = NULL;

void add_cleanup_function(void (*function)())
{
    cleanup_function_node *new_node = (cleanup_function_node *)malloc(sizeof(cleanup_function_node));
    if (new_node == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for cleanup function node\n");
        exit(EXIT_FAILURE);
    }

    new_node->function = function;
    new_node->next = NULL;
    cleanup_functions = new_node;
}

void cleanup()
{
    cleanup_function_node *current_node = cleanup_functions;
    while (current_node != NULL)
    {
        current_node->function();
        cleanup_function_node *next_node = current_node->next;
        free(current_node);
        current_node = next_node;
    }
}

void cleanup_with_error(int error_code)
{
    cleanup();
    exit(EXIT_FAILURE);
}
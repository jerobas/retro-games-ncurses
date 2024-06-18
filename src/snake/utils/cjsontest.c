#include <stdio.h>
#include "../../lib/cJSON.h"

int main()
{
    char *test = "{\"123123\": [1, 2], \"123124\": [3, 4], \"123125\": [5, 6]}";

    cJSON *json = cJSON_Parse(test);
    cJSON *next_node = json->child;
    next_node = next_node->next;
    next_node = next_node->next;
    next_node = next_node->next;

    if (next_node == NULL)
    {
        printf("NULL\n");
    }

    return 0;
}
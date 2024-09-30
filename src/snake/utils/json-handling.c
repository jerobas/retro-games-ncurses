#include "json-handling.h"

cJSON_linked_list cJSONs_cleanup_list = NULL;
bool cJSONs_cleanup_function_added = false;

cJSON *create_cJSON()
{
    cJSON *json = cJSON_CreateObject();

    // need to change
    if (json == NULL)
    {
        fprintf(stderr, "Failed to create JSON object\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    cJSONs_cleanup_list = {.json = json, .next = NULL};

    if (!cJSONs_cleanup_function_added)
    {
        add_cleanup_function(cleanup_cJSONs);
        cJSONs_cleanup_function_added = true;
    }

    return json;
}

char *stringify_cJSON(cJSON *json)
{
    char *json_string = cJSON_PrintUnformatted(json);
    if (json_string == NULL)
    {
        fprintf(stderr, "Failed to print JSON\n");
        cJSON_Delete(json);
        handle_close_socket();
        exit(EXIT_FAILURE);
    }
}

void delete_cJSON(cJSON *json)
{
    cJSON_linked_list *current_node = cJSONs_cleanup_list;
    while (current_node != NULL)
    {
        cJSON_linked_list *next_node = current_node->next;

        if (next_node->json == json)
        {
            current_node->next = next_node->next;
            break;
        }

        current_node = next_node;
    }

    cJSON_Delete(json);
}

void cleanup_cJSONs()
{
    cJSON_linked_list *current_node = cJSONs_cleanup_list;
    while (current_node != NULL)
    {
        cJSON_Delete(current_node->json);
        cJSON_linked_list *next_node = current_node->next;
        current_node = next_node;
    }
}
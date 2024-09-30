#include "../../lib/cJSON.h"

typedef struct cJSON_linked_list
{
    cJSON *json;
    struct cJSON_linked_list *next;
} cJSON_linked_list;

/// @brief Creates a cJSON object (error handling included)
/// @return cJSON object
cJSON *create_cJSON();

/// @brief Stringifies a cJSON object (error handling included)
/// @param cJSON json
/// @return char* JSON string
char *stringify_cJSON(cJSON *json);
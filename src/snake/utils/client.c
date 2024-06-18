#include "client.h"
#include "../types/types.c"

int sockfd;
char bufferCheck[BUFFER_SIZE];

/// @brief Creates a cJSON object (error handling included)
/// @return cJSON object
cJSON *create_cJSON()
{
    cJSON *json = cJSON_CreateObject();
    if (json == NULL)
    {
        fprintf(stderr, "Failed to create JSON object\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    return json;
}

/// @brief Stringifies a cJSON object (error handling included)
/// @param cJSON json
/// @return char* JSON string
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

//
// functions above probably should be in a separate file
//

/// @brief Closes the socket connection
void handle_close_socket()
{
    close(sockfd);
}

/// @brief Starts the socket connection with the server
void handle_socket_connection()
{
    struct sockaddr_in server_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0)
    {
        perror("Invalid address");
        handle_close_socket();
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        handle_close_socket();
        exit(EXIT_FAILURE);
    }
}

// separator

void send_string(char *string)
{
    if (send(sockfd, string, strlen(string), 0) < 0)
    {
        perror("Send failed");
        // free(string);
        handle_close_socket();
        exit(EXIT_FAILURE);
    }
}

void send_json(cJSON *json)
{
    char *json_string = stringify_cJSON(json);

    send_string(json_string);

    cJSON_Delete(json);
    free(json_string);
}

/// @brief  Sends a flag to the server (error handling included)
/// @param char* flag
void send_flag(char *flag)
{
    cJSON *json = create_cJSON();

    cJSON_AddStringToObject(json, "flag", flag);

    send_json(json);
}

/// @brief Sends a connection intention to the server
void send_connection_intention()
{
    send_flag("join");
}

// separator

/// @brief Receives a cJSON object from the server (error handling included)
/// @return cJSON* JSON object
cJSON *receive_json(int timeout)
{
    int bytes;
    char buffer[BUFFER_SIZE];

    int start = (int)(clock());
    int end, diff_in_ms;

    do
    {
        bytes = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        end = (int)(clock());
        diff_in_ms = (int)(((double)(end - *start) / CLOCKS_PER_SEC) * 1000);
    } while (bytes <= 0 && (diff_in_ms <= timeout || timeout < 0));

    if (bytes <= 0)
    {
        perror("Failed to receive JSON");
        handle_close_socket();
        exit(EXIT_FAILURE);
    }
    buffer[bytes] = '\0';

    cJSON *response_json = cJSON_Parse(buffer);
    if (response_json == NULL)
    {
        fprintf(stderr, "Failed to parse JSON\n");
        handle_close_socket();
        exit(EXIT_FAILURE);
    }

    return response_json;
}

/// @brief Receives a UUID from the server
/// @return uuid playerId
uuid receive_uuid()
{
    uuid playerId;

    cJSON *response_json = receive_json(2000);

    cJSON *uuid_json = cJSON_GetObjectItem(response_json, "playerId");
    if (!cJSON_IsString(uuid_json))
    {
        fprintf(stderr, "Invalid UUID in JSON\n");
        cJSON_Delete(response_json);
        handle_close_socket();
        exit(EXIT_FAILURE);
    }
    playerId = (uuid *)uuid_json->valuestring;

    cJSON_Delete(response_json);
    cJSON_Delete(uuid_json);

    return playerId;
}

bool check_if_flag_was_received(char *flag)
{
    cJSON *response_json = receive_json(2000);

    cJSON *flag_json = cJSON_GetObjectItem(response_json, "flag");
    if (!cJSON_IsString(flag_json))
    {
        fprintf(stderr, "Invalid flag in JSON\n");
        cJSON_Delete(response_json);
        handle_close_socket();
        exit(EXIT_FAILURE);
    }

    bool result = strcmp(flag, flag_json->valuestring) == 0;

    cJSON_Delete(response_json);
    cJSON_Delete(flag_json);

    return result;
}

/// @brief Handles the connection with the server
/// @return uuid playerId
uuid handle_connection()
{
    uuid playerId;

    handle_socket_connection();

    send_connection_intention();

    return receive_uuid();
}

/// @brief Awaits the game start signal from the server
void await_game_start()
{
    cJSON *response_json = receive_json(-1);

    cJSON *game_start_json = cJSON_GetObjectItem(response_json, "flag");

    if (!cJSON_IsString(game_start_json))
    {
        fprintf(stderr, "Invalid flag in JSON\n");
        cJSON_Delete(response_json);
        handle_close_socket();
        exit(EXIT_FAILURE);
    }

    char *flag = uuid_json->valuestring;

    if (strcmp(flag, "start") == 0)
        break;

    cJSON_Delete(response_json);
    cJSON_Delete(uuid_json);

    // if (server_response == 1)
    // {
    //     mvprintw(0, 0, "waiting for connection");
    //     for (int j = 0; j < 3; j++)
    //     {
    //         mvprintw(0, 22 + j, j <= i ? "." : " ");
    //     }
    //     i = (i + 1) % 3;
    //     refresh();
    // }
}

/// @brief Awaits the game's initial state from the server
void await_initial_state(int **players_state)
{
    cJSON *response_json = receive_json();

    if (response_json == NULL || !cJSON_IsObject(response_json) || response_json->child == NULL)
    {
        fprintf(stderr, "Invalid JSON\n");
        cJSON_Delete(response_json);
        handle_close_socket();
        exit(EXIT_FAILURE);
    }

    int num_players = cJSON_GetArraySize(response_json);
    players_state = (int **)malloc(num_players * 3 * sizeof(int *));

    cJSON *next_node;
    int i = 0;
    do
    {
        cJSON *next_node = response_json->child;
        players_state[i][0] = next_node->string;
        players_state[i][1] = cJSON_GetArrayItem(next_node, 0)->valueint;
        players_state[i][2] = cJSON_GetArrayItem(next_node, 1)->valueint;
    } while (next_node != NULL);

    cJSON_Delete(response_json);
    cJSON_Delete(next_node);
}

void await_first_tick_flag()
{
    while (!check_if_flag_was_received("first_tick"))
        ;
}

/// @brief Sends a movement command with it's direction to the server
/// @param int direction
/// @param char* playerId
void send_direction(int direction, char *playerId)
{
    cJSON *json = create_cJSON();

    cJSON_AddStringToObject(json, "playerId", playerId);
    cJSON_AddStringToObject(json, "next_move", direction_string(direction));

    char *json_string = stringify_cJSON(json);

    if (send(sockfd, json_string, strlen(json_string), 0) < 0)
    {
        perror("Send failed");
        cJSON_Delete(json);
        free(json_string);
        handle_close_socket();
        exit(EXIT_FAILURE);
    }

    cJSON_Delete(json);
    free(json_string);
}
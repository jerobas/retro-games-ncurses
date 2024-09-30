#include "client.h"

int sockfd;
char bufferCheck[BUFFER_SIZE];
uuid playerId;

void handle_socket_connection()
{
    struct sockaddr_in server_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    add_cleanup_function(handle_close_socket);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0)
    {
        perror("Invalid address");
        cleanup_with_error(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        cleanup_with_error(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }
}

void handle_close_socket()
{
    close(sockfd);
}

// separator

void send_string(char *string)
{
    if (send(sockfd, string, strlen(string), 0) < 0)
    {
        perror("Send failed");
        cleanup_with_error(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }
}
void send_json(cJSON *json)
{
    char str[(int)(ceil(log10(num)) + 1)];
    sprintf(str, "%d", playerId);
    cJSON_AddStringToObject(json, "playerId", str);

    char *json_string = stringify_cJSON(json);

    send_string(json_string);
}

// separator

void send_flag(char *flag)
{
    cJSON *json = create_cJSON();

    cJSON_AddStringToObject(json, "flag", flag);

    send_json(json);
    delete_cJSON(json);
}

void send_connection_intention()
{
    send_flag("join");
}

// separator

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
        cleanup_with_error(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }
    buffer[bytes] = '\0';

    cJSON *response_json = cJSON_Parse(buffer);
    if (response_json == NULL)
    {
        fprintf(stderr, "Failed to parse JSON\n");
        cleanup_with_error(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    return response_json;
}

void receive_uuid()
{
    cJSON *response_json = receive_json(2000);

    cJSON *uuid_json = cJSON_GetObjectItem(response_json, "playerId");
    if (!cJSON_IsString(uuid_json))
    {
        fprintf(stderr, "Invalid UUID in JSON\n");
        cJSON_Delete(response_json);
        cleanup_with_error(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }
    playerId = (uuid *)atoi(uuid_json->valuestring);

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
        cleanup_with_error(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    bool result = strcmp(flag, flag_json->valuestring) == 0;

    cJSON_Delete(response_json);
    cJSON_Delete(flag_json);

    return result;
}

void handle_connection()
{
    handle_socket_connection();

    send_connection_intention();

    receive_uuid();
}

void await_game_start()
{
    cJSON *response_json = receive_json(-1);
    cJSON *game_start_json = cJSON_GetObjectItem(response_json, "flag");

    if (!cJSON_IsString(game_start_json))
    {
        fprintf(stderr, "Invalid flag in JSON\n");
        cJSON_Delete(response_json);
        cleanup_with_error(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    char *flag = game_start_json->valuestring;

    // NOTE: we need to decide what do to with this part of the code
    if (strcmp(flag, "start") != 0)
        ;

    cJSON_Delete(game_start_json);

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

players_state await_initial_state()
{
    cJSON *response_json = receive_json(-1);
    players_state players_state;

    if (response_json == NULL || !cJSON_IsObject(response_json) || response_json->child == NULL)
    {
        fprintf(stderr, "Invalid JSON\n");
        cJSON_Delete(response_json);
        cleanup_with_error(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    int num_players = cJSON_GetArraySize(response_json);

    cJSON *current_node = response_json->child;
    players_state_list_node *current_player_node = (players_state_list_node *)malloc(sizeof(players_state_list_node));
    do
    {
        current_player_node.player.playerId = current_node->string;
        current_player_node.player.x = cJSON_GetArrayItem(current_node, 0)->valueint;
        current_player_node.player.y = cJSON_GetArrayItem(current_node, 1)->valueint;

        cJSON *current_node = current_node->next;
        if (current_node == NULL)
            break;

        players_state_list_node *new_player_node = (players_state_list_node *)malloc(sizeof(players_state_list_node));
        current_player_node->next = new_player_node;
    } while (current_node != NULL);

    players_state.num_players = num_players;

    cJSON_Delete(response_json);

    return players_state;
}

bool check_tick_ended(players_state *players_state)
{
    cJSON *response_json = receive_json(0);

    if (response_json == NULL)
        return false;

    if (!cJSON_IsObject(response_json) || response_json->child == NULL)
    {
        fprintf(stderr, "Invalid JSON\n");
        cJSON_Delete(response_json);
        cleanup_with_error(EXIT_FAILURE);
        exit(EXIT_FAILURE);
    }

    cJSON *players_update = response_json.cJSON_GetObjectItem("players_update");

    players_state_list_node *current_player_node = (players_state_list_node *)malloc(sizeof(players_state_list_node));

    do
    {
        cJSON *current_node = cJSON_GetObjectItem(players_update, current_player_node->player.playerId);
        cJSON *current_node_snake = cJSON_GetObjectItem(current_node, "snake");
        int current_node_snake_size = cJSON_GetArraySize(current_node_snake);

        current_player_node.player.snake = malloc(sizeof(int *) * 2 * current_node_snake_size);

        for (int i = 0; i < current_node_snake_size; i++)
        {
            current_player_node.player.snake[i][0] = cJSON_GetArrayItem(cJSON_GetArrayItem(current_node_snake, i), 0)->valueint;
            current_player_node.player.snake[i][1] = cJSON_GetArrayItem(cJSON_GetArrayItem(current_node_snake, i), 1)->valueint;
        }

        current_player_node.player.playerId = current_node->string;
        current_player_node.player.grow = cJSON_GetObjectItem(current_node, "grow")->valuebool;
        current_player_node.player.alive = cJSON_GetObjectItem(current_node, "alive")->valuebool;

        current_player_node = current_player_node->next;
    } while (current_player_node != NULL);

    cJSON_Delete(response_json);

    return players_state;
    return true;
}

void await_first_tick_flag()
{
    while (!check_if_flag_was_received("first_tick"))
        ;
}

void send_direction(int direction)
{
    cJSON *json = create_cJSON();

    cJSON_AddStringToObject(json, "next_move", direction_string(direction));

    send_json(json);

    cJSON_Delete(json);
}

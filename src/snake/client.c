#include "client.h"

int sockfd;
char bufferCheck[BUFFER_SIZE];

int checkConnection()
{
    int bytes = recv(sockfd, bufferCheck, BUFFER_SIZE - 1, 0);
    if (bytes == 4)
    {
        bufferCheck[5] = '\0';
        if (strcmp(bufferCheck, "true") == 0)
            return 0;
    }
    return 1;
}

void sendDirection(char *direction, char *playerId)
{
    cJSON *arquivo = cJSON_CreateObject();
    if (arquivo == NULL)
    {
        fprintf(stderr, "Failed to create JSON object\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    cJSON_AddStringToObject(arquivo, "type", "MOVE");
    cJSON_AddStringToObject(arquivo, "direction", direction);
    cJSON_AddStringToObject(arquivo, "playerId", playerId);

    char *string_arquivo = cJSON_PrintUnformatted(arquivo);
    if (string_arquivo == NULL)
    {
        fprintf(stderr, "Failed to print JSON\n");
        cJSON_Delete(arquivo);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (send(sockfd, string_arquivo, strlen(string_arquivo), 0) < 0)
    {
        perror("Send failed");
        cJSON_Delete(arquivo);
        free(string_arquivo);
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    cJSON_Delete(arquivo);
    free(string_arquivo);
}

char *handle_connection()
{
    char *playerId = (char *)malloc(37 * sizeof(char));
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

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
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    int bytes = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes <= 0)
    {
        perror("Failed to receive UUID");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    buffer[bytes] = '\0';

    cJSON *response_json = cJSON_Parse(buffer);
    if (response_json == NULL)
    {
        fprintf(stderr, "Failed to parse JSON\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    cJSON *uuid_json = cJSON_GetObjectItem(response_json, "playerId");
    if (!cJSON_IsString(uuid_json))
    {
        fprintf(stderr, "Invalid UUID in JSON\n");
        cJSON_Delete(response_json);
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    strcpy(playerId, uuid_json->valuestring);

    cJSON_Delete(response_json);

    return playerId;
}

void handle_close_socket()
{
    close(sockfd);
}
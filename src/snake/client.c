#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "cJSON.h"

#define BUFFER_SIZE 1024

int main()
{
    int sockfd;

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
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    cJSON *arquivo = cJSON_CreateObject();

    cJSON_AddStringToObject(arquivo, "type", "MOVE");
    cJSON_AddStringToObject(arquivo, "direction", "RIGHT");

    char *string_arquivo = cJSON_PrintUnformatted(arquivo);

    if (send(sockfd, string_arquivo, strlen(string_arquivo), 0) < 0)
    {
        printf("fodeu fml");
    }

    cJSON_Delete(arquivo);
    free(string_arquivo);

    // close(sockfd) quando der erro

    int bytes;

    while ((bytes = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0)
    {
        buffer[bytes] = '\0';
        printf("%s", buffer);
    }

    close(sockfd);

    return 0;
}
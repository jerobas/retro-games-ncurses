#ifndef CLIENT_ALREADY_DEFINED
#define CLIENT_ALREADY_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "cJSON.h"

#define BUFFER_SIZE 1024

extern int sockfd;

extern void sendDirection(int sockfd, char *direction, char *playerId);
extern void handle_close_socket();
extern char *handle_connection();

#endif
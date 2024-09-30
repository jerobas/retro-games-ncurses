#ifndef CLIENT_ALREADY_DEFINED
#define CLIENT_ALREADY_DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>
#include <math.h>

#include "../../lib/cJSON.h"

#include "../types/types.h"
#include "json-handling.h"
#include "../errors/error-handling.h"

#define BUFFER_SIZE 1024

extern int sockfd;
extern char bufferCheck[BUFFER_SIZE];

/// @brief Starts the socket connection with the server
extern void handle_socket_connection();

/// @brief Closes the socket connection
extern void handle_close_socket();

extern void send_string(char *string);

extern void send_json(cJSON *json);

/// @brief  Sends a flag to the server (error handling included)
/// @param char* flag
extern void send_flag(char *flag);

/// @brief Sends a connection intention to the server
extern void send_connection_intention();
// separator

/// @brief Receives a cJSON object from the server (error handling included)
/// @return cJSON* JSON object
extern cJSON *receive_json(int timeout);

/// @brief Receives a UUID from the server
/// @return uuid playerId
extern void receive_uuid();

extern bool check_if_flag_was_received(char *flag);

/// @brief Handles the connection with the server
/// @return uuid playerId
extern void handle_connection();

/// @brief Awaits the game start signal from the server
extern void await_game_start();

/// @brief Awaits the game's initial state from the server
extern void await_initial_state(players_state players_state);

extern bool check_tick_ended(int **players_state);

extern void await_first_tick_flag();

/// @brief Sends a movement command with it's direction to the server
/// @param int direction
/// @param char* playerId
extern void send_direction(int direction);

#endif
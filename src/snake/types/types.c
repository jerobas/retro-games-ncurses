#include "../../lib/cJSON.h"

typedef __uint128_t uuid;

// client to server
char *direction_string(int direction)
{
    switch (direction)
    {
    case 0:
        return "UP";
    case 1:
        return "LEFT";
    case 2:
        return "DOWN";
    case 3:
        return "RIGHT";
    default:
        return "INVALID";
    }
}
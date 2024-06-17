typedef __uint128_t uuid;

typedef enum message_type
{
    SYS,
    MOVE,
    GC
} message_type;

typedef enum game_control
{
    END_TICK = -1,
    RETURN_VALUES = -2,
    END_GAME = -3
} game_control;

typedef enum syscalls
{
    CONNECT = 0,
    DISCONNECT = 1,
    WAIT = 2
} syscalls;

typedef enum moveset
{
    UP = 'U',
    RIGHT = 'R',
    DOWN = 'D',
    LEFT = 'L'
} moveset;

typedef union message_data
{
    syscalls syscall;
    moveset move;
    game_control game_control;
} message_data;

typedef struct message
{
    uuid player_id;
    message_type type;
    message_data data;

} message;
typedef __uint128_t uuid;

typedef struct player_state_i
{
    uuid player_id;
    int x;
    int y;
} players_state_t;

typedef struct player_state_c
{
    uuid player_id;
    int **snake;
    bool grow;
    bool alive;
} players_state;

typedef union
{
    player_state_t;
    player_state_c;
} players_state;

typedef struct players_state_list_node
{
    players_state player;
    players_state_list_node *next;
} players_state_list_node;

typedef struct players_state
{
    players_state_list_node *players;
    int num_players;
} players_state;

char *direction_string(int direction);
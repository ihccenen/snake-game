typedef enum {
    ONGOING,
    PAUSED,
    GAMEOVER
} status;

typedef struct {
    char grid[15][15];
    status status;
} gameState;

typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT
} direction;

typedef struct {
    int x;
    int y;
    int steps;
    direction direction;
    direction nextDirection;
} snake;

void restart(gameState *gameState, snake *snake);

void addFood(gameState *gameState);

void move_snake(gameState *gameState, snake *snake);

void turn(snake *snake);

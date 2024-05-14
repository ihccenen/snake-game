typedef enum {
    ONGOING,
    PAUSED,
    GAMEOVER
} status;

typedef struct {
    char grid[15][15];
    status status;
    int foodX;
    int foodY;
} gameState;

typedef enum {
    UP,
    RIGHT,
    DOWN,
    LEFT
} direction;

typedef struct body {
    int x;
    int y;
    struct body * next;
} body_t;

typedef struct {
    int steps;
    direction direction;
    direction nextDirection;
    body_t body;
} snake;

void restart(gameState *gameState, snake *snake);

void addFood(gameState *gameState);

void drawSnake(int squareSize, body_t *body, gameState gameState);

void push(int x, int y, body_t *body);

void move_body(int x, int y, body_t *body, gameState *gameState);

void move_snake(gameState *gameState, snake *snake);

void turn(snake *snake);

#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "../headers/game.h"

int main() {
    const int screenWidth = 450;
    const int screenHeight = 450;
    gameState gameState;
    int squareSize = 30;
    snake snake = {0, RIGHT, RIGHT};
    gameState.status = PAUSED;

    body_t * head = NULL;
    head = (body_t *) malloc(sizeof(body_t));

    if (head == NULL) return 1;

    head->x = 0;
    head->y = 0;
    head->next = NULL;
    snake.body = *head;

    InitWindow(screenWidth, screenHeight, "Snake Game");

    gameState.grid[snake.body.x][snake.body.y] = '.';
    addFood(&gameState);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(RAYWHITE);

            for (int i = 0; i < 15; ++i) {
                for (int j = 0; j < 15; ++j) {
                    DrawRectangle(i * squareSize, j * squareSize, squareSize, squareSize, (i + j) % 2 == 0 ? GRAY : LIGHTGRAY);
                    if (gameState.grid[i][j] == '.') DrawRectangle(i * squareSize, j * squareSize, squareSize, squareSize, BLACK);
                }
            }

            DrawRectangle(gameState.foodX * squareSize, gameState.foodY * squareSize, squareSize, squareSize, DARKBLUE);
            drawSnake(squareSize, &snake.body, gameState);

            if (gameState.status == ONGOING) {
                move_snake(&gameState, &snake);
                turn(&snake);
            } else if (gameState.status == PAUSED) {
                DrawText("PRESS SPACE TO \nUNPAUSE/PAUSE\n\nOR ENTER\nTO RESTART", 100, 100, 30, BLACK);
            } else if (gameState.status == GAMEOVER) {
                DrawText("PRESS ENTER TO RESTART", 5, 100, 30, BLACK);
            }

            if (gameState.status != GAMEOVER && IsKeyPressed(KEY_SPACE)) {
                gameState.status = gameState.status == PAUSED ? ONGOING : PAUSED;
            }

            if (IsKeyPressed(KEY_ENTER)) restart(&gameState, &snake);

        EndDrawing();
    }

    CloseWindow();
    free(head);

    return 0;
}

void addFood(gameState *gameState)
{
    srandom(time(NULL));
    int x = random() % 15;
    int y = random() % 15;

    while (gameState->grid[x][y] == '.') {
        x = random() % 15;
        y = random() % 15;
    }

    gameState->foodX = x;
    gameState->foodY = y;
    gameState->grid[gameState->foodX][gameState->foodY] = '-';
}

void restart(gameState *gameState, snake *snake) {
    gameState->status = ONGOING;
    snake->body.x = 0;
    snake->body.y = 0;
    snake->steps = 0;
    snake->nextDirection = RIGHT;
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            gameState->grid[i][j] = ' ';
        }
    }
    gameState->grid[snake->body.x][snake->body.y] = '.';
    addFood(gameState);

    body_t * curr = snake->body.next;

    snake->body.next = NULL;

    while (curr != NULL) {
        body_t * temp = curr->next;
        free(curr);
        curr = temp;
    }

}

void drawSnake(int squareSize, body_t *body, gameState gameState)
{
    body_t * curr = body;
    DrawRectangle(curr->x * squareSize, curr->y * squareSize, squareSize, squareSize, gameState.status == GAMEOVER ? RED : BLACK);
    while (curr->next != NULL) {
        curr = curr->next;
        DrawRectangle(curr->x * squareSize, curr->y * squareSize, squareSize, squareSize, gameState.status == GAMEOVER ? RED : BLACK);
    }
}

void push(int x, int y, body_t *body)
{
    struct body *curr = body;

    while (curr->next != NULL) {
        curr = curr->next;
    }

    curr->next = (body_t *) malloc(sizeof(body_t));
    curr->next->x = x;
    curr->next->y = y;
}

void move_body(int x, int y, body_t *body, gameState *gameState)
{
    if (body->next == NULL) {
        gameState->grid[body->x][body->y] = ' ';
        body->x = x;
        body->y = y;
    } else {
        move_body(body->x, body->y, body->next, gameState);
        body->x = x;
        body->y = y;
    }
}

void move_snake(gameState *gameState, snake *snake)
{
    if (snake->steps < 10) {
        snake->steps += 1;
    } else {
        int x = snake->body.x;
        int y = snake->body.y;

        if (gameState->status != GAMEOVER) snake->direction = snake->nextDirection;

        if (snake->direction == UP) y -= 1;
        else if (snake->direction == RIGHT) x += 1;
        else if (snake->direction == DOWN) y += 1;
        else if (snake->direction == LEFT) x -= 1;

        if (gameState->grid[x][y] == '.' || x < 0 || x > 14 || y < 0 || y > 14) {
            gameState->status = GAMEOVER;
        } else {
            if (gameState->grid[x][y] == '-') {
                addFood(gameState);
                push(x, y, &snake->body);
            }
            snake->steps = 0;
            move_body(x, y, &snake->body, gameState);
            gameState->grid[snake->body.x][snake->body.y] = '.';
        }
    }
}

void turn(snake *snake)
{
    KeyboardKey key = GetKeyPressed();

    if ((key == KEY_UP || key == KEY_W) && snake->direction != DOWN) snake-> nextDirection = UP;
    if ((key == KEY_RIGHT || key == KEY_D) && snake->direction != LEFT) snake-> nextDirection = RIGHT;
    if ((key == KEY_DOWN || key == KEY_S) && snake->direction != UP) snake-> nextDirection = DOWN;
    if ((key == KEY_LEFT || key == KEY_A) && snake->direction != RIGHT) snake-> nextDirection = LEFT;
}

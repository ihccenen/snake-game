#include <stdlib.h>
#include <time.h>
#include "raylib.h"
#include "../headers/game.h"

int main() {
    const int screenWidth = 450;
    const int screenHeight = 450;
    gameState gameState;
    int squareSize = 30;
    snake snake = {0, 0, 0, RIGHT, RIGHT};
    gameState.status = PAUSED;

    InitWindow(screenWidth, screenHeight, "Snake Game");

    gameState.grid[snake.x][snake.y] = '.';
    addFood(&gameState);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

            ClearBackground(RAYWHITE);

            for (int i = 0; i < 15; ++i) {
                for (int j = 0; j < 15; ++j) {
                    DrawRectangle(i * squareSize, j * squareSize, squareSize, squareSize, (i + j) % 2 == 0 ? GRAY : LIGHTGRAY);
                }
            }

            DrawRectangle(gameState.foodX * squareSize, gameState.foodY * squareSize, squareSize, squareSize, DARKBLUE);
            DrawRectangle(snake.x * squareSize, snake.y * squareSize, squareSize, squareSize, gameState.status == GAMEOVER ? RED : BLACK);

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

    return 0;
}

void addFood(gameState *gameState)
{
    srandom(time(NULL));
    int x = (random() % (14 - 0 + 1)) + 0;
    int y = (random() % (14 - 0 + 1)) + 0;

    while (gameState->grid[x][y] == '.') {
        x = (random() % (14 - 0 + 1)) + 0;
        y = (random() % (14 - 0 + 1)) + 0;
    }

    gameState->foodX = x;
    gameState->foodY = y;
    gameState->grid[x][y] = '-';
}

void restart(gameState *gameState, snake *snake) {
    gameState->status = ONGOING;
    snake->x = 0;
    snake->y = 0;
    snake->steps = 0;
    snake->nextDirection = RIGHT;
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            gameState->grid[i][j] = ' ';
        }
    }
    gameState->grid[snake->x][snake->y] = '.';
    addFood(gameState);
}

void move_snake(gameState *gameState, snake *snake)
{
    if (snake->steps < 10) {
        snake->steps += 1;
    } else {
        gameState->grid[snake->x][snake->y] = ' ';

        if (gameState->status != GAMEOVER) snake->direction = snake->nextDirection;

        if (snake->direction == UP) {
            if (snake->y - 1 < 0) gameState->status = GAMEOVER;
            else snake->y -= 1;
        } else if (snake->direction == RIGHT) {
            if (snake->x + 1 >= 15) gameState->status = GAMEOVER;
            else snake->x += 1;
        } else if (snake->direction == DOWN) {
            if (snake->y + 1 >= 15) gameState->status = GAMEOVER;
            else snake->y += 1;
        } else if (snake->direction == LEFT) {
            if (snake->x - 1 < 0) gameState->status = GAMEOVER;
            else snake->x -= 1;
        }

        snake->steps = 0;
        if (gameState->grid[snake->x][snake->y] == '-') addFood(gameState);
        gameState->grid[snake->x][snake->y] = '.';
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

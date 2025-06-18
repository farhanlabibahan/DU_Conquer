#include "raylib.h"
#include "raymath.h"
#include "global.h"

#define MAX_SEGMENTS 100

typedef struct Worm {
    Vector2 position[MAX_SEGMENTS];
    int size;
} Worm;

Worm worm = {0};
Vector2 currentDirection;
Vector2 nextDirection;
Vector2 food;
Texture2D background, foodImage, wormHead, wormBody;
bool gameOver = false;


const int screenWidthg = 800;
const int screenHeightg = 600;
const int cellSize = 40;
const float step = cellSize * 0.55f;

void init_snake() {
    background = LoadTexture("resources/bg.jpg");
    foodImage = LoadTexture("resources/food1.png");
    wormHead = LoadTexture("resources/wormhead1.png");
    wormBody = LoadTexture("resources/wormbody.png");

    worm.size = 2;
    Vector2 headStart = (Vector2){screenWidthg / 2, screenHeightg / 2};
    worm.position[0] = headStart;
    currentDirection = (Vector2){step, 0};
    nextDirection = currentDirection;

    for (int i = 1; i < worm.size; i++) {
        worm.position[i] = Vector2Subtract(worm.position[i - 1], currentDirection);
    }

    food = (Vector2){
        (float)(rand() % (screenWidthg / cellSize)) * cellSize,
        (float)(rand() % (screenHeightg / cellSize)) * cellSize
    };

    gameOver = false;
    game_win_microbiology2 = false;
}

void logic_snake() {
    if (worm.size > 15) game_win_microbiology2 = true;

    if (!gameOver) {
        if (IsKeyPressed(KEY_UP) && currentDirection.y == 0) nextDirection = (Vector2){0, -step};
        if (IsKeyPressed(KEY_DOWN) && currentDirection.y == 0) nextDirection = (Vector2){0, step};
        if (IsKeyPressed(KEY_LEFT) && currentDirection.x == 0) nextDirection = (Vector2){-step, 0};
        if (IsKeyPressed(KEY_RIGHT) && currentDirection.x == 0) nextDirection = (Vector2){step, 0};

        for (int i = worm.size - 1; i > 0; i--) worm.position[i] = worm.position[i - 1];
        worm.position[0] = Vector2Add(worm.position[0], currentDirection);
        currentDirection = nextDirection;

        if (worm.position[0].x < 0 || worm.position[0].x >= screenWidthg ||
            worm.position[0].y < 0 || worm.position[0].y >= screenHeightg)
            gameOver = true;

        for (int i = 1; i < worm.size; i++) {
            if (Vector2Distance(worm.position[0], worm.position[i]) < step / 2)
                gameOver = true;
        }

        Rectangle headRect = { worm.position[0].x, worm.position[0].y, cellSize, cellSize };
        Rectangle foodRect = { food.x, food.y, cellSize, cellSize };

        Vector2 headCenter = { headRect.x + cellSize / 2.0f, headRect.y + cellSize / 2.0f };
        Vector2 foodCenter = { foodRect.x + cellSize / 2.0f, foodRect.y + cellSize / 2.0f };
        if (Vector2Distance(headCenter, foodCenter) < cellSize / 2.0f) {
            if (worm.size < MAX_SEGMENTS) {
                worm.position[worm.size] = worm.position[worm.size - 1];
                worm.size++;
            }
            food = (Vector2){
                (float)(rand() % (screenWidthg / cellSize)) * cellSize,
                (float)(rand() % (screenHeightg / cellSize)) * cellSize
            };
        }
    }

    if (gameOver && IsKeyPressed(KEY_R)) {
        init_snake();
    }
}

void draw_snake() {
    DrawTexture(background, 0, 0, WHITE);
    DrawTexture(foodImage, food.x, food.y, WHITE);

    for (int i = 0; i < worm.size; i++) {
        Vector2 drawPos = worm.position[i];
        Texture2D segmentTexture = (i == 0) ? wormHead : wormBody;

        Rectangle destRect = {
            drawPos.x + cellSize / 2.0f,
            drawPos.y + cellSize / 2.0f,
            cellSize,
            cellSize
        };

        Vector2 origin = {
            segmentTexture.width / 2.0f,
            segmentTexture.height / 2.0f
        };

        DrawTexturePro(
            segmentTexture,
            (Rectangle){0, 0, segmentTexture.width, segmentTexture.height},
            destRect,
            origin,
            0.0f,
            WHITE
        );
    }

    DrawText(TextFormat("Score: %d", worm.size), 10, 10, 20, DARKGRAY);

    if (gameOver) {
        DrawText("GAME OVER! Press R to restart.", screenWidthg / 2 - 230, screenHeightg / 2, 30, RED);
    }
}

void unload_snake() {
    UnloadTexture(background);
    UnloadTexture(foodImage);
    UnloadTexture(wormHead);
    UnloadTexture(wormBody);
}

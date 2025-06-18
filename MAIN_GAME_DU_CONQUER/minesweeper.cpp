
// Modularized Minesweeper - using 4 functions
#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include "minesweeper.h"
#include "global.h"
#include <math.h>

#define TILE_SIZE 24
#define GRID_WIDTH 16
#define GRID_HEIGHT 16
#define NUM_MINES 40
#define HEADER_HEIGHT 80
#define MAX_LIVES 3
#define MAX_TIME 300

#define VIRTUAL_WIDTH (GRID_WIDTH * TILE_SIZE)
#define VIRTUAL_HEIGHT (GRID_HEIGHT * TILE_SIZE + HEADER_HEIGHT)

// Game State
typedef enum { STATE_SPLASH, STATE_PLAYING, STATE_END } GameState;
GameState gameState = STATE_SPLASH;
bool firstClick = true;
bool gameWon = false;
int flagsLeft = NUM_MINES;
int livesLeft = MAX_LIVES;
float gameTimer = 0, gameStartTime = 0;

// Grid
typedef struct {
    bool revealed;
    bool flagged;
    bool hasMine;
    int adjacentMines;
} Tile;
Tile grid[GRID_HEIGHT][GRID_WIDTH];

// Assets
Texture2D bombTexture, flagTexture, splashTexture;
RenderTexture2D target;

// Directions
const int dx[8] = {-1,-1,-1,0,1,1,1,0};
const int dy[8] = {-1,0,1,1,1,0,-1,-1};

void PlaceMines(int avoidX, int avoidY) {
    int placed = 0;
    while (placed < NUM_MINES) {
        int x = rand() % GRID_WIDTH;
        int y = rand() % GRID_HEIGHT;
        if ((x == avoidX && y == avoidY) || grid[y][x].hasMine) continue;
        bool near = false;
        for (int i = 0; i < 8; i++) {
            int nx = avoidX + dx[i], ny = avoidY + dy[i];
            if (x == nx && y == ny) near = true;
        }
        if (near) continue;
        grid[y][x].hasMine = true;
        placed++;
    }
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (grid[y][x].hasMine) continue;
            int count = 0;
            for (int i = 0; i < 8; i++) {
                int nx = x + dx[i], ny = y + dy[i];
                if (nx >= 0 && ny >= 0 && nx < GRID_WIDTH && ny < GRID_HEIGHT && grid[ny][nx].hasMine)
                    count++;
            }
            grid[y][x].adjacentMines = count;
        }
    }
}

void Reveal(int x, int y) {
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT) return;
    Tile *tile = &grid[y][x];
    if (tile->revealed || tile->flagged) return;
    tile->revealed = true;
    if (tile->hasMine) {
        livesLeft--;
        if (livesLeft <= 0) gameState = STATE_END;
        return;
    }
    if (tile->adjacentMines == 0) {
        for (int i = 0; i < 8; i++) Reveal(x + dx[i], y + dy[i]);
    }
}

void CheckWinCondition() {
    int revealedCount = 0;
    for (int y = 0; y < GRID_HEIGHT; y++)
        for (int x = 0; x < GRID_WIDTH; x++)
            if (!grid[y][x].hasMine && grid[y][x].revealed) revealedCount++;
    if (revealedCount == GRID_WIDTH * GRID_HEIGHT - NUM_MINES) {
        gameWon = true;
        nuclear_game = true;
        gameState = STATE_END;
    }
}

void init_minesweeper() {
    srand(time(NULL));
    SetWindowPosition((screenWidth - VIRTUAL_WIDTH) / 2, (screenHeight - VIRTUAL_HEIGHT) / 2);

    bombTexture = LoadTexture("resources/explosion.png");
    flagTexture = LoadTexture("resources/flag.png");
    splashTexture = LoadTexture("resources/splash_bomb.png");
    target = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    flagsLeft = NUM_MINES;
    livesLeft = MAX_LIVES;
    firstClick = true;
    gameTimer = 0;
    gameStartTime = 0;
    gameWon = false;
    gameState = STATE_SPLASH;
    nuclear_game = false;

    for (int y = 0; y < GRID_HEIGHT; y++)
        for (int x = 0; x < GRID_WIDTH; x++)
            grid[y][x] = (Tile){0};
}

void logic_minesweeper() {
    if (gameState == STATE_PLAYING) {
        gameTimer = GetTime() - gameStartTime;
        if (gameTimer >= MAX_TIME) gameState = STATE_END;

        float scaleX = (float)screenWidth / VIRTUAL_WIDTH;
        float scaleY = (float)screenHeight / VIRTUAL_HEIGHT;
        float scale = fminf(scaleX, scaleY);
        float offsetX = (screenWidth - VIRTUAL_WIDTH * scale) / 2.0f;
        float offsetY = (screenHeight - VIRTUAL_HEIGHT * scale) / 2.0f;

        Vector2 mouse = GetMousePosition();
        int x = (mouse.x - offsetX) / scale / TILE_SIZE;
        int y = (mouse.y - offsetY - HEADER_HEIGHT * scale) / scale / TILE_SIZE;

        if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))) {
            if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
                if (firstClick) {
                    PlaceMines(x, y);
                    firstClick = false;
                }
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    Reveal(x, y);
                    CheckWinCondition();
                } else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                    Tile *tile = &grid[y][x];
                    if (!tile->revealed) {
                        tile->flagged = !tile->flagged;
                        flagsLeft += tile->flagged ? -1 : 1;
                    }
                }
            }
        }
    }

    if ((gameState == STATE_SPLASH || gameState == STATE_END) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        gameState = STATE_PLAYING;
        gameStartTime = GetTime();
        firstClick = true;
        for (int y = 0; y < GRID_HEIGHT; y++)
            for (int x = 0; x < GRID_WIDTH; x++)
                grid[y][x] = (Tile){0};
        flagsLeft = NUM_MINES;
        livesLeft = MAX_LIVES;
        gameWon = false;
        nuclear_game = false;
    }
}

void draw_minesweeper() {
    BeginTextureMode(target);
    EndTextureMode();

    float scaleX = (float)screenWidth / VIRTUAL_WIDTH;
    float scaleY = (float)screenHeight / VIRTUAL_HEIGHT;
    float scale = fminf(scaleX, scaleY);

    DrawTexturePro(target.texture,
        (Rectangle){0, 0, (float)VIRTUAL_WIDTH, -(float)VIRTUAL_HEIGHT},
        (Rectangle){(screenWidth - VIRTUAL_WIDTH * scale) / 2, (screenHeight - VIRTUAL_HEIGHT * scale) / 2,
                    VIRTUAL_WIDTH * scale, VIRTUAL_HEIGHT * scale},
        (Vector2){0, 0}, 0.0f, WHITE);
}

void unload_minesweeper() {
    UnloadTexture(bombTexture);
    UnloadTexture(flagTexture);
    UnloadTexture(splashTexture);
    UnloadRenderTexture(target);
}

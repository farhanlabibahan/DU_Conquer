#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include "minesweeper.h"
#include "global.h"
#include <math.h>

#define GRID_WIDTH 16
#define GRID_HEIGHT 16
#define NUM_MINES 40
#define HEADER_HEIGHT 80
#define MAX_LIVES 3
#define MAX_TIME 300

#define VIRTUAL_WIDTH 600
#define VIRTUAL_HEIGHT 800

typedef enum { STATE_SPLASH, STATE_PLAYING, STATE_END } vGameState_nuclear;
vGameState_nuclear GameState_nuclear = STATE_SPLASH;

bool firstClick = true;
bool gameWon = false;
int flagsLeft = NUM_MINES;
int livesLeft = MAX_LIVES;
float gameTimer = 0, gameStartTime = 0;

typedef struct {
    bool revealed;
    bool flagged;
    bool hasMine;
    int adjacentMines;
} Tile;
Tile grid[GRID_HEIGHT][GRID_WIDTH];

Texture2D bombTexture, flagTexture, splashTexture;
RenderTexture2D target;

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
        if (livesLeft <= 0) GameState_nuclear = STATE_END;
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
        GameState_nuclear = STATE_END;
    }
}

void init_minesweeper() {
    srand(time(NULL));

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
    GameState_nuclear = STATE_SPLASH;
    nuclear_game = false;

    for (int y = 0; y < GRID_HEIGHT; y++)
        for (int x = 0; x < GRID_WIDTH; x++)
            grid[y][x] = (Tile){0};
}

void logic_minesweeper() {
    float gridScale = fminf((float)VIRTUAL_WIDTH / GRID_WIDTH, (float)(VIRTUAL_HEIGHT - HEADER_HEIGHT) / GRID_HEIGHT);
    float offsetX = (screenWidth - VIRTUAL_WIDTH) / 2.0f + (VIRTUAL_WIDTH - GRID_WIDTH * gridScale) / 2.0f;
    float offsetY = (screenHeight - VIRTUAL_HEIGHT) / 2.0f + HEADER_HEIGHT;

    if (GameState_nuclear == STATE_PLAYING) {
        gameTimer = GetTime() - gameStartTime;
        if (gameTimer >= MAX_TIME) GameState_nuclear = STATE_END;

        Vector2 mouse = GetMousePosition();
        int x = (mouse.x - offsetX) / gridScale;
        int y = (mouse.y - offsetY) / gridScale;

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

    if ((GameState_nuclear == STATE_SPLASH || GameState_nuclear == STATE_END) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        GameState_nuclear = STATE_PLAYING;
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
    ClearBackground(DARKGRAY);

    float gridScale = fminf((float)VIRTUAL_WIDTH / GRID_WIDTH, (float)(VIRTUAL_HEIGHT - HEADER_HEIGHT) / GRID_HEIGHT);

    if (GameState_nuclear == STATE_SPLASH) {
        int splashWidth = 400;
        int splashHeight = 400;
        int splashX = (VIRTUAL_WIDTH - splashWidth) / 2;
        int splashY = (VIRTUAL_HEIGHT - splashHeight) / 2;

        Rectangle dest = { (float)splashX, (float)splashY, (float)splashWidth, (float)splashHeight };
        Rectangle src = { 0, 0, (float)splashTexture.width, (float)splashTexture.height };

        DrawRectangleRec(dest, Fade(BLACK, 0.6f));
        DrawTexturePro(splashTexture, src, dest, (Vector2){0, 0}, 0, WHITE);
        DrawText("Click to Start", VIRTUAL_WIDTH / 2 - 90, splashY + splashHeight + 20, 20, WHITE);
    } else {
        float startX = (VIRTUAL_WIDTH - GRID_WIDTH * gridScale) / 2.0f;
        float startY = HEADER_HEIGHT;

        float scaleBomb = gridScale / bombTexture.width;
        float scaleFlag = gridScale / flagTexture.width;

        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                float px = startX + x * gridScale;
                float py = startY + y * gridScale;
                Rectangle rect = { px, py, gridScale, gridScale };

                if (grid[y][x].revealed) {
                    DrawRectangleRec(rect, LIGHTGRAY);
                    if (grid[y][x].hasMine)
                        DrawTextureEx(bombTexture, (Vector2){px, py}, 0, scaleBomb, WHITE);
                    else if (grid[y][x].adjacentMines > 0)
                        DrawText(TextFormat("%d", grid[y][x].adjacentMines), px + gridScale * 0.3f, py + gridScale * 0.2f, (int)(gridScale * 0.5f), BLUE);
                } else {
                    DrawRectangleRec(rect, GRAY);
                    if (grid[y][x].flagged)
                        DrawTextureEx(flagTexture, (Vector2){px, py}, 0, scaleFlag, WHITE);
                }

                DrawRectangleLinesEx(rect, 1, DARKGRAY);
            }
        }

        DrawText(TextFormat("Lives: %d", livesLeft), 10, 10, 20, RED);
        DrawText(TextFormat("Flags Left: %d", flagsLeft), 200, 10, 20, YELLOW);
        DrawText(TextFormat("Time: %.1fs", gameTimer), 420, 10, 20, LIGHTGRAY);
    }

    EndTextureMode();

    Rectangle src = { 0, 0, (float)VIRTUAL_WIDTH, -(float)VIRTUAL_HEIGHT };
    Rectangle dest = {
        (screenWidth - VIRTUAL_WIDTH) / 2,
        (screenHeight - VIRTUAL_HEIGHT) / 2,
        (float)VIRTUAL_WIDTH,
        (float)VIRTUAL_HEIGHT
    };

    DrawTexturePro(target.texture, src, dest, (Vector2){0, 0}, 0, WHITE);
}

void unload_minesweeper() {
    UnloadTexture(bombTexture);
    UnloadTexture(flagTexture);
    UnloadTexture(splashTexture);
    UnloadRenderTexture(target);
}

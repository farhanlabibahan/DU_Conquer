#include "raylib.h"
#include <math.h>
#include "puzzle.h"
#include "global.h"

#define GRID_SIZE 6
#define TILE_SIZE 128
#define ROTATION_STATES 4

typedef struct {
    int rotation;        // 0, 1, 2, 3 => 0°, 90°, 180°, 270°
    int targetRotation;  // Always 0
} Tile;

static Tile grid[GRID_SIZE][GRID_SIZE];

static Texture2D original;
static RenderTexture2D scaled;
static bool gamewon = false;

const int puzzleWidth = TILE_SIZE * GRID_SIZE;
const int puzzleHeight = TILE_SIZE * GRID_SIZE;

// ------------------ INIT ------------------
void init_puzzle() {
    original = LoadTexture("resources/EEE.png");
    scaled = LoadRenderTexture(puzzleWidth, puzzleHeight);

    BeginTextureMode(scaled);
        // ClearBackground(BLACK);
        DrawTexturePro(
            original,
            (Rectangle){ 0, 0, (float)original.width, (float)original.height },
            (Rectangle){ 0, 0, (float)puzzleWidth, (float)puzzleHeight },
            (Vector2){ 0, 0 },
            0.0f,
            WHITE
        );
    EndTextureMode();

    UnloadTexture(original); // Unload original after drawing into scaled

    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            grid[y][x].rotation = GetRandomValue(0, 3);
            grid[y][x].targetRotation = 0;
        }
    }

    gamewon = false;
}

// ------------------ LOGIC ------------------
void logic_puzzle() {
    int popupX = (screenWidth - puzzleWidth) / 2;
    int popupY = (screenHeight - puzzleHeight) / 2;

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mouse = GetMousePosition();

        // Adjust mouse position to local puzzle area
        int x = (mouse.x - popupX) / TILE_SIZE;
        int y = (mouse.y - popupY) / TILE_SIZE;

        if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
            grid[y][x].rotation = (grid[y][x].rotation + 1) % ROTATION_STATES;
        }
    }

    if (IsKeyPressed(KEY_R)) {
        for (int y = 0; y < GRID_SIZE; y++) {
            for (int x = 0; x < GRID_SIZE; x++) {
                grid[y][x].rotation = GetRandomValue(0, 3);
            }
        }
        gamewon = false;
    }
  if (gamewon){
    cctv_game=true;
  }
    // Check win condition
    gamewon = true;
    for (int y = 0; y < GRID_SIZE && gamewon; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (grid[y][x].rotation != grid[y][x].targetRotation) {
                gamewon = false;
                break;
            }
        }
    }
}


// ------------------ DRAW ------------------
void draw_puzzle() {
    int popupX = (screenWidth- puzzleWidth) / 2;
    int popupY = (screenHeight - puzzleHeight) / 2;

    // DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.6f)); // dim background

    for (int y = 0; y < GRID_SIZE; y++) {
        for (int x = 0; x < GRID_SIZE; x++) {
            Rectangle src = {
                x * TILE_SIZE,
                (puzzleHeight - (y + 1) * TILE_SIZE),
                TILE_SIZE,
                -TILE_SIZE
            };

            Rectangle dest = {
                popupX + x * TILE_SIZE + TILE_SIZE / 2,
                popupY + y * TILE_SIZE + TILE_SIZE / 2,
                TILE_SIZE,
                TILE_SIZE
            };

            DrawTexturePro(
                scaled.texture,
                src,
                dest,
                (Vector2){ TILE_SIZE / 2, TILE_SIZE / 2 },
                grid[y][x].rotation * 90.0f,
                WHITE
            );

            // Color borderColor = (grid[y][x].rotation == grid[y][x].targetRotation)
            //     ? GREEN
            //     : Fade(WHITE, 0.2f);

            // DrawRectangleLines(popupX + x * TILE_SIZE, popupY + y * TILE_SIZE, TILE_SIZE, TILE_SIZE, borderColor);
        }
    }

    if (gamewon) {
        DrawText("Puzzle Solved!", popupX/ 2 - 150, popupY + 20, 40, GREEN);
        
    }

    DrawText("Press R to Reset Puzzle", popupX + 20, popupY + puzzleHeight - 30, 20, GRAY);
}


// ------------------ UNLOAD ------------------
void unload_puzzle() {
    UnloadRenderTexture(scaled);
}

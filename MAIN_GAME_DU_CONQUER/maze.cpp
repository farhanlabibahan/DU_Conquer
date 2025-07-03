#include "raylib.h"
#include "global.h"
#include "raymath.h"
#include "maze.h"
#include <vector>
#include <chrono>

using namespace std;

#define MAZE_WIDTH 20
#define MAZE_HEIGHT 20
#define WALL_SIZE 2.0f
#define PLAYER_RADIUS 0.3f
#define PLAYER_HEIGHT 1.8f


struct Wall {
    Vector3 position;
    BoundingBox bounds;
};

enum GameState_maze { INTRO, PLAYING, WON, LOST };

Camera camera_maze = {0};
Camera orbitalcamera_maze = {0};
vector<Wall> mazeWalls;
GameState_maze GameState_maze = INTRO;
float introTimer = 15.0f;
float elapsedTime = 0.0f;
chrono::time_point<chrono::steady_clock> startTime;
Color nightSky, groundColor, wallColor, wallEdgeColor, exitColor;

const int starsCount = 200;
Vector2 stars[starsCount];

bool maze_cursor_disabled = false;

bool CheckCollision(Vector3 playerPos, const vector<Wall>& walls, float radius) {
    BoundingBox playerBox = {
        {playerPos.x - radius, playerPos.y - PLAYER_HEIGHT/2, playerPos.z - radius},
        {playerPos.x + radius, playerPos.y + PLAYER_HEIGHT/2, playerPos.z + radius}
    };

    for (const Wall& wall : walls) {
        if (CheckCollisionBoxes(playerBox, wall.bounds)) {
            return true;
        }
    }
    return false;
}

bool CheckWinCondition(Vector3 playerPos) {
    return playerPos.x >= (MAZE_WIDTH-2)*WALL_SIZE && 
           playerPos.z >= (MAZE_HEIGHT-2)*WALL_SIZE;
}


void init_maze() {


    startTime = chrono::steady_clock::now();

    int maze[MAZE_HEIGHT][MAZE_WIDTH] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,1},
        {1,1,1,0,1,0,1,1,1,0,1,0,1,0,1,0,1,1,0,1},
        {1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,1},
        {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1},
        {1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1},
        {1,0,0,0,0,0,0,1,0,0,0,0,0,1,0,1,0,1,0,1},
        {1,0,1,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,0,1},
        {1,0,1,0,0,1,0,0,0,0,0,1,0,1,0,1,0,0,0,1},
        {1,0,1,0,1,1,1,1,1,1,0,1,0,1,0,1,1,1,1,1},
        {1,0,0,0,1,0,0,0,0,1,0,1,0,1,0,0,0,0,0,1},
        {1,1,1,1,1,0,1,1,0,1,0,1,0,1,1,1,1,1,0,1},
        {1,0,0,0,0,0,1,0,0,1,0,1,0,0,0,0,0,1,0,1},
        {1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,0,1},
        {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1},
        {1,0,1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,1,0,1},
        {1,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1},
        {1,0,0,0,1,0,1,1,1,1,1,1,0,1,1,1,0,1,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            if (maze[y][x] == 1) {
                Vector3 pos = {x * WALL_SIZE, WALL_SIZE/2, y * WALL_SIZE};
                Wall wall;
                wall.position = pos;
                wall.bounds = {
                    {pos.x - WALL_SIZE/2, 0.0f, pos.z - WALL_SIZE/2},
                    {pos.x + WALL_SIZE/2, WALL_SIZE, pos.z + WALL_SIZE/2}
                };
                mazeWalls.push_back(wall);
            }
        }
    }

    Vector3 playerStartPos = {WALL_SIZE + 0.5f, PLAYER_HEIGHT/2, WALL_SIZE + 0.5f};
    camera_maze.position = playerStartPos;
    camera_maze.target = (Vector3){camera_maze.position.x, camera_maze.position.y, camera_maze.position.z - 1.0f};
    camera_maze.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera_maze.fovy = 60.0f;
    camera_maze.projection = CAMERA_PERSPECTIVE;

    orbitalcamera_maze.position = (Vector3){MAZE_WIDTH * WALL_SIZE/2, MAZE_WIDTH * WALL_SIZE, MAZE_HEIGHT * WALL_SIZE/2};
    orbitalcamera_maze.target = (Vector3){MAZE_WIDTH * WALL_SIZE/2, 0.0f, MAZE_HEIGHT * WALL_SIZE/2};
    orbitalcamera_maze.up = (Vector3){0.0f, 1.0f, 0.0f};
    orbitalcamera_maze.fovy = 45.0f;
    orbitalcamera_maze.projection = CAMERA_PERSPECTIVE;

    nightSky = { 15, 15, 40, 255 };
    groundColor = { 5, 20, 5, 255 };
    wallColor = { 50, 50, 60, 155 };
    wallEdgeColor = { 80, 80, 90, 200 };
    exitColor = { 100, 80, 50, 255 };

    for (int i = 0; i < starsCount; i++) {
        stars[i] = {(float)GetRandomValue(0, GetScreenWidth()), (float)GetRandomValue(0, GetScreenHeight())};
    }
}

void logic_draw_maze() {
    float deltaTime = GetFrameTime();
    auto currentTime = chrono::steady_clock::now();
    elapsedTime = chrono::duration<float>(currentTime - startTime).count();

    if (GameState_maze == INTRO) {
        introTimer -= deltaTime;
        if (introTimer <= 0) {
            GameState_maze = PLAYING;
            startTime = currentTime;
        }
    } else if (GameState_maze == PLAYING) {
        if (!maze_cursor_disabled) {
            DisableCursor();
            maze_cursor_disabled = true;
        }
        if (CheckWinCondition(camera_maze.position)) {
            GameState_maze = WON;
            maze_game = true;
            state_of_game = LAYER_MAP;
            EnableCursor();
            // PlaySound(conquered);
        }
    }

    if (GameState_maze == PLAYING) {
        Vector3 oldPos = camera_maze.position;
        Vector3 oldTarget = camera_maze.target;

        UpdateCamera(&camera_maze, CAMERA_FIRST_PERSON);

        if (CheckCollision(camera_maze.position, mazeWalls, PLAYER_RADIUS)) {
            camera_maze.position = oldPos;
            camera_maze.target = oldTarget;
        }
    } else if (GameState_maze == INTRO) {
        orbitalcamera_maze.position.y = MAZE_WIDTH * WALL_SIZE * (0.5f + 0.5f * sin(introTimer));
        orbitalcamera_maze.position.x = MAZE_WIDTH * WALL_SIZE/2 + MAZE_WIDTH * WALL_SIZE * 0.5f * cos(introTimer * 0.5f);
        UpdateCamera(&orbitalcamera_maze, CAMERA_ORBITAL);
    }

    

    for (int i = 0; i < starsCount; i++) {
        float brightness = (sin(elapsedTime + stars[i].x*0.1f) + 1.0f) * 0.5f;
        Color starColor = {255, 255, 255, (unsigned char)(brightness * 255)};
        DrawPixelV(stars[i], starColor);
    }

    BeginMode3D(GameState_maze == INTRO ? orbitalcamera_maze : camera_maze);

    DrawPlane((Vector3){MAZE_WIDTH * WALL_SIZE/2, 0, MAZE_HEIGHT * WALL_SIZE/2}, 
              (Vector2){MAZE_WIDTH * WALL_SIZE, MAZE_HEIGHT * WALL_SIZE}, groundColor);

    for (const Wall& wall : mazeWalls) {
        DrawCube(wall.position, WALL_SIZE, WALL_SIZE, WALL_SIZE, wallColor);
        DrawCubeWires(wall.position, WALL_SIZE, WALL_SIZE, WALL_SIZE, wallEdgeColor);
    }

    if (GameState_maze == PLAYING) {
        float pulse = (sin(elapsedTime * 3.0f) + 1.0f) * 0.2f + 0.6f;
        Color pulseExitColor = {exitColor.r, exitColor.g, exitColor.b, (unsigned char)(exitColor.a * pulse)};
        Vector3 exitPos = {(MAZE_WIDTH-1.5f)*WALL_SIZE, 0.1f, (MAZE_HEIGHT-1.5f)*WALL_SIZE};
        DrawCube(exitPos, WALL_SIZE, 0.2f, WALL_SIZE, pulseExitColor);
    }

    EndMode3D();

    if (GameState_maze == INTRO) {
        DrawText(TextFormat("Starting in %.1f...", introTimer), 350, 300, 20, RED);
    } else if (GameState_maze == WON) {
        DrawText("ESCAPED THE MAZE!", 250, 250, 40, exitColor);
        DrawText("Press ESC to exit", 320, 350, 20, RED);
    }



}

void unload_maze() {
    CloseWindow();
}
// map2.cpp
#include "map.h"
#include "raylib.h"
#include <math.h>
#include <vector>
#include <fstream>
#include "global.h"
#include "loading.h"

static Music bgm;

Texture2D map_texture;
Texture2D character_map;
Image map_image_data;
Camera2D camera;

float speed = 3.0f;
float theta = 26.2f * DEG2RAD;
float cosTheta = cosf(theta);
float sinTheta = sinf(theta);

struct IsoObstacle {
    Vector2 topLeft;
    Vector2 topRight;
    Vector2 bottomRight;
    Vector2 bottomLeft;
};

std::vector<IsoObstacle> isoObstacles;
std::vector<Vector2> currentClickPoints;
int obstacleCount = 0;

// Add a rectangular isometric obstacle
void add_isometric_obstacle(Vector2 topLeft, Vector2 topRight, Vector2 bottomRight, Vector2 bottomLeft) {
    IsoObstacle quad = { topLeft, topRight, bottomRight, bottomLeft };
    isoObstacles.push_back(quad);
}

// Ray-casting point-in-quad check
bool pointInQuad(Vector2 p, const IsoObstacle& q) {
    int count = 0;
    Vector2 corners[4] = { q.topLeft, q.topRight, q.bottomRight, q.bottomLeft };

    for (int i = 0; i < 4; ++i) {
        Vector2 a = corners[i];
        Vector2 b = corners[(i + 1) % 4];
        if (((a.y > p.y) != (b.y > p.y)) &&
            (p.x < (b.x - a.x) * (p.y - a.y) / (b.y - a.y + 0.0001f) + a.x)) {
            count++;
        }
    }
    return (count % 2 == 1);
}

bool is_colliding(Vector2 point) {
    for (const auto& quad : isoObstacles) {
        if (pointInQuad(point, quad)) return true;
    }
    return false;
}

void init_map() {
    map_texture = LoadTexture("resources/map2.png");
    character_map = LoadTexture("resources/character.png");
    map_image_data = LoadImage("resources/map2.png");
    bgm = LoadMusicStream("resources/nature.wav");

    SetMusicVolume(bgm, 0.4f);
    PlayMusicStream(bgm);

    camera.zoom = 1.0f;

    // Optional: pre-define an obstacle
    // add_isometric_obstacle({7214, 3047}, {8214, 3047}, {8214, 4047}, {7214, 4047});
}

void logic_draw_map() {
    camera.target = (Vector2){ x_co_ordinate, y_co_ordinate };
    camera.offset = (Vector2){ screenWidth / 2, screenHeight / 2 };

    UpdateMusicStream(bgm);

    // State transitions
    if (IsKeyPressed(KEY_X)) state_of_game = LAYER_MENU;
    if (IsKeyPressed(KEY_E)) state_of_game = LAYER_EEE;
    if (IsKeyPressed(KEY_N)) state_of_game = LAYER_NUCLEAR;
    if (IsKeyPressed(KEY_G)) state_of_game = LAYER_GENETICS;
    if (IsKeyPressed(KEY_M)) state_of_game = LAYER_MICROBIOLOGY;
    if (IsKeyPressed(KEY_J)) state_of_game = LAYER_JAPANESSE;
    if (IsKeyPressed(KEY_R)) state_of_game = LAYER_EEE_FIRST;

    Vector2 proposed = { x_co_ordinate, y_co_ordinate };

    if (IsKeyDown(KEY_W)) {
        proposed.x -= cosTheta * speed;
        proposed.y -= sinTheta * speed;
    }
    if (IsKeyDown(KEY_S)) {
        proposed.x += cosTheta * speed;
        proposed.y += sinTheta * speed;
    }
    if (IsKeyDown(KEY_A)) {
        proposed.x -= cosTheta * speed;
        proposed.y += sinTheta * speed;
    }
    if (IsKeyDown(KEY_D)) {
        proposed.x += cosTheta * speed;
        proposed.y -= sinTheta * speed;
    }

    // Check collision
    if (!is_colliding(proposed)) {
        x_co_ordinate = proposed.x;
        y_co_ordinate = proposed.y;
    }

    // Click to mark 4 corners and save
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 worldClick = GetScreenToWorld2D(GetMousePosition(), camera);
        currentClickPoints.push_back(worldClick);

        if (currentClickPoints.size() == 4) {
            add_isometric_obstacle(
                currentClickPoints[0],
                currentClickPoints[1],
                currentClickPoints[2],
                currentClickPoints[3]
            );

            // Save to file
            std::ofstream file("obstacles_saved.txt", std::ios::app);
            if (file.is_open()) {
                file << "add_isometric_obstacle(\n";
                for (int i = 0; i < 4; i++) {
                    file << "    (Vector2){" << currentClickPoints[i].x << "f, " << currentClickPoints[i].y << "f}";
                    if (i < 3) file << ",\n";
                    else file << "\n";
                }
                file << ");\n\n";
                file.close();
            }

            currentClickPoints.clear();
            obstacleCount++;
        }
    }

    // Rendering
    BeginMode2D(camera);
    DrawTexture(map_texture, 0, 0, WHITE);

    // Draw character
    DrawTextureEx(character_map,
        (Vector2){
            x_co_ordinate - (character_map.width * 0.25f) / 2,
            y_co_ordinate - (character_map.height * 0.25f) / 2
        },
        0.0f, 0.25f, WHITE);

    // Draw all obstacles (fill + red outline)
    for (const auto& quad : isoObstacles) {
        DrawTriangle(quad.topLeft, quad.topRight, quad.bottomRight, Fade(RED, 0.3f));
        DrawTriangle(quad.bottomRight, quad.bottomLeft, quad.topLeft, Fade(RED, 0.3f));

        DrawLineV(quad.topLeft, quad.topRight, RED);
        DrawLineV(quad.topRight, quad.bottomRight, RED);
        DrawLineV(quad.bottomRight, quad.bottomLeft, RED);
        DrawLineV(quad.bottomLeft, quad.topLeft, RED);
    }

    // Draw points you've clicked so far (green circles)
    for (const Vector2& p : currentClickPoints) {
        DrawCircleV(p, 6, GREEN);
    }

    EndMode2D();

    // UI info
    DrawText(TextFormat("Position: X: %.1f Y: %.1f", x_co_ordinate, y_co_ordinate), 10, 70, 20, RED);

    Color pixelColor = GetImageColor(map_image_data, x_co_ordinate, y_co_ordinate);
    DrawText(TextFormat("Pixel Color: R: %d G: %d B: %d",
        pixelColor.r, pixelColor.g, pixelColor.b), 10, 100, 20, RED);

    DrawText(TextFormat("Obstacles: %d", obstacleCount), 10, 130, 20, RED);

    loading_screen("Dhaka University Map");
}

void unload_map() {
    UnloadTexture(map_texture);
    UnloadTexture(character_map);
    UnloadMusicStream(bgm);
    UnloadImage(map_image_data);
}

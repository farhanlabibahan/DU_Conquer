#include "raylib.h"
#include <stdio.h>
#include <math.h>
#include "map.h"

double x_co_ordinate=1;
double y_co_ordinate=1;

// ----------------- STRUCTS ------------------
typedef struct Zone {
    Rectangle rect;
    const char* name;
    Color color;
} Zone;

// ----------------- GLOBAL VARIABLES ------------------
Vector2 character_pos = {4523, 2873};

bool mapOpen = true;

float speed = 4.0f;
Camera2D camera = {0};

Image img;
Texture2D background;
Texture2D character;

Zone zones[] = {
    {{100, 100, 200, 150}, "Lava Zone", RED},
    {{400, 300, 180, 120}, "Safe Zone", GREEN},
    {{700, 500, 150, 100}, "Trigger Zone", ORANGE}
};
int zoneCount = sizeof(zones) / sizeof(zones[0]);

float theta = 26.2f * DEG2RAD;
float cosTheta, sinTheta;

// ----------------- INIT FUNCTION ------------------
void init_map() {
    // InitWindow(1200, 800, "Image with Camera and Zones");
    // SetTargetFPS(60);

    img = LoadImage("map2.png");
    background = LoadTextureFromImage(img);
    character = LoadTexture("character.png");
    UnloadImage(img);

    camera.target = character_pos;
    camera.offset = (Vector2){ GetScreenWidth() / 2, GetScreenHeight() / 2 };
    camera.zoom = 1.0f;

    cosTheta = cosf(theta);
    sinTheta = sinf(theta);
}

// ----------------- UNLOAD FUNCTION ------------------
void unload_map() {
    UnloadTexture(background);
    UnloadTexture(character);
    CloseWindow();
}

// ----------------- MAIN LOGIC + DRAWING + LOOP ------------------
void logic_draw_map() {
    while (mapOpen) {
        Vector2 move = {0};

        if (IsKeyPressed(KEY_ESCAPE)) 
        {
            mapOpen = false;
            break;
        }


        if (IsKeyDown(KEY_W)) {
            character_pos.x -= cosTheta * speed;
            character_pos.y -= sinTheta * speed;
        }
        if (IsKeyDown(KEY_S)) {
            character_pos.x += cosTheta * speed;
            character_pos.y += sinTheta * speed;
        }
        if (IsKeyDown(KEY_A)) {
            character_pos.x -= cosTheta * speed;
            character_pos.y += sinTheta * speed;
        }
        if (IsKeyDown(KEY_D)) {
            character_pos.x += cosTheta * speed;
            character_pos.y -= sinTheta * speed;
        }

        x_co_ordinate = character_pos.x;
        y_co_ordinate = character_pos.y;

        camera.target = character_pos;

        const char* currentZone = "None";
        for (int i = 0; i < zoneCount; i++) {
            if (CheckCollisionPointRec(character_pos, zones[i].rect)) {
                currentZone = zones[i].name;
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);

        DrawTexture(background, 0, 0, WHITE);
        DrawRectangle(0, 0, background.width, background.height, Fade(BLACK, 0.3f));

        for (int i = 0; i < zoneCount; i++) {
            DrawRectangleRec(zones[i].rect, Fade(zones[i].color, 0.3f));
            DrawRectangleLinesEx(zones[i].rect, 2, zones[i].color);
            DrawText(zones[i].name, zones[i].rect.x + 4, zones[i].rect.y + 4, 10, zones[i].color);
        }

        float scale = 0.25f;
        int layers = 20;
        float baseRadius = (character.width * scale) * 0.11f;

        for (int i = 0; i < layers; i++) {
            float radius = baseRadius * (1.0f + i * 0.15f);
            float alpha = 0.25f * (1.0f - (float)i / layers);
            DrawCircleV(character_pos, radius, Fade((Color){255, 200, 90, 255}, alpha));
        }

        DrawTextureEx(character,
            (Vector2){ character_pos.x - (character.width * scale) / 2, character_pos.y - (character.height * scale) / 2 },
            0.0f,
            scale,
            WHITE);

        EndMode2D();

        DrawText(TextFormat("Current Zone: %s", currentZone), 10, 10, 20, YELLOW);
        DrawText(TextFormat("X: %f Y: %f", character_pos.x, character_pos.y), 10, 70, 30, RED);
        DrawFPS(10, 40);
        EndDrawing();
    }
    unload_map();
}





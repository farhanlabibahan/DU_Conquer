#include "map.h"
#include "raylib.h"
#include <math.h>
#include <vector>
#include <fstream>
#include "global.h"
#include "loading.h"
#include "obstacle.h"

static Music bgm;

Texture2D map_texture;
Texture2D character_map;
Image map_image_data;
Camera2D camera;

float speed_map = 3.0f;
float theta_map = 26.2f * DEG2RAD;
float costheta_map = cosf(theta_map);
float sintheta_map = sinf(theta_map);

struct IsoObstacle {
    Vector2 topLeft;
    Vector2 topRight;
    Vector2 bottomRight;
    Vector2 bottomLeft;
};


void init_map() {
    LoadIsometricObstacles();
    map_texture = LoadTexture("resources/DUCONQUERMAP.png");
    character_map = LoadTexture("resources/character.png");
    map_image_data = LoadImage("resources/DUCONQUERMAP.png");
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
        proposed.x -= costheta_map * speed_map;
        proposed.y -= sintheta_map * speed_map;
    }
    if (IsKeyDown(KEY_S)) {
        proposed.x += costheta_map * speed_map;
        proposed.y += sintheta_map * speed_map;
    }
    if (IsKeyDown(KEY_A)) {
        proposed.x -= costheta_map * speed_map;
        proposed.y += sintheta_map * speed_map;
    }
    if (IsKeyDown(KEY_D)) {
        proposed.x += costheta_map * speed_map;
        proposed.y -= sintheta_map * speed_map;
    }

    // Check collision
    if (!is_colliding(proposed.x,proposed.y)) {
        x_co_ordinate = proposed.x;
        y_co_ordinate = proposed.y;
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


    EndMode2D();

    // UI info
    DrawText(TextFormat("Position: X: %.1f Y: %.1f", x_co_ordinate, y_co_ordinate), 10, 70, 20, RED);

    loading_screen("Dhaka University Map");
}

void unload_map() {
    UnloadTexture(map_texture);
    UnloadTexture(character_map);
    UnloadMusicStream(bgm);
    UnloadImage(map_image_data);
}

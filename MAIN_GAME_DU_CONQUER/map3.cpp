#include "map.h"
#include "raylib.h"
#include <math.h>
#include <vector>
#include <fstream>
#include "global.h"
#include "loading.h"
#include "character_map.h"
#include "obstacle.h"
static Music bgm;

Texture2D map_texture;
Texture2D character_map;
Image map_image_data;
Camera2D camera;

void init_map() {
    map_texture = LoadTexture("resources/DUCONQUERMAP.png");
    character_map = LoadTexture("resources/character.png");

    map_image_data = LoadImage("resources/DUCONQUERMAP.png");
    bgm = LoadMusicStream("resources/nature.wav");

    SetMusicVolume(bgm, 0.4f);
    PlayMusicStream(bgm);

    camera.zoom = 1.0f;
    LoadIsometricObstacles();
}

void logic_draw_map() {
    camera.target = (Vector2){ x_co_ordinate, y_co_ordinate };
    camera.offset = (Vector2){ screenWidth / 2, screenHeight / 2 };

    UpdateMusicStream(bgm);

    // Handle key presses for game state changes
    if(IsKeyPressed(KEY_X)) state_of_game = LAYER_MENU;
    if(IsKeyPressed(KEY_E)) state_of_game = LAYER_EEE;
    if(IsKeyPressed(KEY_N)) state_of_game = LAYER_NUCLEAR;
    if(IsKeyPressed(KEY_G)) state_of_game = LAYER_GENETICS;
    if(IsKeyPressed(KEY_M)) state_of_game = LAYER_MICROBIOLOGY;
    if(IsKeyPressed(KEY_J)) state_of_game = LAYER_JAPANESSE;
    if(IsKeyPressed(KEY_R)) state_of_game = LAYER_EEE_FIRST;
    if(IsKeyPressed(KEY_V)) state_of_game = LAYER_VC_BUNGLAW;
    if(IsKeyPressed(KEY_L)) state_of_game = LAYER_LIFT;
    if(IsKeyPressed(KEY_C)) state_of_game = LAYER_CCTV;
    if(IsKeyPressed(KEY_Q)) state_of_game = LAYER_MAZE;
    if(IsKeyPressed(KEY_B)) state_of_game = LAYER_BANK;
    if(IsKeyPressed(KEY_I)) state_of_game = LAYER_IBA;

    Vector2 offset_map = walk_character_map();
    float prev_x = x_co_ordinate;
    float prev_y = y_co_ordinate;
    Vector2 proposed = { x_co_ordinate + offset_map.x, y_co_ordinate + offset_map.y };

    if (is_colliding(x_co_ordinate,y_co_ordinate)) {
        x_co_ordinate = prev_x;
        y_co_ordinate = prev_y;
    }
    else 
    {
        x_co_ordinate = proposed.x;
        y_co_ordinate = proposed.y;
    }

    BeginMode2D(camera);
    DrawTexture(map_texture, 0, 0, WHITE);
    draw_char_map((Vector2){ x_co_ordinate, y_co_ordinate });

    DrawCircle(x_co_ordinate, y_co_ordinate, 10.0f, RED);

    EndMode2D();

    // UI info
    DrawText(TextFormat("Position: X: %.1f Y: %.1f", x_co_ordinate, y_co_ordinate), 10, 70, 20, WHITE);


    loading_screen("Dhaka University Map");
    }

    void unload_map() {
    UnloadTexture(map_texture);
    UnloadTexture(character_map);
    UnloadMusicStream(bgm);
    UnloadImage(map_image_data);
}
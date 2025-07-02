// map.c
#include "map.h"
#include "raylib.h"
#include <math.h>
#include "global.h"
#include "loading.h"

static Music bgm;

// Global variables
Texture2D map_texture;
Texture2D character_map;
Image map_image_data;
Camera2D camera;
float speed = 3.0f;
float theta = 26.2f * DEG2RAD;
float cosTheta = cosf(theta);
float sinTheta = sinf(theta);

void init_map()
{
    // Load resources
    map_texture = LoadTexture("resources/map2.png");
    character_map = LoadTexture("resources/character.png");
    map_image_data = LoadImage("resources/map2.png");
    bgm = LoadMusicStream("resources/nature.wav");
    
    // Setup music
    SetMusicVolume(bgm, 0.4f);  
    PlayMusicStream(bgm);
    
    // Initialize camera
    camera.zoom = 1.0f;
}

void logic_draw_map()
{
    // Update camera position
    camera.target = (Vector2){x_co_ordinate, y_co_ordinate};
    camera.offset = (Vector2){ screenWidth / 2, screenHeight / 2 };
    
    // Update music stream
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

    // Handle movement
    if (IsKeyDown(KEY_W)) {
        x_co_ordinate -= cosTheta * speed;
        y_co_ordinate -= sinTheta * speed;
    }
    if (IsKeyDown(KEY_S)) {
        x_co_ordinate += cosTheta * speed;
        y_co_ordinate += sinTheta * speed;
    }
    if (IsKeyDown(KEY_A)) {
        x_co_ordinate -= cosTheta * speed;
        y_co_ordinate += sinTheta * speed;
    }
    if (IsKeyDown(KEY_D)) {
        x_co_ordinate += cosTheta * speed;
        y_co_ordinate -= sinTheta * speed;
    }

    // Begin 2D drawing with camera
    BeginMode2D(camera);
    
    // Draw map texture
    DrawTexture(map_texture, 0, 0, WHITE);
    
    // Draw character
    DrawTextureEx(character_map,
        (Vector2){ 
            x_co_ordinate - (character_map.width * scale) / 2, 
            y_co_ordinate - (character_map.height * scale) / 2 
        },
        0.0f,
        0.25f,
        WHITE);

    EndMode2D();

    // Debug information
    DrawText(TextFormat("Position: X: %.1f Y: %.1f", x_co_ordinate, y_co_ordinate), 10, 70, 20, RED);
    
    // Get and display pixel color (for debugging)
    Color pixelColor = GetImageColor(map_image_data, x_co_ordinate, y_co_ordinate);
    DrawText(TextFormat("Pixel Color: R: %d G: %d B: %d", 
        pixelColor.r, pixelColor.g, pixelColor.b), 10, 100, 20, RED);
    
    // Draw loading screen
    loading_screen("Dhaka University Map");
}

void unload_map()
{
    // Unload resources
    UnloadTexture(map_texture);
    UnloadTexture(character_map);
    UnloadMusicStream(bgm);
    UnloadImage(map_image_data);
}
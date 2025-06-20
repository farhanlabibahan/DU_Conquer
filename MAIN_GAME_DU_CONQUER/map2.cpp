#include "map.h"
#include "raylib.h"
#include <math.h>
#include "global.h"
#include "loading.h"

static Music bgm;

Texture2D map_image,character_map;
Camera2D camera;
float speed = 3.0f;
float theta = 26.2f * DEG2RAD;
float cosTheta = cosf(theta);
float sinTheta = sinf(theta);

void init_map()
{
    map_image = LoadTexture("resources/map2.png");
    character_map = LoadTexture("resources/character.png");

    bgm = LoadMusicStream("resources/nature.wav");
    SetMusicVolume(bgm, 0.4f);  
    PlayMusicStream(bgm);
}

void logic_draw_map()
{
    camera.target = (Vector2){x_co_ordinate,y_co_ordinate};
    camera.offset = (Vector2){ screenWidth / 2, screenHeight / 2 };
    camera.zoom = 1.0f;

    UpdateMusicStream(bgm);

    if(IsKeyPressed(KEY_X))
    {
        state_of_game = LAYER_MENU;
    }
    if(IsKeyPressed(KEY_E))
    {
        state_of_game = LAYER_EEE;
    }
    if(IsKeyPressed(KEY_N))
    {
        state_of_game = LAYER_NUCLEAR;
    }
    if(IsKeyPressed(KEY_G))
    {
        state_of_game = LAYER_GENETICS;
    }

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


        // Draw

        BeginMode2D(camera);

        DrawTexture(map_image, 0, 0, WHITE);
        DrawTextureEx(character_map,
            (Vector2){ x_co_ordinate - (character_map.width * scale) / 2, y_co_ordinate - (character_map.height * scale) / 2 },
            0.0f,
            scale*0.21,
            WHITE);

        EndMode2D();

        // text
        DrawText(TextFormat("X: %f Y: %f",x_co_ordinate, y_co_ordinate), 10, 70, 30, RED);
        DrawFPS(10, 40);


    loading_screen("Dhaka University Map");


}

void unload_map()
{
    if (map_image.id > 0) {
        UnloadTexture(map_image);
        map_image.id = 0; 
    }
    if (character_map.id > 0) {
        UnloadTexture(character_map);
        character_map.id = 0;
    }
    UnloadMusicStream(bgm);
}

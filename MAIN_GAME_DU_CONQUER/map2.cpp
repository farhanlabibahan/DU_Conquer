#include "map.h"
#include "raylib.h"
#include <math.h>
#include <vector>
#include <fstream>
#include <string>
#include "global.h"
#include "loading.h"
#include "character_map.h"
#include "obstacle.h"
using namespace std;

Vector2 mokarram = {4554,2575.0};
Vector2 eee_building = {6853.3,2376.3};
Vector2 nuclear_building = {6196.5,2702.1};
Vector2 ducsu_building = {2051.0,1632.0};
Vector2 phylosophy_building = {2554.4,1882.3};
Vector2 japanesse_building = {2672.8,2006.8};
Vector2 bank_building = {5706.6,2726.0};
Vector2 iba_building = {2406.3,1777.7};
Vector2 law_building = {1932.6,1602.8};
Vector2 cctv_building = {4367.5,2695.4};
Vector2 mall_chattar_maze = {2217.9,1115.4};
Vector2 vc_building = {1133.1,1667.7};



static Music bgm;
string toShowTextMap = "HELLO TEST";

Texture2D map_texture;
Texture2D character_map;
Image map_image_data;
Camera2D camera;





void init_map() {
    map_texture = LoadTexture("resources/map_last.png");
    character_map = LoadTexture("resources/character.png");

    map_image_data = LoadImage("resources/map_last.png");
    bgm = LoadMusicStream("resources/nature.wav");

    SetMusicVolume(bgm, 0.4f);
    PlayMusicStream(bgm);

    camera.zoom = 1.0f;
    LoadIsometricObstacles();

}

void logic_draw_map() {
    camera.target = (Vector2){ x_co_ordinate, y_co_ordinate };
    camera.offset = (Vector2){ screenWidth / 2, screenHeight / 2 };
    toShowTextMap = "";
    UpdateMusicStream(bgm);


    // // Handle key presses for game state changes
    // if(IsKeyPressed(KEY_X)) state_of_game = LAYER_MENU;
    // if(IsKeyPressed(KEY_E)) state_of_game = LAYER_EEE;
    // if(IsKeyPressed(KEY_N)) state_of_game = LAYER_NUCLEAR;
    // if(IsKeyPressed(KEY_G)) state_of_game = LAYER_GENETICS;
    // if(IsKeyPressed(KEY_M)) state_of_game = LAYER_MICROBIOLOGY;
    // if(IsKeyPressed(KEY_J)) state_of_game = LAYER_JAPANESSE;
    // if(IsKeyPressed(KEY_R)) state_of_game = LAYER_EEE_FIRST;
    // if(IsKeyPressed(KEY_V)) state_of_game = LAYER_VC_BUNGLAW;
    // if(IsKeyPressed(KEY_L)) state_of_game = LAYER_LIFT;
    // if(IsKeyPressed(KEY_C)) state_of_game = LAYER_CCTV;
    // if(IsKeyPressed(KEY_Q)) state_of_game = LAYER_MAZE;
    // if(IsKeyPressed(KEY_B)) state_of_game = LAYER_BANK;
    // if(IsKeyPressed(KEY_I)) state_of_game = LAYER_IBA;
    // if(IsKeyPressed(KEY_K)) state_of_game = LAYER_DUCSU;
    // if(IsKeyPressed(KEY_P)) state_of_game = LAYER_LAW;
    // if(IsKeyPressed(KEY_O)) state_of_game = LAYER_ROBOTICS;


    Vector2 offset = walk_character_map();

    Vector2 proposed = {
        x_co_ordinate + offset.x,
        y_co_ordinate + offset.y
    };

    if (!is_colliding(proposed.x+30,proposed.y+40)) {
        x_co_ordinate = proposed.x;
        y_co_ordinate = proposed.y;
    }

    Vector2 character_pos_map = {x_co_ordinate,y_co_ordinate};
    if (CheckCollisionCircles(character_pos_map, 50.0f, mokarram, 50.0f)) {
        toShowTextMap = "Press E to enter Mokarram Sc Building";
        if(IsKeyPressed(KEY_E))
        {
            state_of_game = LAYER_LIFT;
        }
    }
    else if (CheckCollisionCircles(character_pos_map, 50.0f, eee_building, 50.0f)) {
        toShowTextMap = "Press E to enter Mokarram Sc Building";
        if(IsKeyPressed(KEY_E))
        {
            if(!eee_game_1) state_of_game = LAYER_EEE_FIRST;
            else state_of_game = LAYER_EEE;
        }
    }

    else if (CheckCollisionCircles(character_pos_map, 50.0f, nuclear_building, 50.0f)) {
        toShowTextMap = "Press E to enter Nuclear Dept";
        if(IsKeyPressed(KEY_E))
        {
            state_of_game = LAYER_NUCLEAR;
        }
    }

    else if (CheckCollisionCircles(character_pos_map, 50.0f, ducsu_building, 50.0f)) {
        toShowTextMap = "Press E to enter DUCSU";
        if(IsKeyPressed(KEY_E))
        {
            state_of_game = LAYER_DUCSU;
        }
    }

    // else if (CheckCollisionCircles(character_pos_map, 50.0f, phylosophy_building, 50.0f)) {
    //     toShowTextMap = "Press E to enter Mokarram Sc Building";
    //     if(IsKeyPressed(KEY_E))
    //     {
    //         state_of_game = LAYER_DUCSU;
    //     }
    // }

    else if (CheckCollisionCircles(character_pos_map, 50.0f, japanesse_building, 50.0f)) {
        toShowTextMap = "Press E to enter Japanese Dept";
        if(IsKeyPressed(KEY_E))
        {
            state_of_game = LAYER_JAPANESSE;
        }
    }

    else if (CheckCollisionCircles(character_pos_map, 50.0f, bank_building, 50.0f)) {
        toShowTextMap = "Press E to enter Rh BANK";
        if(IsKeyPressed(KEY_E))
        {
            state_of_game = LAYER_BANK;
        }
    }

    else if (CheckCollisionCircles(character_pos_map, 50.0f, iba_building, 50.0f)) {
        toShowTextMap = "Press E to enter IBA";
        if(IsKeyPressed(KEY_E))
        {
            state_of_game = LAYER_IBA;
        }
    }

    else if (CheckCollisionCircles(character_pos_map, 50.0f, law_building, 50.0f)) {
        toShowTextMap = "Press E to enter LAW Dept";
        if(IsKeyPressed(KEY_E))
        {
            state_of_game = LAYER_LAW;
        }
    }

    else if (CheckCollisionCircles(character_pos_map, 50.0f, cctv_building, 50.0f)) {
        if(eee_game_1)
        {  
            toShowTextMap = "Press E to enter ICT CELL";
            if(IsKeyPressed(KEY_E))
            {
                state_of_game = LAYER_CCTV;
            }
    }
    }

    else if (CheckCollisionCircles(character_pos_map, 50.0f, mall_chattar_maze, 50.0f)) {
        toShowTextMap = "Press E to enter MAZE!";
        if(IsKeyPressed(KEY_E))
        {
            state_of_game = LAYER_MAZE;
        }
    }

    else if (CheckCollisionCircles(character_pos_map, 50.0f, vc_building, 50.0f)) {
        toShowTextMap = "Press E to enter VC Bunglaw";
        if(IsKeyPressed(KEY_E))
        {
            state_of_game = LAYER_VC_BUNGLAW;
        }
    }

    else if (CheckCollisionCircles(character_pos_map, 50.0f, cctv_building, 50.0f) && eee_game_1) {
        toShowTextMap = "Press E to enter VC Bunglaw";
        if(IsKeyPressed(KEY_E))
        {
            state_of_game = LAYER_CCTV;
        }
    }

    BeginMode2D(camera);
    DrawTexture(map_texture, 0, 0, WHITE);
    draw_char_map(proposed);
    
    Vector2 offset_map = walk_character_map();
    proposed.x += offset_map.x;


    // DrawCircle(x_co_ordinate+30,y_co_ordinate+40,20.0f,RED);


    EndMode2D();

    DrawText(TextFormat("Position: X: %.1f Y: %.1f", x_co_ordinate, y_co_ordinate), 10, 70, 20, WHITE);
    DrawText(toShowTextMap.c_str(),20,screenHeight-60*scale,15,YELLOW);

    loading_screen("Dhaka University Map");
}

void unload_map() {
    UnloadTexture(map_texture);
    UnloadTexture(character_map);
    UnloadMusicStream(bgm);
    UnloadImage(map_image_data);
}

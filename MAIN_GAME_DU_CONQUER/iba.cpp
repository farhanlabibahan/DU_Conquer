
#include "raylib.h"
#include<iostream>
#include <string>
#include "loading.h"
#include "iba.h"
#include "global.h"
 #include "business.h"
using namespace std;



dept_state_iba dept_status_iba = Dept_iba;

Texture2D bg_image_iba;
Camera2D camera_iba = {0};
Vector2 playerPos_iba;
Vector2 pop_up_zone,game_zone_iba;
Vector2 exit_zone_iba;
string pop_up_iba = "Find a Investment!";
string game_pop_up_iba ="" ;
string game_rules_iba = "Startup Game!\nChoose a business, a strategy, a plan, and an execution method.";

bool walk_music_playing_iba = false;
bool show_rules_popup_iba = false;
bool show_ok_button_iba = false;
bool canpop=false;

void init_iba() {
    SetMusicVolume(bgm_iba, 0.13f);
    PlayMusicStream(bgm_iba);
    SetMusicVolume(walk_music, 1.0f);
    bg_image_iba = LoadTexture("iba.png");
    scale = (float)GetMonitorHeight(0) / bg_image_iba.height;
    float y_pos_floor = (float)GetMonitorHeight(0) - scale * 350;
   // playerPos_iba = (Vector2){-10, y_pos_floor};
    playerPos_iba = {2200*scale,GetMonitorHeight(0)-400*scale};
    exit_zone_iba = {2200*scale,GetMonitorHeight(0)-400*scale};
    pop_up_zone = {1950*scale,GetMonitorHeight(0)-400*scale};
    game_zone_iba ={ 2400*scale,GetMonitorHeight(0)-400*scale};                                     


    camera_iba.target = playerPos_iba;
    camera_iba.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera_iba.rotation = 0.0f;
    camera_iba.zoom = 1.0f;

    init_business();
    game_rules_iba = "Solve the startup challenge!\nPick a model, test a plan, and simulate execution.";
}

void unload_iba() {
    unload_business();
    UnloadTexture(bg_image_iba);
}

void logic_draw_iba() {
    scale = (float)GetMonitorHeight(0) / bg_image_iba.height;
    UpdateMusicStream(bgm_iba);
    if (!iba_game) pop_up_iba = "Find and Solve the Clue";

    if (dept_status_iba == Dept_iba) {
        // bool moving = false;
        // if (IsKeyDown(KEY_A)) { playerPos_iba.x -= 13; moving = true; }
        // if (IsKeyDown(KEY_D)) { playerPos_iba.x += 13; moving = true; }

        Vector2 offset_iba = walk_character_dept();
        playerPos_iba.x += offset_iba.x;

        if (moving && !walk_music_playing_iba) {
            PlayMusicStream(walk_music);
            walk_music_playing_iba = true;
        } else if (!moving && walk_music_playing_iba) {
            StopMusicStream(walk_music);
            walk_music_playing_iba = false;
        }
        if (walk_music_playing_iba) UpdateMusicStream(walk_music);

        bool eKeyHandled = false;
        // CheckCollisionPointCircle(mousePos, point, pointRadius);
        if (CheckCollisionPointCircle(GetScreenToWorld2D(GetMousePosition(), camera_iba), pop_up_zone, 100.0f)) {
            // pop_up_iba = "Press E to Solve"; // Visual feedback
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)&& !iba_game && !show_rules_popup_iba) {
                PlaySound(pop_up_sound);
                PlaySound(click_sound);
                show_rules_popup_iba = true;
                show_ok_button_iba = true;
            }
        }
        if (CheckCollisionPointCircle(GetScreenToWorld2D(GetMousePosition(), camera_iba), game_zone_iba, 100.0f)) {
            // pop_up_iba = "Press E to Solve"; // Visual feedback
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)&& show_ok_button_iba && show_rules_popup_iba) {
                canpop =true;
            }
        }
        if(canpop){
             dept_status_iba=Game_iba;
        }
        if (CheckCollisionCircles(playerPos_iba, 50.0f, exit_zone_iba, 50.0f)) {
            pop_up_iba = "Press E to Exit";
            if (IsKeyPressed(KEY_E)) {
                PlaySound(click_sound);
                //  unload_business();
                eKeyHandled = true;
                state_of_game = LAYER_MAP;
            } else if (IsKeyPressed(KEY_E) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(error_sound);
            }
        }
        

        if (IsKeyPressed(KEY_E) && !eKeyHandled) {
            PlaySound(error_sound);
        }
    } else if (dept_status_iba == Game_iba) {
        logic_business();
        if (iba_game || IsKeyDown(KEY_X)) {
            dept_status_iba = Dept_iba;
            game_pop_up_iba = "IBA Conquered!";
            iba_game = true;
            PlaySound(conquered_sound);
        }
        // Removed KEY_Q close logic, replaced by visual close button below.
    }

    if (playerPos_iba.x <= -20) playerPos_iba.x = -20;
    else if (playerPos_iba.x >= bg_image_iba.width * scale) playerPos_iba.x = bg_image_iba.width * scale;

    scale = (float)GetMonitorHeight(0) / bg_image_iba.height;
    float scaledWidth = bg_image_iba.width * scale;
    float cam = playerPos_iba.x;
    float halfScreen = screenWidth / 2.0f;
    if (cam < halfScreen) cam = halfScreen;
    if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;
    camera_iba.target = (Vector2){ cam, screenHeight / 2.0f };

    BeginMode2D(camera_iba);
    scale = (float)GetMonitorHeight(0) / bg_image_iba.height;
    DrawTextureEx(bg_image_iba, (Vector2){0, 0}, 0.0f, scale, WHITE);
    // DrawTexture(character, playerPos_iba.x, playerPos_iba.y, WHITE);
    draw_char_dept(playerPos_iba,scale);
   
    // cout<<"japposx "<<playerPos_iba.x<<endl;
    // cout<<"japposy "<<playerPos_iba.y<<endl;
    // DrawCircleV(pop_up_zone, 20, RED);
    // DrawCircleV(exit_zone_iba, 20, GREEN);
    // DrawCircleV(playerPos_iba, 20, BLUE);
    EndMode2D();

    if (dept_status_iba == Game_iba) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    }

    if (dept_status_iba == Game_iba) {
        draw_business();
    }

    // Visual close button in Game_iba mode
    if (dept_status_iba == Game_iba) {
        Rectangle closeBtn = { (float)(screenWidth - 60), 20, 40, 40 };
        Vector2 mouse = GetMousePosition();
        Color btnColor = CheckCollisionPointRec(mouse, closeBtn) ? RED : DARKGRAY;

        DrawRectangleRec(closeBtn, btnColor);
        DrawText("X", closeBtn.x + 12, closeBtn.y + 8, 20, WHITE);

        if (CheckCollisionPointRec(mouse, closeBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(pop_up_sound);
            dept_status_iba = Dept_iba;
        }
    }

    DrawText(game_pop_up_iba.c_str(), 20, screenHeight - 100, 10, GREEN);

    if (iba_game && CheckCollisionCircles(playerPos_iba, 50.0f, exit_zone_iba, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!iba_game) {
        DrawText(pop_up_iba.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    if (show_rules_popup_iba) {
        int screenW = screenWidth;
        int screenH = screenHeight;

        DrawRectangle(screenW / 2 - 220, screenH / 2 - 100, 440, 200, Fade(BLACK, 0.9f));
        DrawRectangleLines(screenW / 2 - 220, screenH / 2 - 100, 440, 200, LIGHTGRAY);

        DrawText(game_rules_iba.c_str(), screenW / 2 - MeasureText(game_rules_iba.c_str(), 15) / 2, screenH / 2 - 60, 15, RAYWHITE);

        Rectangle okBtn = { screenW / 2 - 50, screenH / 2 + 30, 100, 40 };
        Vector2 mouse = GetMousePosition();
        Color btnColor = CheckCollisionPointRec(mouse, okBtn) ? RED : DARKGRAY;
        static bool wasHovering = false;
        bool isHovering = CheckCollisionPointRec(mouse, okBtn);
        if (isHovering && !wasHovering) {
            PlaySound(click_sound);
        }
        wasHovering = isHovering;
        DrawRectangleRec(okBtn, btnColor);
        DrawText("OK", screenW / 2 - MeasureText("OK", 20) / 2, screenH / 2 + 40, 20, WHITE);

        if (show_ok_button_iba && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, okBtn)) {
                PlaySound(click_sound);
                dept_status_iba = Game_iba;
                show_rules_popup_iba = false;
                show_ok_button_iba = false;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(error_sound);
        }
    }

    loading_screen("Department Of iba\nUniversity of Dhaka");
}

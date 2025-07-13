#include "raylib.h"
#include<iostream>
#include <string>
#include "loading.h"
#include "cctv.h"
#include "global.h"
#include "puzzle.h"
using namespace std;

typedef enum {
    Dept_cctv,
    Game_cctv
} dept_state_cctv;

dept_state_cctv dept_status_cctv = Dept_cctv;
Texture2D bg_image_cctv;
Camera2D camera_cctv = {0};
Vector2 playerPos_cctv;
Vector2 game_zone_cctv;
Vector2 exit_zone_cctv;
string pop_up_cctv = "Find and Solve the Clue";
string game_pop_up_cctv = " ";
string game_rules_cctv = "All you need to do\nis to rotate!";
bool walk_music_playing_cctv = false;
bool show_rules_popup_cctv = false;
bool show_ok_button_cctv = false;

void init_cctv() {
    SetMusicVolume(bgm_cctv, 0.13f);
    PlayMusicStream(bgm_cctv);
    SetMusicVolume(walk_music, 1.0f);
    bg_image_cctv = LoadTexture("resources/cctv_ict cell_final.png");
    scale = (float)GetScreenHeight() / bg_image_cctv.height;
    float y_pos_floor = (float)GetScreenHeight() - scale * 350;
   // playerPos_cctv = (Vector2){-10, y_pos_floor};
    playerPos_cctv = {2200*scale,GetScreenHeight()-400*scale};
    exit_zone_cctv = {2200*scale,GetScreenHeight()-400*scale};
    game_zone_cctv = {2500*scale,GetScreenHeight()-700*scale};

    camera_cctv.target = playerPos_cctv;
    camera_cctv.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera_cctv.rotation = 0.0f;
    camera_cctv.zoom = 1.0f;

    init_puzzle();
}

void unload_cctv() {
    unload_puzzle();
    UnloadTexture(bg_image_cctv);
}

void logic_draw_cctv() {
    UpdateMusicStream(bgm_cctv);
    if (!cctv_game) pop_up_cctv = "Revive the CCTV Footage";

    if (dept_status_cctv == Dept_cctv) {
        // bool moving = false;
        // if (IsKeyDown(KEY_A)) { playerPos_cctv.x -= 13; moving = true; }
        // if (IsKeyDown(KEY_D)) { playerPos_cctv.x += 13; moving = true; }

        Vector2 offset_cctv = walk_character_dept();
        playerPos_cctv.x += offset_cctv.x;

        if (moving && !walk_music_playing_cctv) {
            PlayMusicStream(walk_music);
            walk_music_playing_cctv = true;
        } else if (!moving && walk_music_playing_cctv) {
            StopMusicStream(walk_music);
            walk_music_playing_cctv = false;
        }
        if (walk_music_playing_cctv) UpdateMusicStream(walk_music);

        bool eKeyHandled = false;
        // CheckCollisionPointCircle(mousePos, point, pointRadius);
        if (CheckCollisionPointCircle(GetScreenToWorld2D(GetMousePosition(), camera_cctv), game_zone_cctv, 100.0f)) {
            // pop_up_cctv = "Press E to Solve"; // Visual feedback
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)&& !cctv_game && !show_rules_popup_cctv) {
                PlaySound(pop_up_sound);
                PlaySound(click_sound);
                show_rules_popup_cctv = true;
                show_ok_button_cctv = true;
            }
        }

        if (CheckCollisionCircles(playerPos_cctv, 50.0f, exit_zone_cctv, 50.0f)) {
            pop_up_cctv = "Press E to Exit";
            if (IsKeyPressed(KEY_E)) {
                PlaySound(click_sound);
                unload_puzzle();
                eKeyHandled = true;
                state_of_game = LAYER_MAP;
            } else if (IsKeyPressed(KEY_E) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(error_sound);
            }
        }

        if (IsKeyPressed(KEY_E) && !eKeyHandled) {
            PlaySound(error_sound);
        }
    } else if (dept_status_cctv == Game_cctv) {
        logic_puzzle();
        if (cctv_game || IsKeyDown(KEY_X)) {
            dept_status_cctv = Dept_cctv;
            game_pop_up_cctv = "EEE NOT CONQUERED! SABOTAGE!!";
            cctv_game = true;
            PlaySound(conquered_sound);
            unload_puzzle();
        }
        if (IsKeyDown(KEY_Q)) {
            unload_puzzle();
            dept_status_cctv = Dept_cctv;
        }
    }

    if (playerPos_cctv.x <= -20) playerPos_cctv.x = -20;
    else if (playerPos_cctv.x >= bg_image_cctv.width * scale) playerPos_cctv.x = bg_image_cctv.width * scale;

    scale = (float)GetScreenHeight() / bg_image_cctv.height;
    float scaledWidth = bg_image_cctv.width * scale;
    float cam = playerPos_cctv.x;
    float halfScreen = screenWidth / 2.0f;
    if (cam < halfScreen) cam = halfScreen;
    if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;
    camera_cctv.target = (Vector2){ cam, screenHeight / 2.0f };

    BeginMode2D(camera_cctv);
    scale = (float)GetScreenHeight() / bg_image_cctv.height;
    DrawTextureEx(bg_image_cctv, (Vector2){0, 0}, 0.0f, scale, WHITE);
    // DrawTexture(character, playerPos_cctv.x, playerPos_cctv.y, WHITE);
    draw_char_dept(playerPos_cctv,scale);
   
    // cout<<"japposx "<<playerPos_cctv.x<<endl;
    // cout<<"japposy "<<playerPos_cctv.y<<endl;
    // DrawCircleV(game_zone_cctv, 20, RED);
    // DrawCircleV(exit_zone_cctv, 20, GREEN);
    // DrawCircleV(playerPos_cctv, 20, BLUE);
    EndMode2D();

    if (dept_status_cctv == Game_cctv) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    }

    if (dept_status_cctv == Game_cctv) {
        draw_puzzle();
    }

    DrawText(game_pop_up_cctv.c_str(), 20, screenHeight - 100, 20, RED);

    if (cctv_game && CheckCollisionCircles(playerPos_cctv, 50.0f, exit_zone_cctv, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!cctv_game) {
        DrawText(pop_up_cctv.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    if (show_rules_popup_cctv) {
        int screenW = screenWidth;
        int screenH = screenHeight;

        DrawRectangle(screenW / 2 - 220, screenH / 2 - 100, 440, 200, Fade(BLACK, 0.9f));
        DrawRectangleLines(screenW / 2 - 220, screenH / 2 - 100, 440, 200, LIGHTGRAY);

        DrawText(game_rules_cctv.c_str(), screenW / 2 - MeasureText(game_rules_cctv.c_str(), 20) / 2, screenH / 2 - 60, 20, RAYWHITE);

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

        if (show_ok_button_cctv && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, okBtn)) {
                PlaySound(click_sound);
                dept_status_cctv = Game_cctv;
                show_rules_popup_cctv = false;
                show_ok_button_cctv = false;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(error_sound);
        }
    }

    loading_screen("Department Of cctv\nUniversity of Dhaka");
}

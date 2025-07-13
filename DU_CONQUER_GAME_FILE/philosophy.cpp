#include "raylib.h"
#include<iostream>
#include <string>
#include "loading.h"
#include "philosophy.h"
#include "global.h"
#include "tictactoe.h"
using namespace std;

typedef enum {
    Dept_philosophy,
    Game_philosophy
} dept_state_philosophy;

dept_state_philosophy dept_status_philosophy = Dept_philosophy;
Texture2D bg_image_philosophy;
Camera2D camera_philosophy = {0};
Vector2 playerPos_philosophy;
Vector2 game_zone_philosophy;
Vector2 exit_zone_philosophy;
string pop_up_philosophy = "Find and Solve the Clue";
string game_pop_up_philosophy = " ";
string game_rules_philosophy = "Buckle up for some memory exercise";
bool walk_music_playing_philosophy = false;
bool show_rules_popup_philosophy = false;
bool show_ok_button_philosophy = false;

void init_philosophy() {
    SetMusicVolume(bgm_philosophy, 0.13f);
    PlayMusicStream(bgm_philosophy);
    SetMusicVolume(walk_music, 1.0f);
    bg_image_philosophy = LoadTexture("resources/Philosophy.png");
    scale = (float)GetScreenHeight() / bg_image_philosophy.height;
    float y_pos_floor = (float)GetScreenHeight() - scale * 350;
   // playerPos_philosophy = (Vector2){-10, y_pos_floor};
    playerPos_philosophy = {2200*scale,GetScreenHeight()-400*scale};
    exit_zone_philosophy = {2200*scale,GetScreenHeight()-400*scale};
    game_zone_philosophy = {1950*scale,GetScreenHeight()-400*scale};

    camera_philosophy.target = playerPos_philosophy;
    camera_philosophy.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera_philosophy.rotation = 0.0f;
    camera_philosophy.zoom = 1.0f;

    init_tictactoe();
}

void unload_philosophy() {
    unload_tictactoe();
    UnloadTexture(bg_image_philosophy);
}

void logic_draw_philosophy() {
    UpdateMusicStream(bgm_philosophy);
    if (!philosophy_game) pop_up_philosophy = "Let's gain some knowledge";

    if (dept_status_philosophy == Dept_philosophy) {
        // bool moving = false;
        // if (IsKeyDown(KEY_A)) { playerPos_philosophy.x -= 13; moving = true; }
        // if (IsKeyDown(KEY_D)) { playerPos_philosophy.x += 13; moving = true; }

        Vector2 offset_philosophy = walk_character_dept();
        playerPos_philosophy.x += offset_philosophy.x;

        if (moving && !walk_music_playing_philosophy) {
            PlayMusicStream(walk_music);
            walk_music_playing_philosophy = true;
        } else if (!moving && walk_music_playing_philosophy) {
            StopMusicStream(walk_music);
            walk_music_playing_philosophy = false;
        }
        if (walk_music_playing_philosophy) UpdateMusicStream(walk_music);

        bool eKeyHandled = false;
        // CheckCollisionPointCircle(mousePos, point, pointRadius);
        if (CheckCollisionPointCircle(GetScreenToWorld2D(GetMousePosition(), camera_philosophy), game_zone_philosophy, 100.0f)) {
            // pop_up_philosophy = "Press E to Solve"; // Visual feedback
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)&& !philosophy_game && !show_rules_popup_philosophy) {
                PlaySound(pop_up_sound);
                PlaySound(click_sound);
                show_rules_popup_philosophy = true;
                show_ok_button_philosophy = true;
            }
        }

        if (CheckCollisionCircles(playerPos_philosophy, 50.0f, exit_zone_philosophy, 50.0f)) {
            pop_up_philosophy = "Press E to Exit";
            if (IsKeyPressed(KEY_E)) {
                PlaySound(click_sound);
                unload_tictactoe();
                eKeyHandled = true;
                state_of_game = LAYER_MAP;
            } else if (IsKeyPressed(KEY_E) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(error_sound);
            }
        }

        if (IsKeyPressed(KEY_E) && !eKeyHandled) {
            PlaySound(error_sound);
        }
    } else if (dept_status_philosophy == Game_philosophy) {
        logic_tictactoe();
        if (philosophy_game || IsKeyDown(KEY_X)) {
            dept_status_philosophy = Dept_philosophy;
            game_pop_up_philosophy = "Nihongo gakka o kōryaku shita!!";
            philosophy_game = true;
            PlaySound(conquered_sound);
            unload_tictactoe();
        }
        if (IsKeyDown(KEY_Q)) {
            unload_tictactoe();
            dept_status_philosophy = Dept_philosophy;
        }
    }

    if (playerPos_philosophy.x <= -20) playerPos_philosophy.x = -20;
    else if (playerPos_philosophy.x >= bg_image_philosophy.width * scale) playerPos_philosophy.x = bg_image_philosophy.width * scale;

    scale = (float)GetScreenHeight() / bg_image_philosophy.height;
    float scaledWidth = bg_image_philosophy.width * scale;
    float cam = playerPos_philosophy.x;
    float halfScreen = screenWidth / 2.0f;
    if (cam < halfScreen) cam = halfScreen;
    if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;
    camera_philosophy.target = (Vector2){ cam, screenHeight / 2.0f };

    BeginMode2D(camera_philosophy);
    scale = (float)GetScreenHeight() / bg_image_philosophy.height;
    DrawTextureEx(bg_image_philosophy, (Vector2){0, 0}, 0.0f, scale, WHITE);
    // DrawTexture(character, playerPos_philosophy.x, playerPos_philosophy.y, WHITE);
    draw_char_dept(playerPos_philosophy,scale);
   
    // cout<<"japposx "<<playerPos_philosophy.x<<endl;
    // cout<<"japposy "<<playerPos_philosophy.y<<endl;
    // DrawCircleV(game_zone_philosophy, 20, RED);
    // DrawCircleV(exit_zone_philosophy, 20, GREEN);
    // DrawCircleV(playerPos_philosophy, 20, BLUE);
    EndMode2D();

    if (dept_status_philosophy == Game_philosophy) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    }

    if (dept_status_philosophy == Game_philosophy) {
        draw_tictactoe();
    }

    DrawText(game_pop_up_philosophy.c_str(), 20, screenHeight - 100, 20, GREEN);

    if (philosophy_game && CheckCollisionCircles(playerPos_philosophy, 50.0f, exit_zone_philosophy, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!philosophy_game) {
        DrawText(pop_up_philosophy.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    if (show_rules_popup_philosophy) {
        int screenW = screenWidth;
        int screenH = screenHeight;

        DrawRectangle(screenW / 2 - 220, screenH / 2 - 100, 440, 200, Fade(BLACK, 0.9f));
        DrawRectangleLines(screenW / 2 - 220, screenH / 2 - 100, 440, 200, LIGHTGRAY);

        DrawText(game_rules_philosophy.c_str(), screenW / 2 - MeasureText(game_rules_philosophy.c_str(), 20) / 2, screenH / 2 - 60, 20, RAYWHITE);

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

        if (show_ok_button_philosophy && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, okBtn)) {
                PlaySound(click_sound);
                dept_status_philosophy = Game_philosophy;
                show_rules_popup_philosophy = false;
                show_ok_button_philosophy = false;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(error_sound);
        }
    }

    loading_screen("Department Of philosophy\nUniversity of Dhaka");
}
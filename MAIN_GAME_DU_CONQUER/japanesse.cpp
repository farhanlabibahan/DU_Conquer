#include "raylib.h"
#include<iostream>
#include <string>
#include "loading.h"
#include "japanesse.h"
#include "global.h"
#include "kanji.h"
using namespace std;

typedef enum {
    Dept_japanesse,
    Game_japanesse
} dept_state_japanesse;

dept_state_japanesse dept_status_japanesse = Dept_japanesse;
Texture2D bg_image_japanesse;
Camera2D camera_japanesse = {0};
Vector2 playerPos_japanesse;
Vector2 game_zone_japanesse;
Vector2 exit_zone_japanesse;
string pop_up_japanesse = "Find and Solve the Clue";
string game_pop_up_japanesse = " ";
string game_rules_japanesse = "Lights On Game Rules:\nTurn on all the lights to win.\nPress X to exit the game.";
bool walk_music_playing_japanesse = false;
bool show_rules_popup_japanesse = false;
bool show_ok_button_japanesse = false;

void init_japanesse() {
    SetMusicVolume(bgm_japanesse, 0.13f);
    PlayMusicStream(bgm_japanesse);
    SetMusicVolume(walk_music, 1.0f);
    bg_image_japanesse = LoadTexture("resources/japanesse.png");
    scale = (float)GetMonitorHeight(0) / bg_image_japanesse.height;
    float y_pos_floor = (float)GetMonitorHeight(0) - scale * 350;
   // playerPos_japanesse = (Vector2){-10, y_pos_floor};
    playerPos_japanesse = {2200*scale,GetMonitorHeight(0)-400*scale};
    exit_zone_japanesse = {2200*scale,GetMonitorHeight(0)-400*scale};
    game_zone_japanesse = {1950*scale,GetMonitorHeight(0)-400*scale};

    camera_japanesse.target = playerPos_japanesse;
    camera_japanesse.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera_japanesse.rotation = 0.0f;
    camera_japanesse.zoom = 1.0f;

    init_kanji();
}

void unload_japanesse() {
    unload_kanji();
    UnloadTexture(bg_image_japanesse);
}

void logic_draw_japanesse() {
    UpdateMusicStream(bgm_japanesse);
    if (!japanesse_game) pop_up_japanesse = "Find and Solve the Clue";

    if (dept_status_japanesse == Dept_japanesse) {
        // bool moving = false;
        // if (IsKeyDown(KEY_A)) { playerPos_japanesse.x -= 13; moving = true; }
        // if (IsKeyDown(KEY_D)) { playerPos_japanesse.x += 13; moving = true; }

        Vector2 offset_japanesse = walk_character_dept();
        playerPos_japanesse.x += offset_japanesse.x;

        if (moving && !walk_music_playing_japanesse) {
            PlayMusicStream(walk_music);
            walk_music_playing_japanesse = true;
        } else if (!moving && walk_music_playing_japanesse) {
            StopMusicStream(walk_music);
            walk_music_playing_japanesse = false;
        }
        if (walk_music_playing_japanesse) UpdateMusicStream(walk_music);

        bool eKeyHandled = false;
        // CheckCollisionPointCircle(mousePos, point, pointRadius);
        Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera_japanesse);
        if (CheckCollisionPointCircle(mousePos, game_zone_japanesse, 100.0f)) {
            // pop_up_japanesse = "Press E to Solve"; // Visual feedback
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)&& !japanesse_game && !show_rules_popup_japanesse) {
                PlaySound(pop_up_sound);
                PlaySound(click_sound);
                show_rules_popup_japanesse = true;
                show_ok_button_japanesse = true;
            }
        }

        if (CheckCollisionCircles(playerPos_japanesse, 50.0f, exit_zone_japanesse, 50.0f)) {
            pop_up_japanesse = "Press E to Exit";
            if (IsKeyPressed(KEY_E)) {
                PlaySound(click_sound);
                unload_kanji();
                eKeyHandled = true;
                state_of_game = LAYER_MAP;
            } else if (IsKeyPressed(KEY_E)) {
                PlaySound(error_sound);
            }
        }

        if (IsKeyPressed(KEY_E) && !eKeyHandled) {
            PlaySound(error_sound);
        }
    } else if (dept_status_japanesse == Game_japanesse) {
        logic_kanji();
        if (japanesse_game || IsKeyDown(KEY_X)) {
            dept_status_japanesse = Dept_japanesse;
            game_pop_up_japanesse = "Nihongo gakka o kōryaku shita!!";
            japanesse_game = true;
            PlaySound(conquered_sound);
            unload_kanji();
        }
        if (IsKeyDown(KEY_Q)) {
            unload_kanji();
            dept_status_japanesse = Dept_japanesse;
        }
    }

    if (playerPos_japanesse.x <= -20) playerPos_japanesse.x = -20;
    else if (playerPos_japanesse.x >= bg_image_japanesse.width * scale) playerPos_japanesse.x = bg_image_japanesse.width * scale;

    scale = (float)GetMonitorHeight(0) / bg_image_japanesse.height;
    float scaledWidth = bg_image_japanesse.width * scale;
    float cam = playerPos_japanesse.x;
    float halfScreen = screenWidth / 2.0f;
    if (cam < halfScreen) cam = halfScreen;
    if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;
    camera_japanesse.target = (Vector2){ cam, screenHeight / 2.0f };

    BeginMode2D(camera_japanesse);
    scale = (float)GetMonitorHeight(0) / bg_image_japanesse.height;
    DrawTextureEx(bg_image_japanesse, (Vector2){0, 0}, 0.0f, scale, WHITE);
    // DrawTexture(character, playerPos_japanesse.x, playerPos_japanesse.y, WHITE);
    draw_char_dept(playerPos_japanesse,scale);
   
    // cout<<"japposx "<<playerPos_japanesse.x<<endl;
    // cout<<"japposy "<<playerPos_japanesse.y<<endl;
    // DrawCircleV(game_zone_japanesse, 20, RED);
    // DrawCircleV(exit_zone_japanesse, 20, GREEN);
    // DrawCircleV(playerPos_japanesse, 20, BLUE);
    EndMode2D();

    if (dept_status_japanesse == Game_japanesse) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    }

    if (dept_status_japanesse == Game_japanesse) {
        draw_kanji();
    }

    DrawText(game_pop_up_japanesse.c_str(), 20, screenHeight - 100, 20, GREEN);

    if (japanesse_game && CheckCollisionCircles(playerPos_japanesse, 50.0f, exit_zone_japanesse, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!japanesse_game) {
        DrawText(pop_up_japanesse.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    if (show_rules_popup_japanesse) {
        int screenW = screenWidth;
        int screenH = screenHeight;

        DrawRectangle(screenW / 2 - 220, screenH / 2 - 100, 440, 200, Fade(BLACK, 0.9f));
        DrawRectangleLines(screenW / 2 - 220, screenH / 2 - 100, 440, 200, LIGHTGRAY);

        DrawText(game_rules_japanesse.c_str(), screenW / 2 - MeasureText(game_rules_japanesse.c_str(), 20) / 2, screenH / 2 - 60, 20, RAYWHITE);

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

        if (show_ok_button_japanesse && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, okBtn)) {
                PlaySound(click_sound);
                dept_status_japanesse = Game_japanesse;
                show_rules_popup_japanesse = false;
                show_ok_button_japanesse = false;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(error_sound);
        }
    }

    loading_screen("Department Of japanesse\nUniversity of Dhaka");
}

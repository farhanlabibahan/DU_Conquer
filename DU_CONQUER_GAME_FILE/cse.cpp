#include "raylib.h"
#include <string>
#include "loading.h"
#include "cse.h"
#include "global.h"
#include "debug.h"
using namespace std;

typedef enum {
    Dept_cse,
    Game_cse
} dept_state_cse;

dept_state_cse dept_status_cse = Dept_cse;
Texture2D bg_image_cse;
// bool cse_game;
Camera2D camera_cse = {0};
Vector2 playerPos_cse;
Vector2 game_zone_cse;
Vector2 exit_zone_cse;
string pop_up_cse = "Find and Solve the Clue";
string game_pop_up_cse = " ";
string game_rules_cse = "Tame the bugs with necessary measures\n be fast";
bool walk_music_playing_cse = false;
bool show_rules_popup_cse = false;
bool show_ok_button_cse = false;

void init_cse() {
    SetMusicVolume(bgm_cse, 0.13f);
    PlayMusicStream(bgm_cse);
    SetMusicVolume(walk_music, 1.0f);
    bg_image_cse = LoadTexture("resources/cse.png");
    scale = (float)GetScreenHeight() / bg_image_cse.height;
    playerPos_cse = {4700*scale,screenHeight-400*scale};
    exit_zone_cse = {4700*scale,screenHeight-400*scale};
    game_zone_cse = {2491*scale,screenHeight-400*scale};

    camera_cse.target = playerPos_cse;
    camera_cse.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera_cse.rotation = 0.0f;
    camera_cse.zoom = 1.0f;

    init_debug();
}

void unload_cse() {
    unload_debug();
    UnloadTexture(bg_image_cse);
}

void logic_draw_cse() {
    UpdateMusicStream(bgm_cse);
    if (!cse_game) pop_up_cse = "Warning bugs corrupted one monitor";

    if (dept_status_cse == Dept_cse) {
        // bool moving = false;
        // if (IsKeyDown(KEY_A)) { playerPos_cse.x -= 13; moving = true; }
        // if (IsKeyDown(KEY_D)) { playerPos_cse.x += 13; moving = true; }


        Vector2 offset_cse = walk_character_dept();
        playerPos_cse.x += offset_cse.x;

        if (moving && !walk_music_playing_cse) {
            PlayMusicStream(walk_music);
            walk_music_playing_cse = true;
        } else if (!moving && walk_music_playing_cse) {
            StopMusicStream(walk_music);
            walk_music_playing_cse = false;
        }
        if (walk_music_playing_cse) UpdateMusicStream(walk_music);

        bool eKeyHandled = false;
        if (CheckCollisionPointCircle(GetScreenToWorld2D(GetMousePosition(), camera_cse), game_zone_cse, 100.0f)) {
            // pop_up_cse = "Press E to Solve"; // Visual feedback
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)&& !cse_game && !show_rules_popup_cse) {
                PlaySound(pop_up_sound);
                PlaySound(click_sound);
                show_rules_popup_cse = true;
                show_ok_button_cse = true;
                eKeyHandled = true;
            }
        }

        if (CheckCollisionCircles(playerPos_cse, 50.0f, exit_zone_cse, 50.0f)) {
            pop_up_cse = "Press E to Exit";
            if (IsKeyPressed(KEY_E)) {
                PlaySound(click_sound);
                // unload_debug();
                eKeyHandled = true;
                state_of_game = LAYER_LIFT;
            } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(error_sound);
            }
        }

        if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) && !eKeyHandled) {
            PlaySound(error_sound);
        }
    } else if (dept_status_cse == Game_cse) {
        logic_debug();
        if (cse_game || IsKeyDown(KEY_X)) {
            dept_status_cse = Dept_cse;
            game_pop_up_cse = "cse Conqured!! Abort";
            cse_game = true;
            PlaySound(conquered_sound);
            // unload_debug();
        }
        if (IsKeyDown(KEY_Q)) {
            // unload_debug();
            dept_status_cse = Dept_cse;
        }
    }

    if (playerPos_cse.x <= -20) playerPos_cse.x = -20;
    else if (playerPos_cse.x >= bg_image_cse.width * scale) playerPos_cse.x = bg_image_cse.width * scale;

    scale = (float)GetScreenHeight() / bg_image_cse.height;
    float scaledWidth = bg_image_cse.width * scale;
    float cam = playerPos_cse.x;
    float halfScreen = screenWidth / 2.0f;
    if (cam < halfScreen) cam = halfScreen;
    if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;
    camera_cse.target = (Vector2){ cam, screenHeight / 2.0f };

    BeginMode2D(camera_cse);
    scale = (float)GetScreenHeight() / bg_image_cse.height;
    DrawTextureEx(bg_image_cse, (Vector2){0, 0}, 0.0f, scale, WHITE);
    // DrawTexture(character, playerPos_cse.x, playerPos_cse.y, WHITE);
    draw_char_dept(playerPos_cse,scale);
    // DrawCircleV(game_zone_cse, 20, RED);
    // DrawCircleV(exit_zone_cse, 20, GREEN);
    // DrawCircleV(playerPos_cse, 20, BLUE);
    EndMode2D();

    if (dept_status_cse == Game_cse) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    }

    if (dept_status_cse == Game_cse) {
        draw_debug();
    }

    DrawText(game_pop_up_cse.c_str(), 20, screenHeight - 100, 20, GREEN);

    if (cse_game && CheckCollisionCircles(playerPos_cse, 50.0f, exit_zone_cse, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!cse_game) {
        DrawText(pop_up_cse.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    if (show_rules_popup_cse) {
        int screenW = screenWidth;
        int screenH = screenHeight;

        DrawRectangle(screenW / 2 - 220, screenH / 2 - 100, 440, 200, Fade(BLACK, 0.9f));
        DrawRectangleLines(screenW / 2 - 220, screenH / 2 - 100, 440, 200, LIGHTGRAY);

        DrawText(game_rules_cse.c_str(), screenW / 2 - MeasureText(game_rules_cse.c_str(), 20) / 2, screenH / 2 - 60, 20, RAYWHITE);

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

        if (show_ok_button_cse) {
            if (CheckCollisionPointRec(mouse, okBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(click_sound);
                dept_status_cse = Game_cse;
                show_rules_popup_cse = false;
                show_ok_button_cse = false;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(error_sound);
        }
    }

    loading_screen("Department Of cse\nUniversity of Dhaka");
}

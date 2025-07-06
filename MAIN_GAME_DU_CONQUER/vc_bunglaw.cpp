#include "raylib.h"
#include <string>
#include "loading.h"
#include "vc_bunglaw.h"
#include "global.h"
#include "shooting.h"
using namespace std;

typedef enum {
    Dept_vc_bunglaw,
    Game_vc_bunglaw
} dept_state_vc_bunglaw;

dept_state_vc_bunglaw dept_status_vc_bunglaw = Dept_vc_bunglaw;
bool cursor_locked = false;
Texture2D bg_image_vc_bunglaw;
// bool shooting_game;
Camera2D camera_vc_bunglaw = {0};
Vector2 playerPos_vc_bunglaw;
Vector2 game_zone_vc_bunglaw;
Vector2 exit_zone_vc_bunglaw;
string pop_up_vc_bunglaw = "Find and Solve the Clue";
string game_pop_up_vc_bunglaw = " ";
string game_rules_vc_bunglaw = "Lights On Game Rules:\nTurn on all the lights to win.\nPress X to exit the game.";
bool walk_music_playing_vc_bunglaw = false;
bool show_rules_popup_vc_bunglaw = false;
bool show_ok_button_vc_bunglaw = false;

void init_vc_bunglaw() {
    SetMusicVolume(bgm_vc_bunglaw, 0.13f);
    PlayMusicStream(bgm_vc_bunglaw);
    SetMusicVolume(walk_music, 1.0f);
    bg_image_vc_bunglaw = LoadTexture("resources/vc bunglow.png");
    scale = (float)GetMonitorHeight(0) / bg_image_vc_bunglaw.height;
    playerPos_vc_bunglaw = {0*scale,screenHeight-400*scale};
    exit_zone_vc_bunglaw = {0*scale,screenHeight-400*scale};
    game_zone_vc_bunglaw = {2500*scale,screenHeight-400*scale};

    camera_vc_bunglaw.target = playerPos_vc_bunglaw;
    camera_vc_bunglaw.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera_vc_bunglaw.rotation = 0.0f;
    camera_vc_bunglaw.zoom = 1.0f;

    init_shooting();
}

void unload_vc_bunglaw() {
    unload_shooting();
    UnloadTexture(bg_image_vc_bunglaw);
}

void logic_draw_vc_bunglaw() {
    if (dept_status_vc_bunglaw == Game_vc_bunglaw) {
        if (!cursor_locked) {
            DisableCursor();
            cursor_locked = true;
        }
        DrawRectangle(0,0,screenWidth,screenHeight,BLACK);
        logic_shooting();

        if (shooting_game || IsKeyDown(KEY_X)) {
            dept_status_vc_bunglaw = Dept_vc_bunglaw;
            game_pop_up_vc_bunglaw = "vc_bunglaw Conqured!! Abort";
            shooting_game = true;
            PlaySound(conquered_sound);
        }

        if (IsKeyDown(KEY_Q)) {
            dept_status_vc_bunglaw = Dept_vc_bunglaw;
            return;
        }
        draw_shooting();
    }
    
    if (!shooting_game) pop_up_vc_bunglaw = "Find and Solve the Clue";

    if (dept_status_vc_bunglaw == Dept_vc_bunglaw) {
        if (cursor_locked) {
            EnableCursor();
            cursor_locked = false;
        }
        // bool moving = false;
        // if (IsKeyDown(KEY_A)) { playerPos_vc_bunglaw.x -= 13; moving = true; }
        // if (IsKeyDown(KEY_D)) { playerPos_vc_bunglaw.x += 13; moving = true; }

        UpdateMusicStream(bgm_vc_bunglaw);
        Vector2 offset_vc_bunglaw = walk_character_dept();
        playerPos_vc_bunglaw.x += offset_vc_bunglaw.x;

        if (moving && !walk_music_playing_vc_bunglaw) {
            PlayMusicStream(walk_music);
            walk_music_playing_vc_bunglaw = true;
        } else if (!moving && walk_music_playing_vc_bunglaw) {
            StopMusicStream(walk_music);
            walk_music_playing_vc_bunglaw = false;
        }
        if (walk_music_playing_vc_bunglaw) UpdateMusicStream(walk_music);

        bool eKeyHandled = false;
        if (CheckCollisionPointCircle(GetScreenToWorld2D(GetMousePosition(), camera_vc_bunglaw), game_zone_vc_bunglaw, 100.0f)) {
            // pop_up_vc_bunglaw = "Press E to Solve"; // Visual feedback
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)&& !shooting_game && !show_rules_popup_vc_bunglaw) {
                PlaySound(pop_up_sound);
                PlaySound(click_sound);
                show_rules_popup_vc_bunglaw = true;
                show_ok_button_vc_bunglaw = true;
                eKeyHandled = true;
            }
        }

        if (CheckCollisionCircles(playerPos_vc_bunglaw, 50.0f, exit_zone_vc_bunglaw, 50.0f)) {
            pop_up_vc_bunglaw = "Press E to Exit";
            if (IsKeyPressed(KEY_E)) {
                PlaySound(click_sound);
                // unload_shooting();
                eKeyHandled = true;
                state_of_game = LAYER_MAP;
            } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(error_sound);
            }
        }

        if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) && !eKeyHandled) {
            PlaySound(error_sound);
        }
    

    if (playerPos_vc_bunglaw.x <= -20) playerPos_vc_bunglaw.x = -20;
    else if (playerPos_vc_bunglaw.x >= bg_image_vc_bunglaw.width * scale) playerPos_vc_bunglaw.x = bg_image_vc_bunglaw.width * scale;

    scale = (float)GetMonitorHeight(0) / bg_image_vc_bunglaw.height;
    float scaledWidth = bg_image_vc_bunglaw.width * scale;
    float cam = playerPos_vc_bunglaw.x;
    float halfScreen = screenWidth / 2.0f;
    if (cam < halfScreen) cam = halfScreen;
    if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;
    camera_vc_bunglaw.target = (Vector2){ cam, screenHeight / 2.0f };

    BeginMode2D(camera_vc_bunglaw);
    scale = (float)GetMonitorHeight(0) / bg_image_vc_bunglaw.height;
    DrawTextureEx(bg_image_vc_bunglaw, (Vector2){0, 0}, 0.0f, scale, WHITE);
    // DrawTexture(character, playerPos_vc_bunglaw.x, playerPos_vc_bunglaw.y, WHITE);
    draw_char_dept(playerPos_vc_bunglaw,scale);
    // DrawCircleV(game_zone_vc_bunglaw, 20, RED);
    // DrawCircleV(exit_zone_vc_bunglaw, 20, GREEN);
    // DrawCircleV(playerPos_vc_bunglaw, 20, BLUE);
    EndMode2D();

    DrawText(game_pop_up_vc_bunglaw.c_str(), 20, screenHeight - 100, 20, GREEN);

    if (shooting_game && CheckCollisionCircles(playerPos_vc_bunglaw, 50.0f, exit_zone_vc_bunglaw, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!shooting_game) {
        DrawText(pop_up_vc_bunglaw.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    if (show_rules_popup_vc_bunglaw) {
        int screenW = screenWidth;
        int screenH = screenHeight;

        DrawRectangle(screenW / 2 - 220, screenH / 2 - 100, 440, 200, Fade(BLACK, 0.9f));
        DrawRectangleLines(screenW / 2 - 220, screenH / 2 - 100, 440, 200, LIGHTGRAY);

        DrawText(game_rules_vc_bunglaw.c_str(), screenW / 2 - MeasureText(game_rules_vc_bunglaw.c_str(), 20) / 2, screenH / 2 - 60, 20, RAYWHITE);

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

        if (show_ok_button_vc_bunglaw) {
            if (CheckCollisionPointRec(mouse, okBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(click_sound);
                dept_status_vc_bunglaw = Game_vc_bunglaw;
                show_rules_popup_vc_bunglaw = false;
                show_ok_button_vc_bunglaw = false;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(error_sound);
        }
    }

    loading_screen("Department Of vc_bunglaw\nUniversity of Dhaka");
    }
}

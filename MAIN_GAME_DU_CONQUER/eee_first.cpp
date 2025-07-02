#include "raylib.h"
#include <string>
#include "loading.h"
#include "eee_first.h"
#include "global.h"
#include "signal.h"
using namespace std;

typedef enum {
    Dept_eee_first,
    Game_eee_first
} dept_state_eee_first;

dept_state_eee_first dept_status_eee_first = Dept_eee_first;
Texture2D bg_image_eee_first;
// bool eee_game_1;
Camera2D camera_eee_first = {0};
Vector2 playerPos_eee_first;
Vector2 game_zone_eee_first;
Vector2 exit_zone_eee_first;
string pop_up_eee_first = "Find and Solve the Clue";
string game_pop_up_eee_first = " ";
string game_rules_eee_first = "Lights On Game Rules:\nTurn on all the lights to win.\nPress X to exit the game.";
bool walk_music_playing_eee_first = false;
bool show_rules_popup_eee_first = false;
bool show_ok_button_eee_first = false;

void init_eee_first() {
    SetMusicVolume(bgm_eee_first, 0.13f);
    PlayMusicStream(bgm_eee_first);
    SetMusicVolume(walk_music, 1.0f);
    bg_image_eee_first = LoadTexture("resources/eeeeeee.png");
    scale = (float)GetMonitorHeight(0) / bg_image_eee_first.height;
    playerPos_eee_first = {0*scale,screenHeight-400*scale};
    exit_zone_eee_first = {0*scale,screenHeight-400*scale};
    game_zone_eee_first = {2500*scale,screenHeight-400*scale};

    camera_eee_first.target = playerPos_eee_first;
    camera_eee_first.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera_eee_first.rotation = 0.0f;
    camera_eee_first.zoom = 1.0f;

    init_signal();
}

void unload_eee_first() {
    unload_signal();
    UnloadTexture(bg_image_eee_first);
}

void logic_draw_eee_first() {
    UpdateMusicStream(bgm_eee_first);
    if (!eee_game_1) pop_up_eee_first = "Find and Solve the Clue";

    if (dept_status_eee_first == Dept_eee_first) {
        // bool moving = false;
        // if (IsKeyDown(KEY_A)) { playerPos_eee_first.x -= 13; moving = true; }
        // if (IsKeyDown(KEY_D)) { playerPos_eee_first.x += 13; moving = true; }


        Vector2 offset_eee_first = walk_character_dept();
        playerPos_eee_first.x += offset_eee_first.x;

        if (moving && !walk_music_playing_eee_first) {
            PlayMusicStream(walk_music);
            walk_music_playing_eee_first = true;
        } else if (!moving && walk_music_playing_eee_first) {
            StopMusicStream(walk_music);
            walk_music_playing_eee_first = false;
        }
        if (walk_music_playing_eee_first) UpdateMusicStream(walk_music);

        bool eKeyHandled = false;
        if (CheckCollisionPointCircle(GetScreenToWorld2D(GetMousePosition(), camera_eee_first), game_zone_eee_first, 100.0f)) {
            // pop_up_eee_first = "Press E to Solve"; // Visual feedback
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)&& !eee_game_1 && !show_rules_popup_eee_first) {
                PlaySound(pop_up_sound);
                PlaySound(click_sound);
                show_rules_popup_eee_first = true;
                show_ok_button_eee_first = true;
                eKeyHandled = true;
            }
        }

        if (CheckCollisionCircles(playerPos_eee_first, 50.0f, exit_zone_eee_first, 50.0f)) {
            pop_up_eee_first = "Press E to Exit";
            if (IsKeyPressed(KEY_E)) {
                PlaySound(click_sound);
                // unload_signal();
                eKeyHandled = true;
                state_of_game = LAYER_MAP;
            } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(error_sound);
            }
        }

        if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) && !eKeyHandled) {
            PlaySound(error_sound);
        }
    } else if (dept_status_eee_first == Game_eee_first) {
        logic_signal();
        if (eee_game_1 || IsKeyDown(KEY_X)) {
            dept_status_eee_first = Dept_eee_first;
            game_pop_up_eee_first = "eee_first Conqured!! Abort";
            eee_game_1 = true;
            PlaySound(conquered_sound);
            // unload_signal();
        }
        if (IsKeyDown(KEY_Q)) {
            // unload_signal();
            dept_status_eee_first = Dept_eee_first;
        }
    }

    if (playerPos_eee_first.x <= -20) playerPos_eee_first.x = -20;
    else if (playerPos_eee_first.x >= bg_image_eee_first.width * scale) playerPos_eee_first.x = bg_image_eee_first.width * scale;

    scale = (float)GetMonitorHeight(0) / bg_image_eee_first.height;
    float scaledWidth = bg_image_eee_first.width * scale;
    float cam = playerPos_eee_first.x;
    float halfScreen = screenWidth / 2.0f;
    if (cam < halfScreen) cam = halfScreen;
    if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;
    camera_eee_first.target = (Vector2){ cam, screenHeight / 2.0f };

    BeginMode2D(camera_eee_first);
    scale = (float)GetMonitorHeight(0) / bg_image_eee_first.height;
    DrawTextureEx(bg_image_eee_first, (Vector2){0, 0}, 0.0f, scale, WHITE);
    // DrawTexture(character, playerPos_eee_first.x, playerPos_eee_first.y, WHITE);
    draw_char_dept(playerPos_eee_first,scale);
    // DrawCircleV(game_zone_eee_first, 20, RED);
    // DrawCircleV(exit_zone_eee_first, 20, GREEN);
    // DrawCircleV(playerPos_eee_first, 20, BLUE);
    EndMode2D();

    if (dept_status_eee_first == Game_eee_first) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    }

    if (dept_status_eee_first == Game_eee_first) {
        draw_signal();
    }

    DrawText(game_pop_up_eee_first.c_str(), 20, screenHeight - 100, 20, GREEN);

    if (eee_game_1 && CheckCollisionCircles(playerPos_eee_first, 50.0f, exit_zone_eee_first, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!eee_game_1) {
        DrawText(pop_up_eee_first.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    if (show_rules_popup_eee_first) {
        int screenW = screenWidth;
        int screenH = screenHeight;

        DrawRectangle(screenW / 2 - 220, screenH / 2 - 100, 440, 200, Fade(BLACK, 0.9f));
        DrawRectangleLines(screenW / 2 - 220, screenH / 2 - 100, 440, 200, LIGHTGRAY);

        DrawText(game_rules_eee_first.c_str(), screenW / 2 - MeasureText(game_rules_eee_first.c_str(), 20) / 2, screenH / 2 - 60, 20, RAYWHITE);

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

        if (show_ok_button_eee_first) {
            if (CheckCollisionPointRec(mouse, okBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(click_sound);
                dept_status_eee_first = Game_eee_first;
                show_rules_popup_eee_first = false;
                show_ok_button_eee_first = false;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(error_sound);
        }
    }

    loading_screen("Department Of eee_first\nUniversity of Dhaka");
}

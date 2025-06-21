#include "raylib.h"
#include <string>
#include "loading.h"
#include "microbiology.h"
#include "global.h"
#include "snake.h"
using namespace std;

typedef enum {
    Dept_microbiology,
    Game_microbiology
} dept_state_microbiology;

dept_state_microbiology dept_status_microbiology = Dept_microbiology;
Texture2D bg_image_microbiology;
Camera2D camera_microbiology = {0};
Vector2 playerPos_microbiology = {-20, 410};
Vector2 game_zone_microbiology = {1200,700};
Vector2 exit_zone_microbiology = {50,700};
string pop_up_microbiology = "Find and Solve the Clue";
string game_pop_up_microbiology = " ";
string game_rules_microbiology = "Lights On Game Rules:\nTurn on all the lights to win.\nPress X to exit the game.";
bool walk_music_playing_microbiology = false;
bool show_rules_popup_microbiology = false;
bool show_ok_button_microbiology = false;

void init_microbiology() {
    SetMusicVolume(bgm_microbiology, 0.13f);
    PlayMusicStream(bgm_microbiology);
    SetMusicVolume(walk_music, 1.0f);
    bg_image_microbiology = LoadTexture("resources/HISTORY_EX.png");
    scale = (float)GetMonitorHeight(0) / bg_image_microbiology.height;
    float y_pos_floor = (float)GetMonitorHeight(0) - scale * 350;
    playerPos_microbiology = (Vector2){-10, y_pos_floor};
    game_zone_microbiology = {scale * 1200, y_pos_floor};
    exit_zone_microbiology = {scale * 10, y_pos_floor};

    camera_microbiology.target = playerPos_microbiology;
    camera_microbiology.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera_microbiology.rotation = 0.0f;
    camera_microbiology.zoom = 1.0f;

    init_snake();
}

void unload_microbiology() {
    unload_snake();
    UnloadTexture(bg_image_microbiology);
}

void logic_draw_microbiology() {
    UpdateMusicStream(bgm_microbiology);
    if (!microbiology_game) pop_up_microbiology = "Find and Solve the Clue";

    if (dept_status_microbiology == Dept_microbiology) {
        bool moving = false;
        if (IsKeyDown(KEY_A)) { playerPos_microbiology.x -= 13; moving = true; }
        if (IsKeyDown(KEY_D)) { playerPos_microbiology.x += 13; moving = true; }

        if (moving && !walk_music_playing_microbiology) {
            PlayMusicStream(walk_music);
            walk_music_playing_microbiology = true;
        } else if (!moving && walk_music_playing_microbiology) {
            StopMusicStream(walk_music);
            walk_music_playing_microbiology = false;
        }
        if (walk_music_playing_microbiology) UpdateMusicStream(walk_music);

        bool eKeyHandled = false;
        if (CheckCollisionCircles(playerPos_microbiology, 50.0f, game_zone_microbiology, 50.0f)) {
            pop_up_microbiology = "Press E to Solve";
            if (IsKeyPressed(KEY_E) && !microbiology_game && !show_rules_popup_microbiology) {
                PlaySound(pop_up_sound);
                PlaySound(click_sound);
                init_snake();
                show_rules_popup_microbiology = true;
                show_ok_button_microbiology = true;
                eKeyHandled = true;
            }
        }

        if (CheckCollisionCircles(playerPos_microbiology, 50.0f, exit_zone_microbiology, 50.0f)) {
            pop_up_microbiology = "Press E to Exit";
            if (IsKeyPressed(KEY_E)) {
                PlaySound(click_sound);
                unload_snake();
                eKeyHandled = true;
                state_of_game = LAYER_MAP;
            } else if (IsKeyPressed(KEY_E)) {
                PlaySound(error_sound);
            }
        }

        if (IsKeyPressed(KEY_E) && !eKeyHandled) {
            PlaySound(error_sound);
        }
    } else if (dept_status_microbiology == Game_microbiology) {
        logic_snake();
        if (microbiology_game || IsKeyDown(KEY_X)) {
            dept_status_microbiology = Dept_microbiology;
            game_pop_up_microbiology = "microbiology Conqured!! Abort";
            microbiology_game = true;
            PlaySound(conquered_sound);
            unload_snake();
        }
        if (IsKeyDown(KEY_Q)) {
            unload_snake();
            dept_status_microbiology = Dept_microbiology;
        }
    }

    if (playerPos_microbiology.x <= -20) playerPos_microbiology.x = -20;
    else if (playerPos_microbiology.x >= bg_image_microbiology.width * scale) playerPos_microbiology.x = bg_image_microbiology.width * scale;

    scale = (float)GetMonitorHeight(0) / bg_image_microbiology.height;
    float scaledWidth = bg_image_microbiology.width * scale;
    float cam = playerPos_microbiology.x;
    float halfScreen = screenWidth / 2.0f;
    if (cam < halfScreen) cam = halfScreen;
    if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;
    camera_microbiology.target = (Vector2){ cam, screenHeight / 2.0f };

    BeginMode2D(camera_microbiology);
    scale = (float)GetMonitorHeight(0) / bg_image_microbiology.height;
    DrawTextureEx(bg_image_microbiology, (Vector2){0, 0}, 0.0f, scale, WHITE);
    DrawTexture(character, playerPos_microbiology.x, playerPos_microbiology.y, WHITE);
    DrawCircleV(game_zone_microbiology, 20, RED);
    DrawCircleV(exit_zone_microbiology, 20, GREEN);
    DrawCircleV(playerPos_microbiology, 20, BLUE);
    EndMode2D();

    if (dept_status_microbiology == Game_microbiology) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    }

    if (dept_status_microbiology == Game_microbiology) {
        draw_snake();
    }

    DrawText(game_pop_up_microbiology.c_str(), 20, screenHeight - 100, 20, GREEN);

    if (microbiology_game && CheckCollisionCircles(playerPos_microbiology, 50.0f, exit_zone_microbiology, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!microbiology_game) {
        DrawText(pop_up_microbiology.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    if (show_rules_popup_microbiology) {
        int screenW = screenWidth;
        int screenH = screenHeight;

        DrawRectangle(screenW / 2 - 220, screenH / 2 - 100, 440, 200, Fade(BLACK, 0.9f));
        DrawRectangleLines(screenW / 2 - 220, screenH / 2 - 100, 440, 200, LIGHTGRAY);

        DrawText(game_rules_microbiology.c_str(), screenW / 2 - MeasureText(game_rules_microbiology.c_str(), 20) / 2, screenH / 2 - 60, 20, RAYWHITE);

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

        if (show_ok_button_microbiology && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, okBtn)) {
                PlaySound(click_sound);
                dept_status_microbiology = Game_microbiology;
                show_rules_popup_microbiology = false;
                show_ok_button_microbiology = false;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(error_sound);
        }
    }

    loading_screen("Department Of microbiology\nUniversity of Dhaka");
}

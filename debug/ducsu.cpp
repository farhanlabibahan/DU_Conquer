// ducsu.cpp
#include "raylib.h"
#include "global.h"
#include "loading.h"
#include "memory_match.h"
#include <string>

using namespace std;

// Game state for DUCSU
typedef enum {
    Dept_ducsu,
    Game_ducsu
} dept_state_ducsu;

dept_state_ducsu dept_status_ducsu = Dept_ducsu;
Vector2 playerPos_ducsu = {-20, 400};
Vector2 game_zone_ducsu = {1200, 700};
Vector2 exit_zone_ducsu = {50, 700};
string pop_up_ducsu = "Find and Solve the Clue";
string game_pop_up_ducsu = " ";
bool walk_music_playing_ducsu = false;
bool game_win_ducsu = false;
bool show_rules_popup_ducsu = false;
bool show_ok_button_ducsu = false;
Camera2D camera_ducsu;
Texture2D bg_image_ducsu;

void init_ducsu() {
    SetMusicVolume(bgm_ducsu, 0.13f);
    PlayMusicStream(bgm_ducsu);
    SetMusicVolume(walk_music, 1.0f);
    float y_pos_floor = (float)screenHeight - scale * 350;
    bg_image_ducsu = LoadTexture("resources/EEE_EX.png");
    playerPos_ducsu = (Vector2){-10, y_pos_floor};
    game_zone_ducsu = {scale * 1200, y_pos_floor};
    exit_zone_ducsu = {scale * 10, y_pos_floor};
    camera_ducsu.target = playerPos_ducsu;
    camera_ducsu.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera_ducsu.rotation = 0.0f;
    camera_ducsu.zoom = 1.0f;
    init_memory_match();
}

void unload_ducsu() {
    unload_memory_match();
}

void logic_draw_ducsu() {
    UpdateMusicStream(bgm_ducsu);
    if (!game_win_ducsu) pop_up_ducsu = "Find and Solve the Clue";

    if (dept_status_ducsu == Dept_ducsu) {
        bool moving = false;
        if (IsKeyDown(KEY_A)) {
            playerPos_ducsu.x -= 3;
            moving = true;
        }
        if (IsKeyDown(KEY_D)) {
            playerPos_ducsu.x += 3;
            moving = true;
        }

        if (moving && !walk_music_playing_ducsu) {
            PlayMusicStream(walk_music);
            walk_music_playing_ducsu = true;
        } else if (!moving && walk_music_playing_ducsu) {
            StopMusicStream(walk_music);
            walk_music_playing_ducsu = false;
        }

        if (walk_music_playing_ducsu) {
            UpdateMusicStream(walk_music);
        }

        bool eKeyHandled = false;

        if (CheckCollisionCircles(playerPos_ducsu, 50.0f, game_zone_ducsu, 50.0f)) {
            pop_up_ducsu = "Press E to Solve";
            if (IsKeyPressed(KEY_E) && !game_win_ducsu && !show_rules_popup_ducsu) {
                PlaySound(pop_up_sound);
                PlaySound(click_sound);
                show_rules_popup_ducsu = true;
                show_ok_button_ducsu = true;
                eKeyHandled = true;
            }
        }

        if (CheckCollisionCircles(playerPos_ducsu, 50.0f, exit_zone_ducsu, 50.0f)) {
            pop_up_ducsu = "Press E to Exit";
            if (IsKeyPressed(KEY_E)) {
                PlaySound(click_sound);
                unload_memory_match();
                eKeyHandled = true;
                state_of_game = LAYER_MAP;
            }
        }

        if (IsKeyPressed(KEY_E) && !eKeyHandled) {
            PlaySound(error_sound);
        }
    } else if (dept_status_ducsu == Game_ducsu) {
        logic_memory_match();
        if (ducsu_game || IsKeyDown(KEY_X)) {
            dept_status_ducsu = Dept_ducsu;
            game_pop_up_ducsu = "DUCSU Conquered!";
            game_win_ducsu = true;
            PlaySound(conquered_sound);
            unload_memory_match();
        }
    }

    if (playerPos_ducsu.x <= -20) playerPos_ducsu.x = -20;
    else if (playerPos_ducsu.x >= bg_image_ducsu.width * scale) playerPos_ducsu.x = bg_image_ducsu.width * scale;
    float cam = playerPos_ducsu.x;
    float halfScreen = screenHeight / 2.0f;
    float scaledWidth = scale * bg_image_ducsu.width;
    if (cam < halfScreen) cam = halfScreen;
    if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;

    camera_ducsu.target = (Vector2){cam, screenHeight / 2.0f};

    BeginMode2D(camera_ducsu);
    DrawTextureEx(bg_image_ducsu, (Vector2){0, 0}, 0.0f, scale, WHITE);
    DrawTexture(character, playerPos_ducsu.x, playerPos_ducsu.y, WHITE);
    DrawCircleV(game_zone_ducsu, 20, RED);
    DrawCircleV(exit_zone_ducsu, 20, GREEN);
    DrawCircleV(playerPos_ducsu, 20, BLUE);
    EndMode2D();

    if (dept_status_ducsu == Game_ducsu) {
        Rectangle popupRect = {
            (float)(screenWidth - 600) / 2,
            (float)(screenHeight - 800) / 2,
            600,
            800
        };
        draw_memory_match(popupRect);
    }

    DrawText(game_pop_up_ducsu.c_str(), 20, screenHeight - 100, 20, GREEN);
    if (game_win_ducsu && CheckCollisionCircles(playerPos_ducsu, 50.0f, exit_zone_ducsu, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!game_win_ducsu) {
        DrawText(pop_up_ducsu.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    if (show_rules_popup_ducsu) {
        int screenW = screenWidth;
        int screenH = screenHeight;
        DrawRectangle(screenW / 2 - 220, screenH / 2 - 100, 440, 200, Fade(BLACK, 0.9f));
        DrawRectangleLines(screenW / 2 - 220, screenH / 2 - 100, 440, 200, LIGHTGRAY);
        DrawText("Memory Match Rules:\nFind matching clues to win.\nClick to flip cards. Press X to exit.", screenW / 2 - 200, screenH / 2 - 70, 20, RAYWHITE);

        Rectangle okBtn = {screenW / 2 - 50, screenH / 2 + 30, 100, 40};
        Vector2 mouse = GetMousePosition();
        Color btnColor = CheckCollisionPointRec(mouse, okBtn) ? RED : DARKGRAY;
        static bool wasHovering = false;
        bool isHovering = CheckCollisionPointRec(mouse, okBtn);
        if (isHovering && !wasHovering) PlaySound(click_sound);
        wasHovering = isHovering;

        DrawRectangleRec(okBtn, btnColor);
        DrawText("OK", screenW / 2 - MeasureText("OK", 20) / 2, screenH / 2 + 40, 20, WHITE);

        if (show_ok_button_ducsu && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, okBtn)) {
                PlaySound(click_sound);
                dept_status_ducsu = Game_ducsu;
                show_rules_popup_ducsu = false;
                show_ok_button_ducsu = false;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(error_sound);
        }
    }

    loading_screen("DUCSU - Memory Match Game");
}

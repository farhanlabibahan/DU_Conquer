#include "raylib.h"
#include <string>
#include "loading.h"
#include "ducsu.h"
#include "global.h"
#include "memory_match.h"
using namespace std;

typedef enum {
    Dept_ducsu,
    Game_ducsu
} dept_state_ducsu;

dept_state_ducsu dept_status_ducsu = Dept_ducsu;
Texture2D bg_image_ducsu;
// bool ducsu_game;
Camera2D camera_ducsu = {0};
Vector2 playerPos_ducsu;
Vector2 game_zone_ducsu;
Vector2 exit_zone_ducsu;
string pop_up_ducsu = "Opps a corrupted vote";
string game_pop_up_ducsu = " ";
string game_rules_ducsu = "Match the symbols to cast a vote";
bool walk_music_playing_ducsu = false;
bool show_rules_popup_ducsu = false;
bool show_ok_button_ducsu = false;

void init_ducsu() {
    SetMusicVolume(bgm_ducsu, 0.13f);
    PlayMusicStream(bgm_ducsu);
    SetMusicVolume(walk_music, 1.0f);
    bg_image_ducsu = LoadTexture("resources/ducsu.png");
    scale = (float)GetScreenHeight() / bg_image_ducsu.height;
    playerPos_ducsu = {0*scale,screenHeight-400*scale};
    exit_zone_ducsu = {0*scale,screenHeight-400*scale};
    game_zone_ducsu = {2500*scale,screenHeight-400*scale};

    camera_ducsu.target = playerPos_ducsu;
    camera_ducsu.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera_ducsu.rotation = 0.0f;
    camera_ducsu.zoom = 1.0f;

    init_memory_match();
}

void unload_ducsu() {
    unload_memory_match();
    UnloadTexture(bg_image_ducsu);
}

void logic_draw_ducsu() {
    UpdateMusicStream(bgm_ducsu);
    if (!ducsu_game) pop_up_ducsu = "Find and Solve the Clue";

    if (dept_status_ducsu == Dept_ducsu) {
        // bool moving = false;
        // if (IsKeyDown(KEY_A)) { playerPos_ducsu.x -= 13; moving = true; }
        // if (IsKeyDown(KEY_D)) { playerPos_ducsu.x += 13; moving = true; }


        Vector2 offset_ducsu = walk_character_dept();
        playerPos_ducsu.x += offset_ducsu.x;

        if (moving && !walk_music_playing_ducsu) {
            PlayMusicStream(walk_music);
            walk_music_playing_ducsu = true;
        } else if (!moving && walk_music_playing_ducsu) {
            StopMusicStream(walk_music);
            walk_music_playing_ducsu = false;
        }
        if (walk_music_playing_ducsu) UpdateMusicStream(walk_music);

        bool eKeyHandled = false;
        if (CheckCollisionPointCircle(GetScreenToWorld2D(GetMousePosition(), camera_ducsu), game_zone_ducsu, 100.0f)) {
            // pop_up_ducsu = "Press E to Solve"; // Visual feedback
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)&& !ducsu_game && !show_rules_popup_ducsu) {
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
                // unload_memory_match();
                eKeyHandled = true;
                state_of_game = LAYER_MAP;
            } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(error_sound);
            }
        }

        if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) && !eKeyHandled) {
            PlaySound(error_sound);
        }
    } else if (dept_status_ducsu == Game_ducsu) {
        logic_memory_match();
        if (ducsu_game || IsKeyDown(KEY_X)) {
            dept_status_ducsu = Dept_ducsu;
            game_pop_up_ducsu = "ducsu Conqured!! Abort";
            ducsu_game = true;
            PlaySound(conquered_sound);
            // unload_memory_match();
        }
        if (IsKeyDown(KEY_Q)) {
            // unload_memory_match();
            dept_status_ducsu = Dept_ducsu;
        }
    }

    if (playerPos_ducsu.x <= -20) playerPos_ducsu.x = -20;
    else if (playerPos_ducsu.x >= bg_image_ducsu.width * scale) playerPos_ducsu.x = bg_image_ducsu.width * scale;

    scale = (float)GetScreenHeight() / bg_image_ducsu.height;
    float scaledWidth = bg_image_ducsu.width * scale;
    float cam = playerPos_ducsu.x;
    float halfScreen = screenWidth / 2.0f;
    if (cam < halfScreen) cam = halfScreen;
    if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;
    camera_ducsu.target = (Vector2){ cam, screenHeight / 2.0f };

    BeginMode2D(camera_ducsu);
    scale = (float)GetScreenHeight() / bg_image_ducsu.height;
    DrawTextureEx(bg_image_ducsu, (Vector2){0, 0}, 0.0f, scale, WHITE);
    // DrawTexture(character, playerPos_ducsu.x, playerPos_ducsu.y, WHITE);
    draw_char_dept(playerPos_ducsu,scale);
    // DrawCircleV(game_zone_ducsu, 20, RED);
    // DrawCircleV(exit_zone_ducsu, 20, GREEN);
    // DrawCircleV(playerPos_ducsu, 20, BLUE);
    EndMode2D();

    if (dept_status_ducsu == Game_ducsu) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    }

    if (dept_status_ducsu == Game_ducsu) {
        draw_memory_match();
    }

    DrawText(game_pop_up_ducsu.c_str(), 20, screenHeight - 100, 20, GREEN);

    if (ducsu_game && CheckCollisionCircles(playerPos_ducsu, 50.0f, exit_zone_ducsu, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!ducsu_game) {
        DrawText(pop_up_ducsu.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    if (show_rules_popup_ducsu) {
        int screenW = screenWidth;
        int screenH = screenHeight;

        DrawRectangle(screenW / 2 - 220, screenH / 2 - 100, 440, 200, Fade(BLACK, 0.9f));
        DrawRectangleLines(screenW / 2 - 220, screenH / 2 - 100, 440, 200, LIGHTGRAY);

        DrawText(game_rules_ducsu.c_str(), screenW / 2 - MeasureText(game_rules_ducsu.c_str(), 20) / 2, screenH / 2 - 60, 20, RAYWHITE);

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

        if (show_ok_button_ducsu) {
            if (CheckCollisionPointRec(mouse, okBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(click_sound);
                dept_status_ducsu = Game_ducsu;
                show_rules_popup_ducsu = false;
                show_ok_button_ducsu = false;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(error_sound);
        }
    }

    loading_screen("Department Of ducsu\nUniversity of Dhaka");
}

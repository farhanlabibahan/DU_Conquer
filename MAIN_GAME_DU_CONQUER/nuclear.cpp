#include "raylib.h"
#include <string>
#include "loading.h"
#include "nuclear.h"
#include "global.h"
#include "minesweeper.h"
using namespace std;

typedef enum {
    Dept_nuclear,
    Game_nuclear
} dept_state_nuclear;

dept_state_nuclear dept_status_nuclear = Dept_nuclear;
Texture2D bg_image_nuclear;
// bool nuclear_game;
Camera2D camera_nuclear = {0};
Vector2 playerPos_nuclear;
Vector2 game_zone_nuclear;
Vector2 exit_zone_nuclear;
string pop_up_nuclear = "Find and Solve the Clue";
string game_pop_up_nuclear = " ";
string game_rules_nuclear = "Lights On Game Rules:\nTurn on all the lights to win.\nPress X to exit the game.";
bool walk_music_playing_nuclear = false;
bool show_rules_popup_nuclear = false;
bool show_ok_button_nuclear = false;

void init_nuclear() {
    SetMusicVolume(bgm_nuclear, 0.13f);
    PlayMusicStream(bgm_nuclear);
    SetMusicVolume(walk_music, 1.0f);
    bg_image_nuclear = LoadTexture("resources/nuclear.png");
    scale = (float)GetMonitorHeight(0) / bg_image_nuclear.height;
    playerPos_nuclear = {0*scale,screenHeight-400*scale};
    exit_zone_nuclear = {0*scale,screenHeight-400*scale};
    game_zone_nuclear = {2500*scale,screenHeight-400*scale};

    camera_nuclear.target = playerPos_nuclear;
    camera_nuclear.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera_nuclear.rotation = 0.0f;
    camera_nuclear.zoom = 1.0f;

    init_minesweeper();
}

void unload_nuclear() {
    unload_minesweeper();
    UnloadTexture(bg_image_nuclear);
}

void logic_draw_nuclear() {
    UpdateMusicStream(bgm_nuclear);
    if (!nuclear_game) pop_up_nuclear = "Find and Solve the Clue";

    if (dept_status_nuclear == Dept_nuclear) {
        // bool moving = false;
        // if (IsKeyDown(KEY_A)) { playerPos_nuclear.x -= 13; moving = true; }
        // if (IsKeyDown(KEY_D)) { playerPos_nuclear.x += 13; moving = true; }


        Vector2 offset_nuclear = walk_character_dept();
        playerPos_nuclear.x += offset_nuclear.x;

        if (moving && !walk_music_playing_nuclear) {
            PlayMusicStream(walk_music);
            walk_music_playing_nuclear = true;
        } else if (!moving && walk_music_playing_nuclear) {
            StopMusicStream(walk_music);
            walk_music_playing_nuclear = false;
        }
        if (walk_music_playing_nuclear) UpdateMusicStream(walk_music);

        bool eKeyHandled = false;
        if (CheckCollisionPointCircle(GetScreenToWorld2D(GetMousePosition(), camera_nuclear), game_zone_nuclear, 100.0f)) {
            // pop_up_nuclear = "Press E to Solve"; // Visual feedback
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)&& !nuclear_game && !show_rules_popup_nuclear) {
                PlaySound(pop_up_sound);
                PlaySound(click_sound);
                show_rules_popup_nuclear = true;
                show_ok_button_nuclear = true;
                eKeyHandled = true;
            }
        }

        if (CheckCollisionCircles(playerPos_nuclear, 50.0f, exit_zone_nuclear, 50.0f)) {
            pop_up_nuclear = "Press E to Exit";
            if (IsKeyPressed(KEY_E)) {
                PlaySound(click_sound);
                // unload_minesweeper();
                eKeyHandled = true;
                state_of_game = LAYER_MAP;
            } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(error_sound);
            }
        }

        if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) && !eKeyHandled) {
            PlaySound(error_sound);
        }
    } else if (dept_status_nuclear == Game_nuclear) {
        logic_minesweeper();
        if (nuclear_game || IsKeyDown(KEY_X)) {
            dept_status_nuclear = Dept_nuclear;
            game_pop_up_nuclear = "nuclear Conqured!! Abort";
            nuclear_game = true;
            PlaySound(conquered_sound);
            // unload_minesweeper();
        }
        if (IsKeyDown(KEY_Q)) {
            // unload_minesweeper();
            dept_status_nuclear = Dept_nuclear;
        }
    }

    if (playerPos_nuclear.x <= -20) playerPos_nuclear.x = -20;
    else if (playerPos_nuclear.x >= bg_image_nuclear.width * scale) playerPos_nuclear.x = bg_image_nuclear.width * scale;

    scale = (float)GetMonitorHeight(0) / bg_image_nuclear.height;
    float scaledWidth = bg_image_nuclear.width * scale;
    float cam = playerPos_nuclear.x;
    float halfScreen = screenWidth / 2.0f;
    if (cam < halfScreen) cam = halfScreen;
    if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;
    camera_nuclear.target = (Vector2){ cam, screenHeight / 2.0f };

    BeginMode2D(camera_nuclear);
    scale = (float)GetMonitorHeight(0) / bg_image_nuclear.height;
    DrawTextureEx(bg_image_nuclear, (Vector2){0, 0}, 0.0f, scale, WHITE);
    // DrawTexture(character, playerPos_nuclear.x, playerPos_nuclear.y, WHITE);
    draw_char_dept(playerPos_nuclear,scale);
    // DrawCircleV(game_zone_nuclear, 20, RED);
    // DrawCircleV(exit_zone_nuclear, 20, GREEN);
    // DrawCircleV(playerPos_nuclear, 20, BLUE);
    EndMode2D();

    if (dept_status_nuclear == Game_nuclear) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    }

    if (dept_status_nuclear == Game_nuclear) {
        draw_minesweeper();
    }

    DrawText(game_pop_up_nuclear.c_str(), 20, screenHeight - 100, 20, GREEN);

    if (nuclear_game && CheckCollisionCircles(playerPos_nuclear, 50.0f, exit_zone_nuclear, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!nuclear_game) {
        DrawText(pop_up_nuclear.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    if (show_rules_popup_nuclear) {
        int screenW = screenWidth;
        int screenH = screenHeight;

        DrawRectangle(screenW / 2 - 220, screenH / 2 - 100, 440, 200, Fade(BLACK, 0.9f));
        DrawRectangleLines(screenW / 2 - 220, screenH / 2 - 100, 440, 200, LIGHTGRAY);

        DrawText(game_rules_nuclear.c_str(), screenW / 2 - MeasureText(game_rules_nuclear.c_str(), 20) / 2, screenH / 2 - 60, 20, RAYWHITE);

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

        if (show_ok_button_nuclear) {
            if (CheckCollisionPointRec(mouse, okBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(click_sound);
                dept_status_nuclear = Game_nuclear;
                show_rules_popup_nuclear = false;
                show_ok_button_nuclear = false;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(error_sound);
        }
    }

    loading_screen("Department Of nuclear\nUniversity of Dhaka");
}

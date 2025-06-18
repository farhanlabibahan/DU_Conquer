
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
bool game_win_nuclear2;
Camera2D camera_nuclear = {0};
Vector2 playerPos_nuclear = {-20, 410};
Vector2 game_zone_nuclear = {1200,700};
Vector2 exit_zone_nuclear = {50,700};
string pop_up_nuclear = "Find and Solve the Clue";
string game_pop_up_nuclear = " ";
string game_rules_nuclear = "Lights On Game Rules:\nTurn on all the lights to win.\nPress X to exit the game.";
bool walk_music_playing = false;
bool show_rules_popup = false;
bool show_ok_button = false;

void init_nuclear() {
    SetMusicVolume(bgm_nuclear, 0.13f);
    PlayMusicStream(bgm_nuclear);
    SetMusicVolume(walk_music, 1.0f);
    bg_image_nuclear = LoadTexture("resources/HISTORY_EX.png");
    scale = (float)GetMonitorHeight(0) / bg_image_nuclear.height;
    float y_pos_floor = (float)GetMonitorHeight(0) - scale * 350;
    playerPos_nuclear = (Vector2){-10, y_pos_floor};
    game_zone_nuclear = {scale * 1200, y_pos_floor};
    exit_zone_nuclear = {scale * 10, y_pos_floor};

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
    if (!game_win_nuclear2) pop_up_nuclear = "Find and Solve the Clue";

    if (dept_status_nuclear == Dept_nuclear) {
        bool moving = false;
        if (IsKeyDown(KEY_A)) { playerPos_nuclear.x -= 13; moving = true; }
        if (IsKeyDown(KEY_D)) { playerPos_nuclear.x += 13; moving = true; }

        if (moving && !walk_music_playing) {
            PlayMusicStream(walk_music);
            walk_music_playing = true;
        } else if (!moving && walk_music_playing) {
            StopMusicStream(walk_music);
            walk_music_playing = false;
        }
        if (walk_music_playing) UpdateMusicStream(walk_music);

        bool eKeyHandled = false;
        if (CheckCollisionCircles(playerPos_nuclear, 50.0f, game_zone_nuclear, 50.0f)) {
            pop_up_nuclear = "Press E to Solve";
            if (IsKeyPressed(KEY_E) && !game_win_nuclear2 && !show_rules_popup) {
                PlaySound(pop_up_sound);
                PlaySound(click_sound);
                show_rules_popup = true;
                show_ok_button = true;
                eKeyHandled = true;
            }
        }

        if (CheckCollisionCircles(playerPos_nuclear, 50.0f, exit_zone_nuclear, 50.0f)) {
            pop_up_nuclear = "Press E to Exit";
            if (IsKeyPressed(KEY_E)) {
                PlaySound(click_sound);
                unload_minesweeper();
                eKeyHandled = true;
                state_of_game = LAYER_MAP;
            } else if (IsKeyPressed(KEY_E)) {
                PlaySound(error_sound);
            }
        }

        if (IsKeyPressed(KEY_E) && !eKeyHandled) {
            PlaySound(error_sound);
        }
    } else if (dept_status_nuclear == Game_nuclear) {
        logic_minesweeper();
        if (game_win_nuclear2 || IsKeyDown(KEY_X)) {
            dept_status_nuclear = Dept_nuclear;
            game_pop_up_nuclear = "nuclear Conqured!! Abort";
            game_win_nuclear2 = true;
            PlaySound(conquered_sound);
            unload_minesweeper();
        }
        if (IsKeyDown(KEY_Q)) {
            unload_minesweeper();
        }
    }

    if (playerPos_nuclear.x <= -20) playerPos_nuclear.x = -20;
    else if (playerPos_nuclear.x >= bg_image_nuclear.width * scale) playerPos_nuclear.x = bg_image_nuclear.width * scale;

    scale = (float)GetMonitorHeight(0) / bg_image_nuclear.height;
    float scaledWidth = bg_image_nuclear.width * scale;
    float cam = playerPos_nuclear.x;
    float halfScreen = GetMonitorWidth(0) / 2.0f;
    if (cam < halfScreen) cam = halfScreen;
    if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;
    camera_nuclear.target = (Vector2){ cam, screenHeight / 2.0f };

    BeginMode2D(camera_nuclear);
    scale = (float)GetMonitorHeight(0) / bg_image_nuclear.height;
    DrawTextureEx(bg_image_nuclear, (Vector2){0, 0}, 0.0f, scale, WHITE);
    DrawTexture(character, playerPos_nuclear.x, playerPos_nuclear.y, WHITE);
    DrawCircleV(game_zone_nuclear, 20, RED);
    DrawCircleV(exit_zone_nuclear, 20, GREEN);
    DrawCircleV(playerPos_nuclear, 20, BLUE);
    EndMode2D();

    if (dept_status_nuclear == Game_nuclear || !game_pop_up_nuclear.empty()) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    }

    if (dept_status_nuclear == Game_nuclear) {
        draw_minesweeper();
    }

    DrawText(game_pop_up_nuclear.c_str(), 20, screenHeight - 100, 20, GREEN);

    if (game_win_nuclear2 && CheckCollisionCircles(playerPos_nuclear, 50.0f, exit_zone_nuclear, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!game_win_nuclear2) {
        DrawText(pop_up_nuclear.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    if (show_rules_popup) {
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

        if (show_ok_button && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, okBtn)) {
                PlaySound(click_sound);
                dept_status_nuclear = Game_nuclear;
                show_rules_popup = false;
                show_ok_button = false;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(error_sound);
        }
    }

    loading_screen("Department Of nuclear\nUniversity of Dhaka");
}

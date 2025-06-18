#include "raylib.h"
#include <string>
#include "loading.h"
#include "genetics.h"
#include "global.h"
#include "minesweeper.h"
using namespace std;

typedef enum {
    Dept_genetics,
    Game_genetics
} dept_state_genetics;

dept_state_genetics dept_status_genetics = Dept_genetics;
Texture2D bg_image_genetics;
bool game_win_genetics2;
Camera2D camera_genetics = {0};
Vector2 playerPos_genetics = {-20, 410};
Vector2 game_zone_genetics = {1200,700};
Vector2 exit_zone_genetics = {50,700};
string pop_up_genetics = "Find and Solve the Clue";
string game_pop_up_genetics = " ";
string game_rules_genetics = "Lights On Game Rules:\nTurn on all the lights to win.\nPress X to exit the game.";
bool walk_music_playing = false;
bool show_rules_popup = false;
bool show_ok_button = false;

void init_genetics() {
    SetMusicVolume(bgm_genetics, 0.13f);
    PlayMusicStream(bgm_genetics);
    SetMusicVolume(walk_music, 1.0f);
    bg_image_genetics = LoadTexture("resources/HISTORY_EX.png");
    scale = (float)GetMonitorHeight(0) / bg_image_genetics.height;
    float y_pos_floor = (float)GetMonitorHeight(0) - scale * 350;
    playerPos_genetics = (Vector2){-10, y_pos_floor};
    game_zone_genetics = {scale * 1200, y_pos_floor};
    exit_zone_genetics = {scale * 10, y_pos_floor};

    camera_genetics.target = playerPos_genetics;
    camera_genetics.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera_genetics.rotation = 0.0f;
    camera_genetics.zoom = 1.0f;

    init_minesweeper();
}

void unload_genetics() {
    unload_minesweeper();
    UnloadTexture(bg_image_genetics);
}

void logic_draw_genetics() {
    UpdateMusicStream(bgm_genetics);
    if (!game_win_genetics2) pop_up_genetics = "Find and Solve the Clue";

    if (dept_status_genetics == Dept_genetics) {
        bool moving = false;
        if (IsKeyDown(KEY_A)) { playerPos_genetics.x -= 13; moving = true; }
        if (IsKeyDown(KEY_D)) { playerPos_genetics.x += 13; moving = true; }

        if (moving && !walk_music_playing) {
            PlayMusicStream(walk_music);
            walk_music_playing = true;
        } else if (!moving && walk_music_playing) {
            StopMusicStream(walk_music);
            walk_music_playing = false;
        }
        if (walk_music_playing) UpdateMusicStream(walk_music);

        bool eKeyHandled = false;
        if (CheckCollisionCircles(playerPos_genetics, 50.0f, game_zone_genetics, 50.0f)) {
            pop_up_genetics = "Press E to Solve";
            if (IsKeyPressed(KEY_E) && !game_win_genetics2 && !show_rules_popup) {
                PlaySound(pop_up_sound);
                PlaySound(click_sound);
                show_rules_popup = true;
                show_ok_button = true;
                eKeyHandled = true;
            }
        }

        if (CheckCollisionCircles(playerPos_genetics, 50.0f, exit_zone_genetics, 50.0f)) {
            pop_up_genetics = "Press E to Exit";
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
    } else if (dept_status_genetics == Game_genetics) {
        logic_minesweeper();
        if (game_win_genetics2 || IsKeyDown(KEY_X)) {
            dept_status_genetics = Dept_genetics;
            game_pop_up_genetics = "genetics Conqured!! Abort";
            game_win_genetics2 = true;
            PlaySound(conquered_sound);
            unload_minesweeper();
        }
        if (IsKeyDown(KEY_Q)) {
            unload_minesweeper();
            dept_status_genetics = Dept_genetics;
        }
    }

    if (playerPos_genetics.x <= -20) playerPos_genetics.x = -20;
    else if (playerPos_genetics.x >= bg_image_genetics.width * scale) playerPos_genetics.x = bg_image_genetics.width * scale;

    scale = (float)GetMonitorHeight(0) / bg_image_genetics.height;
    float scaledWidth = bg_image_genetics.width * scale;
    float cam = playerPos_genetics.x;
    float halfScreen = screenWidth / 2.0f;
    if (cam < halfScreen) cam = halfScreen;
    if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;
    camera_genetics.target = (Vector2){ cam, screenHeight / 2.0f };

    BeginMode2D(camera_genetics);
    scale = (float)GetMonitorHeight(0) / bg_image_genetics.height;
    DrawTextureEx(bg_image_genetics, (Vector2){0, 0}, 0.0f, scale, WHITE);
    DrawTexture(character, playerPos_genetics.x, playerPos_genetics.y, WHITE);
    DrawCircleV(game_zone_genetics, 20, RED);
    DrawCircleV(exit_zone_genetics, 20, GREEN);
    DrawCircleV(playerPos_genetics, 20, BLUE);
    EndMode2D();

    if (dept_status_genetics == Game_genetics) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    }

    if (dept_status_genetics == Game_genetics) {
        draw_minesweeper();
    }

    DrawText(game_pop_up_genetics.c_str(), 20, screenHeight - 100, 20, GREEN);

    if (game_win_genetics2 && CheckCollisionCircles(playerPos_genetics, 50.0f, exit_zone_genetics, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!game_win_genetics2) {
        DrawText(pop_up_genetics.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    if (show_rules_popup) {
        int screenW = screenWidth;
        int screenH = screenHeight;

        DrawRectangle(screenW / 2 - 220, screenH / 2 - 100, 440, 200, Fade(BLACK, 0.9f));
        DrawRectangleLines(screenW / 2 - 220, screenH / 2 - 100, 440, 200, LIGHTGRAY);

        DrawText(game_rules_genetics.c_str(), screenW / 2 - MeasureText(game_rules_genetics.c_str(), 20) / 2, screenH / 2 - 60, 20, RAYWHITE);

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
                dept_status_genetics = Game_genetics;
                show_rules_popup = false;
                show_ok_button = false;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(error_sound);
        }
    }

    loading_screen("Department Of genetics\nUniversity of Dhaka");
}

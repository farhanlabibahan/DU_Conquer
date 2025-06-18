#include "raylib.h"
#include <string>
#include "loading.h"
#include "japanesse.h"
#include "global.h"
#include "minesweeper.h"
using namespace std;

typedef enum {
    Dept_japanesse,
    Game_japanesse
} dept_state_japanesse;

dept_state_japanesse dept_status_japanesse = Dept_japanesse;
Texture2D bg_image_japanesse;
bool game_win_japanesse2;
Camera2D camera_japanesse = {0};
Vector2 playerPos_japanesse = {-20, 410};
Vector2 game_zone_japanesse = {1200,700};
Vector2 exit_zone_japanesse = {50,700};
string pop_up_japanesse = "Find and Solve the Clue";
string game_pop_up_japanesse = " ";
string game_rules_japanesse = "Lights On Game Rules:\nTurn on all the lights to win.\nPress X to exit the game.";
bool walk_music_playing = false;
bool show_rules_popup = false;
bool show_ok_button = false;

void init_japanesse() {
    SetMusicVolume(bgm_japanesse, 0.13f);
    PlayMusicStream(bgm_japanesse);
    SetMusicVolume(walk_music, 1.0f);
    bg_image_japanesse = LoadTexture("resources/HISTORY_EX.png");
    scale = (float)GetMonitorHeight(0) / bg_image_japanesse.height;
    float y_pos_floor = (float)GetMonitorHeight(0) - scale * 350;
    playerPos_japanesse = (Vector2){-10, y_pos_floor};
    game_zone_japanesse = {scale * 1200, y_pos_floor};
    exit_zone_japanesse = {scale * 10, y_pos_floor};

    camera_japanesse.target = playerPos_japanesse;
    camera_japanesse.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera_japanesse.rotation = 0.0f;
    camera_japanesse.zoom = 1.0f;

    init_minesweeper();
}

void unload_japanesse() {
    unload_minesweeper();
    UnloadTexture(bg_image_japanesse);
}

void logic_draw_japanesse() {
    UpdateMusicStream(bgm_japanesse);
    if (!game_win_japanesse2) pop_up_japanesse = "Find and Solve the Clue";

    if (dept_status_japanesse == Dept_japanesse) {
        bool moving = false;
        if (IsKeyDown(KEY_A)) { playerPos_japanesse.x -= 13; moving = true; }
        if (IsKeyDown(KEY_D)) { playerPos_japanesse.x += 13; moving = true; }

        if (moving && !walk_music_playing) {
            PlayMusicStream(walk_music);
            walk_music_playing = true;
        } else if (!moving && walk_music_playing) {
            StopMusicStream(walk_music);
            walk_music_playing = false;
        }
        if (walk_music_playing) UpdateMusicStream(walk_music);

        bool eKeyHandled = false;
        if (CheckCollisionCircles(playerPos_japanesse, 50.0f, game_zone_japanesse, 50.0f)) {
            pop_up_japanesse = "Press E to Solve";
            if (IsKeyPressed(KEY_E) && !game_win_japanesse2 && !show_rules_popup) {
                PlaySound(pop_up_sound);
                PlaySound(click_sound);
                show_rules_popup = true;
                show_ok_button = true;
                eKeyHandled = true;
            }
        }

        if (CheckCollisionCircles(playerPos_japanesse, 50.0f, exit_zone_japanesse, 50.0f)) {
            pop_up_japanesse = "Press E to Exit";
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
    } else if (dept_status_japanesse == Game_japanesse) {
        logic_minesweeper();
        if (game_win_japanesse2 || IsKeyDown(KEY_X)) {
            dept_status_japanesse = Dept_japanesse;
            game_pop_up_japanesse = "japanesse Conqured!! Abort";
            game_win_japanesse2 = true;
            PlaySound(conquered_sound);
            unload_minesweeper();
        }
        if (IsKeyDown(KEY_Q)) {
            unload_minesweeper();
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
    DrawTexture(character, playerPos_japanesse.x, playerPos_japanesse.y, WHITE);
    DrawCircleV(game_zone_japanesse, 20, RED);
    DrawCircleV(exit_zone_japanesse, 20, GREEN);
    DrawCircleV(playerPos_japanesse, 20, BLUE);
    EndMode2D();

    if (dept_status_japanesse == Game_japanesse) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    }

    if (dept_status_japanesse == Game_japanesse) {
        draw_minesweeper();
    }

    DrawText(game_pop_up_japanesse.c_str(), 20, screenHeight - 100, 20, GREEN);

    if (game_win_japanesse2 && CheckCollisionCircles(playerPos_japanesse, 50.0f, exit_zone_japanesse, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!game_win_japanesse2) {
        DrawText(pop_up_japanesse.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    if (show_rules_popup) {
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

        if (show_ok_button && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, okBtn)) {
                PlaySound(click_sound);
                dept_status_japanesse = Game_japanesse;
                show_rules_popup = false;
                show_ok_button = false;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(error_sound);
        }
    }

    loading_screen("Department Of japanesse\nUniversity of Dhaka");
}

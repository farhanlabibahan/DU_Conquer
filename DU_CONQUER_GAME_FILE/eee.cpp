#include "raylib.h"
#include <string>
#include "loading.h"
#include "eee.h"
#include "global.h"
#include "lights_on.h"
using namespace std;

typedef enum {
    Dept_eee,
    Game_eee
} dept_state_eee;

dept_state_eee dept_status_eee = Dept_eee;
Texture2D bg_image_eee;
// bool eee_game_2;
Camera2D camera_eee = {0};
Vector2 playerPos_eee;
Vector2 game_zone_eee;
Vector2 exit_zone_eee;
string pop_up_eee = "Find and Solve the Clue";
string game_pop_up_eee = " ";
string game_rules_eee = "Turn on all lights.\nChase from top";
bool walk_music_playing_eee = false;
bool show_rules_popup_eee = false;
bool show_ok_button_eee = false;

void init_eee() {
    SetMusicVolume(bgm_eee, 0.13f);
    PlayMusicStream(bgm_eee);
    SetMusicVolume(walk_music, 1.0f);
    bg_image_eee = LoadTexture("resources/eeeeeee.png");
    scale = (float)GetScreenHeight() / bg_image_eee.height;
    playerPos_eee = {0*scale,screenHeight-400*scale};
    exit_zone_eee = {0*scale,screenHeight-400*scale};
    game_zone_eee = {2500*scale,screenHeight-400*scale};

    camera_eee.target = playerPos_eee;
    camera_eee.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera_eee.rotation = 0.0f;
    camera_eee.zoom = 1.0f;

    init_lights_on();
}

void unload_eee() {
    unload_lights_on();
    UnloadTexture(bg_image_eee);
}

void logic_draw_eee() {
    UpdateMusicStream(bgm_eee);
    if (!eee_game_2) pop_up_eee = "Server is out of elecricity";

    if (dept_status_eee == Dept_eee) {
        // bool moving = false;
        // if (IsKeyDown(KEY_A)) { playerPos_eee.x -= 13; moving = true; }
        // if (IsKeyDown(KEY_D)) { playerPos_eee.x += 13; moving = true; }


        Vector2 offset_eee = walk_character_dept();
        playerPos_eee.x += offset_eee.x;

        if (moving && !walk_music_playing_eee) {
            PlayMusicStream(walk_music);
            walk_music_playing_eee = true;
        } else if (!moving && walk_music_playing_eee) {
            StopMusicStream(walk_music);
            walk_music_playing_eee = false;
        }
        if (walk_music_playing_eee) UpdateMusicStream(walk_music);

        bool eKeyHandled = false;
        if (CheckCollisionPointCircle(GetScreenToWorld2D(GetMousePosition(), camera_eee), game_zone_eee, 100.0f)) {
            // pop_up_eee = "Press E to Solve"; // Visual feedback
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)&& !eee_game_2 && !show_rules_popup_eee) {
                PlaySound(pop_up_sound);
                PlaySound(click_sound);
                show_rules_popup_eee = true;
                show_ok_button_eee = true;
                eKeyHandled = true;
            }
        }

        if (CheckCollisionCircles(playerPos_eee, 50.0f, exit_zone_eee, 50.0f)) {
            pop_up_eee = "Press E to Exit";
            if (IsKeyPressed(KEY_E)) {
                PlaySound(click_sound);
                // unload_lights_on();
                eKeyHandled = true;
                state_of_game = LAYER_MAP;
            } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(error_sound);
            }
        }

        if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) && !eKeyHandled) {
            PlaySound(error_sound);
        }
    } else if (dept_status_eee == Game_eee) {
        logic_lights_on();
        if (eee_game_2 || IsKeyDown(KEY_X)) {
            dept_status_eee = Dept_eee;
            game_pop_up_eee = "eee Conqured!! Abort";
            eee_game_2 = true;
            PlaySound(conquered_sound);
            // unload_lights_on();
        }
        if (IsKeyDown(KEY_Q)) {
            // unload_lights_on();
            dept_status_eee = Dept_eee;
        }
    }

    if (playerPos_eee.x <= -20) playerPos_eee.x = -20;
    else if (playerPos_eee.x >= bg_image_eee.width * scale) playerPos_eee.x = bg_image_eee.width * scale;

    scale = (float)GetScreenHeight() / bg_image_eee.height;
    float scaledWidth = bg_image_eee.width * scale;
    float cam = playerPos_eee.x;
    float halfScreen = screenWidth / 2.0f;
    if (cam < halfScreen) cam = halfScreen;
    if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;
    camera_eee.target = (Vector2){ cam, screenHeight / 2.0f };

    BeginMode2D(camera_eee);
    scale = (float)GetScreenHeight() / bg_image_eee.height;
    DrawTextureEx(bg_image_eee, (Vector2){0, 0}, 0.0f, scale, WHITE);
    // DrawTexture(character, playerPos_eee.x, playerPos_eee.y, WHITE);
    draw_char_dept(playerPos_eee,scale);
    // DrawCircleV(game_zone_eee, 20, RED);
    // DrawCircleV(exit_zone_eee, 20, GREEN);
    // DrawCircleV(playerPos_eee, 20, BLUE);
    EndMode2D();

    if (dept_status_eee == Game_eee) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    }

    if (dept_status_eee == Game_eee) {
        draw_lights_on();
    }

    DrawText(game_pop_up_eee.c_str(), 20, screenHeight - 100, 20, GREEN);

    if (eee_game_2 && CheckCollisionCircles(playerPos_eee, 50.0f, exit_zone_eee, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!eee_game_2) {
        DrawText(pop_up_eee.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    if (show_rules_popup_eee) {
        int screenW = screenWidth;
        int screenH = screenHeight;

        DrawRectangle(screenW / 2 - 220, screenH / 2 - 100, 440, 200, Fade(BLACK, 0.9f));
        DrawRectangleLines(screenW / 2 - 220, screenH / 2 - 100, 440, 200, LIGHTGRAY);

        DrawText(game_rules_eee.c_str(), screenW / 2 - MeasureText(game_rules_eee.c_str(), 20) / 2, screenH / 2 - 60, 20, RAYWHITE);

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

        if (show_ok_button_eee) {
            if (CheckCollisionPointRec(mouse, okBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(click_sound);
                dept_status_eee = Game_eee;
                show_rules_popup_eee = false;
                show_ok_button_eee = false;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(error_sound);
        }
    }

    loading_screen("Department Of eee\nUniversity of Dhaka");
}

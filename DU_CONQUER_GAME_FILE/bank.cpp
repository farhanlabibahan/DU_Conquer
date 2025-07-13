#include "raylib.h"
#include <string>
#include "loading.h"
#include "bank.h"
#include "global.h"
using namespace std;

typedef enum {
    Dept_bank,
    Game_bank
} dept_state_bank;

dept_state_bank dept_status_bank = Dept_bank;
Texture2D bg_image_bank,cheque;
// bool bank_game;
Camera2D camera_bank = {0};
Vector2 playerPos_bank;
Vector2 game_zone_bank;
Vector2 exit_zone_bank;
string pop_up_bank = "Find and Solve the Clue";
string game_pop_up_bank = " ";
string game_rules_bank = "Withdraw Money";
bool walk_music_playing_bank = false;
bool show_rules_popup_bank = false;
bool show_ok_button_bank = false;

void init_bank() {
    SetMusicVolume(bgm_bank, 0.13f);
    PlayMusicStream(bgm_bank);
    SetMusicVolume(walk_music, 1.0f);
    bg_image_bank = LoadTexture("resources/bank.png");
    cheque = LoadTexture("resources/cheque.png");
    scale = (float)GetScreenHeight() / bg_image_bank.height;
    playerPos_bank = {0*scale,screenHeight-400*scale};
    exit_zone_bank = {0*scale,screenHeight-400*scale};
    game_zone_bank = {2500*scale,screenHeight-400*scale};

    camera_bank.target = playerPos_bank;
    camera_bank.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera_bank.rotation = 0.0f;
    camera_bank.zoom = 1.0f;
}

void unload_bank() {
    UnloadTexture(bg_image_bank);
    // Removed UnloadTexture(cheque); to keep cheque texture in memory
}

void logic_draw_bank() {
    scale = (float)GetScreenHeight() / bg_image_bank.height;
    UpdateMusicStream(bgm_bank);
    if (!bank_game) pop_up_bank = "Money Needs to be counted";

    if (dept_status_bank == Dept_bank) {
        // bool moving = false;
        // if (IsKeyDown(KEY_A)) { playerPos_bank.x -= 13; moving = true; }
        // if (IsKeyDown(KEY_D)) { playerPos_bank.x += 13; moving = true; }


        Vector2 offset_bank = walk_character_dept();
        playerPos_bank.x += offset_bank.x;

        if (moving && !walk_music_playing_bank) {
            PlayMusicStream(walk_music);
            walk_music_playing_bank = true;
        } else if (!moving && walk_music_playing_bank) {
            StopMusicStream(walk_music);
            walk_music_playing_bank = false;
        }
        if (walk_music_playing_bank) UpdateMusicStream(walk_music);

        bool eKeyHandled = false;
        if (CheckCollisionPointCircle(GetScreenToWorld2D(GetMousePosition(), camera_bank), game_zone_bank, 100.0f)) {
            // pop_up_bank = "Press E to Solve"; // Visual feedback
            
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)&& !bank_game && !show_rules_popup_bank) {
                PlaySound(pop_up_sound);
                PlaySound(click_sound);
                show_rules_popup_bank = true;
                show_ok_button_bank = true;
                eKeyHandled = true;
            }
        }

        if (CheckCollisionCircles(playerPos_bank, 50.0f, exit_zone_bank, 50.0f)) {
            pop_up_bank = "Press E to Exit";
            if (IsKeyPressed(KEY_E)) {
                PlaySound(click_sound);
    
                eKeyHandled = true;
                state_of_game = LAYER_MAP;
            } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(error_sound);
            }
        }

        if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) && !eKeyHandled) {
            PlaySound(error_sound);
        }
    } else if (dept_status_bank == Game_bank) {
        int screenW = screenWidth;
        int screenH = screenHeight;
        // Removed drawing code from here to move it after EndMode2D()
    }

    if (playerPos_bank.x <= -20) playerPos_bank.x = -20;
    else if (playerPos_bank.x >= bg_image_bank.width * scale) playerPos_bank.x = bg_image_bank.width * scale;

    scale = (float)GetScreenHeight() / bg_image_bank.height;
    float scaledWidth = bg_image_bank.width * scale;
    float cam = playerPos_bank.x;
    float halfScreen = screenWidth / 2.0f;
    if (cam < halfScreen) cam = halfScreen;
    if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;
    camera_bank.target = (Vector2){ cam, screenHeight / 2.0f };

    BeginMode2D(camera_bank);
    scale = (float)GetScreenHeight() / bg_image_bank.height;
    DrawTextureEx(bg_image_bank, (Vector2){0, 0}, 0.0f, scale, WHITE);
    // DrawTexture(character, playerPos_bank.x, playerPos_bank.y, WHITE);
    draw_char_dept(playerPos_bank,scale);
    // DrawCircleV(game_zone_bank, 20, RED);
    // DrawCircleV(exit_zone_bank, 20, GREEN);
    // DrawCircleV(playerPos_bank, 20, BLUE);
    EndMode2D();

    if (dept_status_bank == Game_bank) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));

        int screenW = screenWidth;
        int screenH = screenHeight;

        DrawRectangle(screenW/2 - 300, screenH/2 - 150, 600, 300, Fade(BLACK, 0.8f));
        DrawTexture(cheque, screenW/2 - cheque.width/2, screenH/2 - cheque.height/2 - 20, WHITE);

        Rectangle withdrawBtn = { (float)(screenW/2 - 60), (float)(screenH/2 + 100), 170, 60 };
        Color btnColor = CheckCollisionPointRec(GetMousePosition(), withdrawBtn) ? RED : ORANGE;
        DrawRectangleRec(withdrawBtn, btnColor);
        Color withdraw = {0,0,0,120};
        DrawText("WITHDRAW", withdrawBtn.x + 10, withdrawBtn.y + 10, 20, WHITE);

        if (CheckCollisionPointRec(GetMousePosition(), withdrawBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(click_sound);
            bank_game = true;
            dept_status_bank = Dept_bank;
            game_pop_up_bank = "Cheque Withdrawn!";
            PlaySound(conquered_sound);
        }
    }

    // if (dept_status_bank == Game_bank) {
    //     draw_minesweeper();
    // }

    DrawText(game_pop_up_bank.c_str(), 20, screenHeight - 100, 20, GREEN);

    if (bank_game && CheckCollisionCircles(playerPos_bank, 50.0f, exit_zone_bank, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!bank_game) {
        DrawText(pop_up_bank.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    if (show_rules_popup_bank) {
        int screenW = screenWidth;
        int screenH = screenHeight;

        DrawRectangle(screenW / 2 - 220, screenH / 2 - 100, 440, 200, Fade(BLACK, 0.9f));
        DrawRectangleLines(screenW / 2 - 220, screenH / 2 - 100, 440, 200, LIGHTGRAY);

        DrawText(game_rules_bank.c_str(), screenW / 2 - MeasureText(game_rules_bank.c_str(), 20) / 2, screenH / 2 - 60, 20, RAYWHITE);

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

        if (show_ok_button_bank) {
            if (CheckCollisionPointRec(mouse, okBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(click_sound);
                dept_status_bank = Game_bank;
                show_rules_popup_bank = false;
                show_ok_button_bank = false;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(error_sound);
        }
    }

    loading_screen("Department Of bank\nUniversity of Dhaka");
}

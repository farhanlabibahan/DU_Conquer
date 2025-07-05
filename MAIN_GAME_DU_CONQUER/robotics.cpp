#include "raylib.h"
#include "robotics.h"
#include "robot.h"
#include <vector>
#include <algorithm>
#include "character_dept.h"
#include <random>
#include <ctime>
#include <string>
#include "global.h"
#include "loading.h"
using namespace std;
typedef enum {
    Dept_robotics,
    Game_robotics
} dept_state_robotics;

dept_state_robotics dept_status_robotics = Dept_robotics;


Texture2D bg_image_robotics;
Camera2D camera_robotics = {0};
Vector2 playerPos_robotics ;
Vector2 game_zone_robotics ;
Vector2 exit_zone_robotics ;
string pop_up_robotics = "Find and Solve the Clue";
string game_pop_up_robotics= " ";
string game_rules_robotics = "Match all the components to win\nPress X to exit the game.";
bool walk_music_playing_robotics = false;
bool show_rules_popup_robotics = false;
bool show_ok_button_robotics = false;

void init_robotics() {
   // SetMusicVolume(bgm_robotics, 0.13f);
  //  PlayMusicStream(bgm_robotics);
    SetMusicVolume(walk_music, 1.0f);
    bg_image_robotics = LoadTexture("resources/robotics_floor.png");
    scale = (float)GetMonitorHeight(0) / bg_image_robotics.height;
    //float y_pos_floor = (float)GetMonitorHeight(0) - scale * 350;
    playerPos_robotics = {4700*scale,screenHeight-400*scale};
    exit_zone_robotics = {4700*scale,screenHeight-400*scale};
    game_zone_robotics = {2500*scale,screenHeight-400*scale};
    camera_robotics.target = playerPos_robotics;
    camera_robotics.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera_robotics.rotation = 0.0f;
    camera_robotics.zoom = 1.0f;

    init_robot();
}

void unload_robotics() {
    unload_robot();
    UnloadTexture(bg_image_robotics);
}

void logic_draw_robotics() {
   UpdateMusicStream(bgm_robotics);
   if (!robotics_game) pop_up_robotics = "Find the hidden components";

    if (dept_status_robotics == Dept_robotics) {
        bool moving = false;
       // if (IsKeyDown(KEY_A)) { playerPos_robotics.x -= 13; moving = true; }
       // if (IsKeyDown(KEY_D)) { playerPos_robotics.x += 13; moving = true; }
       Vector2 offset_robotics = walk_character_dept();
        playerPos_robotics.x += offset_robotics.x;

        if (moving && !walk_music_playing_robotics) {
            PlayMusicStream(walk_music);
            walk_music_playing_robotics = true;
        } else if (!moving && walk_music_playing_robotics) {
            StopMusicStream(walk_music);
            walk_music_playing_robotics = false;
        }
        if (walk_music_playing_robotics) UpdateMusicStream(walk_music);

        bool eKeyHandled = false;
        if (CheckCollisionCircles(playerPos_robotics, 50.0f, game_zone_robotics, 50.0f)) {
          // pop_up_robotics = "Press E to Solve";
          if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !robotics_game && !show_rules_popup_robotics) {
                PlaySound(pop_up_sound);
                PlaySound(click_sound);
                show_rules_popup_robotics = true;
                show_ok_button_robotics = true;
                eKeyHandled = true;
            }
        }

        if (CheckCollisionCircles(playerPos_robotics, 50.0f, exit_zone_robotics, 50.0f)) {
           pop_up_robotics = "Press E to Exit";
            if (IsKeyPressed(KEY_E)) {
                PlaySound(click_sound);
                // unload_robot();
              eKeyHandled = true;
                state_of_game = LAYER_LIFT;
            } else if (IsKeyPressed(KEY_E)) {
                PlaySound(error_sound);
            }
        }

        if (IsKeyPressed(KEY_E) && !eKeyHandled) {
            PlaySound(error_sound);
        }
    } else if (dept_status_robotics == Game_robotics) {
        logic_robot();
        if (robotics_game || IsKeyDown(KEY_X)) {
            dept_status_robotics = Dept_robotics;
            game_pop_up_robotics = "You found the necessary components! Abort!";
            robotics_game = true;
            PlaySound(conquered_sound);
            // unload_robot();
        }
        if (IsKeyDown(KEY_Q)) {
            // unload_robot();
            dept_status_robotics = Dept_robotics;
        }
    }

    if (playerPos_robotics.x <= -20) playerPos_robotics.x = -20;
    else if (playerPos_robotics.x >= bg_image_robotics.width * scale) playerPos_robotics.x = bg_image_robotics.width ;
    scale = (float)GetMonitorHeight(0) / bg_image_robotics.height;
    float scaledWidth =bg_image_robotics.width * scale;
    float cam = playerPos_robotics.x;
    float halfScreen = screenWidth / 2.0f;
    if (cam < halfScreen) cam = halfScreen;
    if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;
    camera_robotics.target = (Vector2){ cam, screenHeight / 2.0f };

    BeginMode2D(camera_robotics);
    scale = (float)GetMonitorHeight(0) / bg_image_robotics.height;
    DrawTextureEx(bg_image_robotics, (Vector2){0, 0}, 0.0f, scale, WHITE);
   // DrawTexture(character, playerPos_robotics.x, playerPos_robotics.y, WHITE);
   // DrawCircleV(game_zone_robotics, 20, RED);
   // DrawCircleV(exit_zone_robotics, 20, GREEN);
   // DrawCircleV(playerPos_robotics, 20, BLUE);
  draw_char_dept(playerPos_robotics,scale);
    EndMode2D();

    if (dept_status_robotics == Game_robotics) {
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
    }

    if (dept_status_robotics == Game_robotics) {
        draw_robot();
    }

   DrawText(game_pop_up_robotics.c_str(), 20, screenHeight - 100, 20, GREEN);

    if (robotics_game && CheckCollisionCircles(playerPos_robotics, 50.0f, exit_zone_robotics, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!robotics_game) {
       DrawText(pop_up_robotics.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    if (show_rules_popup_robotics) {
        int screenW = screenWidth;
        int screenH = screenHeight;

        DrawRectangle(screenW / 2 - 220, screenH / 2 - 100, 440, 200, Fade(BLACK, 0.9f));
        DrawRectangleLines(screenW / 2 - 220, screenH / 2 - 100, 440, 200, LIGHTGRAY);

        DrawText(game_rules_robotics.c_str(), screenW / 2 - MeasureText(game_rules_robotics.c_str(), 20) / 2, screenH / 2 - 60, 20, RAYWHITE);

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

        if (show_ok_button_robotics && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (CheckCollisionPointRec(mouse, okBtn)) {
                PlaySound(click_sound);
                dept_status_robotics = Game_robotics;
                show_rules_popup_robotics = false;
                show_ok_button_robotics = false;
            }
        } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            PlaySound(error_sound);
        }
    }

    loading_screen("Department Of Robotics\nUniversity of Dhaka");
}

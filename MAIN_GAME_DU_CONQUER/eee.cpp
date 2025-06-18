#include "raylib.h"
#include <string>
#include "lights_on.h" //change here
#include "loading.h"
#include "eee.h"   // change here
#include "global.h"
using namespace std;
// whole block
typedef enum {
    Dept_eee,
    Game_eee
} dept_state;

Texture2D bg_image_eee;
dept_state dept_status = Dept_eee;
bool game_win_eee2;  
Camera2D camera_eee = {0};
Vector2 playerPos = {-20, 410};
Vector2 game_zone = {1200,700};
Vector2 exit_zone = {50,700};
string pop_up_eee = "Find and Solve the Clue";
string game_pop_up_eee = " ";
string game_rules_eee = "Lights On Game Rules:\nTurn on all the lights to win.\nPress X to exit the game.";

// block ends

bool walk_music_playing = false;
bool show_rules_popup = false;
bool show_ok_button = false;


void init_eee()
{  // change
    SetMusicVolume(bgm_eee, 0.13f);//change
    PlayMusicStream(bgm_eee);//change
    SetMusicVolume(walk_music, 1.0f);
    bg_image_eee = LoadTexture("resources/EEE_EX.png"); //change
    scale = (float)GetMonitorHeight(0) / bg_image_eee.height; // change
    float y_pos_floor = (float)GetMonitorHeight(0) - scale*350;
    playerPos = (Vector2){-10, y_pos_floor};
    
    game_zone = {scale*1200,y_pos_floor}; 
    exit_zone = {scale*10,y_pos_floor}; 
    camera_eee.target = playerPos;  // change
    camera_eee.offset = (Vector2){ screenWidth / 2.0f, screenHeight / 2.0f };// change
    camera_eee.rotation = 0.0f;// change
    camera_eee.zoom = 1.0f;// change
    InitLightsOn(); //change
}

void unload_eee()
{
    UnloadLightsOn();
    UnloadTexture(bg_image_eee);  // change
}

void logic_draw_eee() //change
{
        UpdateMusicStream(bgm_eee); //change
        if(!game_win_eee2) pop_up_eee = "Find and Solve the Clue"; //change
        if (dept_status == Dept_eee) // change
        {
            bool moving = false;
            if (IsKeyDown(KEY_A)) {
                playerPos.x -= 3;
                moving = true;
            }
            if (IsKeyDown(KEY_D)) {
                playerPos.x += 3;
                moving = true;
            }

            if (moving && !walk_music_playing) {
                PlayMusicStream(walk_music);
                walk_music_playing = true;
            }
            else if (!moving && walk_music_playing) {
                StopMusicStream(walk_music);
                walk_music_playing = false;
            }

            if (walk_music_playing) {
                UpdateMusicStream(walk_music);
            }

            bool eKeyHandled = false;

            if (CheckCollisionCircles(playerPos, 50.0f,game_zone, 50.0f)) {
                pop_up_eee = "Press E to Solve";  //change
                if(IsKeyPressed(KEY_E) && !game_win_eee2 && !show_rules_popup) //change
                {
                    PlaySound(pop_up_sound);
                    PlaySound(click_sound);
                    show_rules_popup = true;
                    show_ok_button = true;
                    eKeyHandled = true;
                }
            }

            if (CheckCollisionCircles(playerPos, 50.0f,exit_zone, 50.0f)) {
                pop_up_eee = "Press E to Exit"; // change
                if(IsKeyPressed(KEY_E))
                {
                    PlaySound(click_sound);
                    UnloadLightsOn();
                    eKeyHandled = true;
                    state_of_game = LAYER_MAP;
                }
                else if(IsKeyPressed(KEY_E)) {
                    PlaySound(error_sound);
                }
            }

            if (IsKeyPressed(KEY_E) && !eKeyHandled) {
                PlaySound(error_sound);
            }
        } 
        else if(dept_status == Game_eee) // change
        {
            UpdateLightsOn();  // change
            if (checkLightsOnWinStatus() || IsKeyDown(KEY_X)) {
                dept_status = Dept_eee;  // change
                game_pop_up_eee = "EEE Conqured!! Abort"; // change
                game_win_eee2 = true;  // change
                PlaySound(conquered_sound);
                UnloadLightsOn();  // change
            }

            if(IsKeyDown(KEY_Q))
            {
                UnloadLightsOn();  //change
            }
        }

        if(playerPos.x<=-20) playerPos.x = -20;
        else if(playerPos.x>=bg_image_eee.width*scale) playerPos.x = bg_image_eee.width*scale;  // change
    
        scale = (float)GetMonitorHeight(0) / bg_image_eee.height;  // change
        float scaledWidth = bg_image_eee.width * scale;  // change

        float cam = playerPos.x;

        float halfScreen = GetMonitorWidth(0) / 2.0f;

        if (cam < halfScreen) cam = halfScreen;
        if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;

        camera_eee.target = (Vector2){ cam, screenHeight / 2.0f};  // change

        BeginMode2D(camera_eee);  // change

        scale = (float)GetMonitorHeight(0) / bg_image_eee.height;  // change
        DrawTextureEx(bg_image_eee, (Vector2){0, 0}, 0.0f, scale, WHITE); // change
        DrawTexture(character, playerPos.x, playerPos.y, WHITE);
        DrawCircleV(game_zone, 20, RED);
        DrawCircleV(exit_zone, 20, GREEN);
        DrawCircleV(playerPos, 20, BLUE);
        EndMode2D();

        
        if (dept_status == Game_eee) DrawLightsOn();  // change

        DrawText(game_pop_up_eee.c_str(), 20, screenHeight-100, 20, GREEN);   // change

        if (game_win_eee2 && CheckCollisionCircles(playerPos, 50.0f, exit_zone, 50.0f)) {  // change
            DrawText("Press E to Exit", 20, screenHeight-70, 20, RAYWHITE);
        } else if (!game_win_eee2) { // change
            DrawText(pop_up_eee.c_str(), 20, screenHeight-50, 20, RAYWHITE);  // change
        }

        if (show_rules_popup) {
            int screenW = screenWidth;
            int screenH = screenHeight;

            // Draw box behind popup
            DrawRectangle(screenW / 2 - 220, screenH / 2 - 100, 440, 200, Fade(BLACK, 0.9f));
            DrawRectangleLines(screenW / 2 - 220, screenH / 2 - 100, 440, 200, LIGHTGRAY);

            DrawText(game_rules_eee.c_str(), screenW/2 - MeasureText(game_rules_eee.c_str(), 20)/2, screenH/2 - 60, 20, RAYWHITE);  // change

            Rectangle okBtn = { screenW/2 - 50, screenH/2 + 30, 100, 40 };
            Vector2 mouse = GetMousePosition();
            Color btnColor = CheckCollisionPointRec(mouse, okBtn) ? RED : DARKGRAY;
            static bool wasHovering = false;
            bool isHovering = CheckCollisionPointRec(mouse, okBtn);
            if (isHovering && !wasHovering) {
                PlaySound(click_sound);
            }
            wasHovering = isHovering;
            DrawRectangleRec(okBtn, btnColor);
            DrawText("OK", screenW/2 - MeasureText("OK", 20)/2, screenH/2 + 40, 20, WHITE);

            if (show_ok_button && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, okBtn)) {
                    PlaySound(click_sound);
                    dept_status = Game_eee;  // change
                    show_rules_popup = false;
                    show_ok_button = false;
                }
            }
            else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                PlaySound(error_sound);
            }

            
        }

        loading_screen("Department Of EEE\nUniversity of Dhaka");   // change
    }


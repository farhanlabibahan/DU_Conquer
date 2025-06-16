#include "raylib.h"
#include <string>
#include "lights_on.h"
#include "global.h"
using namespace std;

typedef enum {
    Dept,
    Game
} dept_state;

Sound conquered_sound,pop_up_sound;
Sound click_sound, error_sound;
Music bgm_eee;
Music walk_music;
bool walk_music_playing = false;

dept_state dept_status = Dept;
bool game_win_eee2;

Texture2D character,bg_image_eee;

// Edit these
Vector2 playerPos = {-20, 410};
Vector2 game_zone = {1200,700};
Vector2 exit_zone = {50,700};
string pop_up = "Find and Solve the Clue";
string game_pop_up = " ";
string game_rules = "Lights On Game Rules:\nTurn on all the lights to win.\nPress X to exit the game.";

Camera2D camera_eee = {0};

bool show_rules_popup = false;
bool show_ok_button = false;


void init_eee()
{
    // InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "DU_Conquer/EEE DEPT");
    // InitAudioDevice();
    conquered_sound = LoadSound("resources/conquered.mp3"); 
    pop_up_sound = LoadSound("resources/pop_up.mp3"); 
    click_sound = LoadSound("resources/click.mp3");
    error_sound = LoadSound("resources/error.mp3");
    bgm_eee = LoadMusicStream("resources/bgm_eee.mp3");
    SetMusicVolume(bgm_eee, 0.13f);
    PlayMusicStream(bgm_eee);
    walk_music = LoadMusicStream("resources/walk.mp3");
    SetMusicVolume(walk_music, 1.0f);
    character = LoadTexture("resources/character.png");
    bg_image_eee = LoadTexture("resources/EEE_EX.png");
    scale = (float)GetMonitorHeight(0) / bg_image_eee.height;
    float y_pos_floor = (float)GetMonitorHeight(0) - scale*350;
    playerPos = (Vector2){-10, y_pos_floor};
    
    game_zone = {scale*1200,y_pos_floor}; 
    exit_zone = {scale*10,y_pos_floor}; 
    camera_eee.target = playerPos;  
    camera_eee.offset = (Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f };
    camera_eee.rotation = 0.0f;
    camera_eee.zoom = 1.0f;
    ToggleFullscreen();
    SetTargetFPS(60);
    InitLightsOn();
}

void unload_eee()
{
    UnloadLightsOn();
    UnloadTexture(character);
    UnloadTexture(bg_image_eee);
    UnloadSound(pop_up_sound);
    UnloadSound(conquered_sound);
    UnloadSound(click_sound);
    UnloadSound(error_sound);
    UnloadMusicStream(bgm_eee);
    UnloadMusicStream(walk_music);
    // CloseAudioDevice();
    // CloseWindow();
}

void logic_draw_eee()
{
    // while (!WindowShouldClose()) 
    // {
        UpdateMusicStream(bgm_eee);
        if(!game_win_eee2) pop_up = "Find and Solve the Clue";
        if (dept_status == Dept) 
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
                pop_up = "Press E to Solve";
                if(IsKeyPressed(KEY_E) && !game_win_eee2 && !show_rules_popup)
                {
                    PlaySound(pop_up_sound);
                    PlaySound(click_sound);
                    show_rules_popup = true;
                    show_ok_button = true;
                    eKeyHandled = true;
                }
            }

            if (CheckCollisionCircles(playerPos, 50.0f,exit_zone, 50.0f)) {
                pop_up = "Press E to Exit";
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
        else if(dept_status == Game)
        {
            UpdateLightsOn();
            if (checkLightsOnWinStatus() || IsKeyDown(KEY_X)) {
                dept_status = Dept;
                game_pop_up = "EEE Conqured!! Abort";
                game_win_eee2 = true;
                PlaySound(conquered_sound);
                UnloadLightsOn();
            }

            if(IsKeyDown(KEY_Q))
            {
                UnloadLightsOn();
            }
        }

        if(playerPos.x<=-20) playerPos.x = -20;
        else if(playerPos.x>=bg_image_eee.width) playerPos.x = bg_image_eee.width;
    
        scale = (float)GetMonitorHeight(0) / bg_image_eee.height;
        float scaledWidth = bg_image_eee.width * scale;

        float cam = playerPos.x;

        float halfScreen = GetMonitorWidth(0) / 2.0f;

        if (cam < halfScreen) cam = halfScreen;
        if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;

        camera_eee.target = (Vector2){ cam, GetScreenHeight() / 2.0f};

        // BeginDrawing();
        // ClearBackground(BLACK);

        BeginMode2D(camera_eee);

        scale = (float)GetMonitorHeight(0) / bg_image_eee.height;
        // float scaledWidth = image.width * scale;
        DrawTextureEx(bg_image_eee, (Vector2){0, 0}, 0.0f, scale, WHITE);
        DrawTexture(character, playerPos.x, playerPos.y, WHITE);
        DrawCircleV(game_zone, 20, RED);
        DrawCircleV(exit_zone, 20, GREEN);
        DrawCircleV(playerPos, 20, BLUE);
        EndMode2D();

        
        if (dept_status == Game) DrawLightsOn();

        DrawText(game_pop_up.c_str(), 20, GetMonitorHeight(0)-100, 20, GREEN);

        if (game_win_eee2 && CheckCollisionCircles(playerPos, 50.0f, exit_zone, 50.0f)) {
            DrawText("Press E to Exit", 20, GetMonitorHeight(0)-70, 20, RAYWHITE);
        } else if (!game_win_eee2) {
            DrawText(pop_up.c_str(), 20, GetMonitorHeight(0)-50, 20, RAYWHITE);
        }

        if (show_rules_popup) {
            int screenW = GetMonitorWidth(0);
            int screenH = GetMonitorHeight(0);

            // Draw box behind popup
            DrawRectangle(screenW / 2 - 220, screenH / 2 - 100, 440, 200, Fade(BLACK, 0.9f));
            DrawRectangleLines(screenW / 2 - 220, screenH / 2 - 100, 440, 200, LIGHTGRAY);

            DrawText(game_rules.c_str(), screenW/2 - MeasureText(game_rules.c_str(), 20)/2, screenH/2 - 60, 20, RAYWHITE);

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
                    dept_status = Game;
                    show_rules_popup = false;
                    show_ok_button = false;
                }
            }
            else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                PlaySound(error_sound);
            }

            
        }

        // EndDrawing();
    }
// }


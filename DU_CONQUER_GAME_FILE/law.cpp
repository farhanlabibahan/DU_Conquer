#include "raylib.h"
#include <string>
#include "loading.h"
#include "law.h"
#include "global.h"
using namespace std;

typedef enum {
    Dept_law,
    Game_law
} dept_state_law;

dept_state_law dept_status_law = Dept_law;
Texture2D bg_image_law;
// bool law_game;
Camera2D camera_law = {0};
Vector2 playerPos_law;
Vector2 game_zone_law;
Vector2 exit_zone_law;
string pop_up_law = "Find and Solve the Clue";
string game_pop_up_law = " ";
string game_rules_law = "Law Rule:\nEveryone is innocent until proven guilty.\nClick OK to judge the case.";
bool walk_music_playing_law = false;
bool show_rules_popup_law = false;
bool show_ok_button_law = false;

enum QuizStep {
    RULE,
    SCENARIO,
    RESULT
};

QuizStep quiz_step = RULE;
string quiz_rule_text = "Law Rule:\nEveryone is innocent until proven guilty.";
string quiz_scenario_text = "Scenario:\nA person is accused of theft.\nWhat is your judgment?";
bool quiz_answer_correct = false;
string quiz_result_text = "";

void init_law() {
    SetMusicVolume(bgm_law, 0.13f);
    PlayMusicStream(bgm_law);
    SetMusicVolume(walk_music, 1.0f);
    bg_image_law = LoadTexture("resources/law.png");
    scale = (float)GetScreenHeight() / bg_image_law.height;
    playerPos_law = {0*scale,screenHeight-400*scale};
    exit_zone_law = {0*scale,screenHeight-400*scale};
    game_zone_law = {2500*scale,screenHeight-400*scale};

    camera_law.target = playerPos_law;
    camera_law.offset = (Vector2){screenWidth / 2.0f, screenHeight / 2.0f};
    camera_law.rotation = 0.0f;
    camera_law.zoom = 1.0f;
}

void unload_law() {
    UnloadTexture(bg_image_law);
}

void logic_draw_law() {
    UpdateMusicStream(bgm_law);
    if (!law_game) pop_up_law = "Where is the judge?";

    if (dept_status_law == Dept_law) {
        Vector2 offset_law = walk_character_dept();
        playerPos_law.x += offset_law.x;

        if (moving && !walk_music_playing_law) {
            PlayMusicStream(walk_music);
            walk_music_playing_law = true;
        } else if (!moving && walk_music_playing_law) {
            StopMusicStream(walk_music);
            walk_music_playing_law = false;
        }
        if (walk_music_playing_law) UpdateMusicStream(walk_music);

        bool eKeyHandled = false;
        if (CheckCollisionPointCircle(GetScreenToWorld2D(GetMousePosition(), camera_law), game_zone_law, 100.0f)) {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !law_game) {
                PlaySound(pop_up_sound);
                dept_status_law = Game_law;
                PlaySound(click_sound);
                eKeyHandled = true;
            }
        }

        if (CheckCollisionCircles(playerPos_law, 50.0f, exit_zone_law, 50.0f)) {
            if (law_game) {
                pop_up_law = "Press E to Exit";
                if (IsKeyPressed(KEY_E)) {
                    PlaySound(click_sound);
                    eKeyHandled = true;
                    state_of_game = LAYER_MAP;
                } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    PlaySound(error_sound);
                }
            } else {
                pop_up_law = "Solve the quiz to exit";
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    PlaySound(error_sound);
                }
            }
        }

        if ((IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) && !eKeyHandled) {
            PlaySound(error_sound);
        }
    } else if (dept_status_law == Game_law) {
        // Draw the overlay only during the quiz
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.6f));
        int screenW = screenWidth;
        int screenH = screenHeight;

        DrawRectangle(screenW / 2 - 250, screenH / 2 - 150, 500, 300, Fade(BLACK, 0.9f));
        DrawRectangleLines(screenW / 2 - 250, screenH / 2 - 150, 500, 300, LIGHTGRAY);

        if (quiz_step == RULE) {
            DrawText(quiz_rule_text.c_str(), screenW / 2 - MeasureText(quiz_rule_text.c_str(), 20) / 2, screenH / 2 - 100, 20, RAYWHITE);

            Rectangle okBtn = { screenW / 2 - 50, screenH / 2 + 50, 100, 40 };
            Vector2 mouse = GetMousePosition();
            Color btnColor = CheckCollisionPointRec(mouse, okBtn) ? RED : DARKGRAY;
            static bool wasHovering = false;
            bool isHovering = CheckCollisionPointRec(mouse, okBtn);
            if (isHovering && !wasHovering) {
                PlaySound(click_sound);
            }
            wasHovering = isHovering;
            DrawRectangleRec(okBtn, btnColor);
            DrawText("Next", screenW / 2 - MeasureText("Next", 20) / 2, screenH / 2 + 60, 20, WHITE);

            if (CheckCollisionPointRec(mouse, okBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(click_sound);
                quiz_step = SCENARIO;
            }
        } else if (quiz_step == SCENARIO) {
            DrawText(quiz_scenario_text.c_str(), screenW / 2 - MeasureText("Scenario:", 20) / 2, screenH / 2 - 110, 20, RAYWHITE);

            Rectangle guiltyBtn = { screenW / 2 - 180, screenH / 2 + 30, 150, 50 };
            Rectangle notGuiltyBtn = { screenW / 2 + 30, screenH / 2 + 30, 150, 50 };
            Vector2 mouse = GetMousePosition();

            Color guiltyColor = CheckCollisionPointRec(mouse, guiltyBtn) ? RED : DARKGRAY;
            Color notGuiltyColor = CheckCollisionPointRec(mouse, notGuiltyBtn) ? RED : DARKGRAY;

            static bool wasHoveringGuilty = false;
            static bool wasHoveringNotGuilty = false;
            bool isHoveringGuilty = CheckCollisionPointRec(mouse, guiltyBtn);
            bool isHoveringNotGuilty = CheckCollisionPointRec(mouse, notGuiltyBtn);

            if ((isHoveringGuilty && !wasHoveringGuilty) || (isHoveringNotGuilty && !wasHoveringNotGuilty)) {
                PlaySound(click_sound);
            }
            wasHoveringGuilty = isHoveringGuilty;
            wasHoveringNotGuilty = isHoveringNotGuilty;

            DrawRectangleRec(guiltyBtn, guiltyColor);
            DrawText("Guilty", guiltyBtn.x + (guiltyBtn.width - MeasureText("Guilty", 20)) / 2, guiltyBtn.y + 15, 20, WHITE);

            DrawRectangleRec(notGuiltyBtn, notGuiltyColor);
            DrawText("Not Guilty", notGuiltyBtn.x + (notGuiltyBtn.width - MeasureText("Not Guilty", 20)) / 2, notGuiltyBtn.y + 15, 20, WHITE);

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (CheckCollisionPointRec(mouse, guiltyBtn)) {
                    quiz_answer_correct = false;
                    quiz_result_text = "Wrong Judgment. Try again.";
                    quiz_step = RESULT;
                    PlaySound(error_sound);
                } else if (CheckCollisionPointRec(mouse, notGuiltyBtn)) {
                    quiz_answer_correct = true;
                    quiz_result_text = "Correct Judgment!";
                    quiz_step = RESULT;
                    PlaySound(conquered_sound);
                    law_game = true;
                }
            }
        } else if (quiz_step == RESULT) {
            DrawText(quiz_result_text.c_str(), screenW / 2 - MeasureText(quiz_result_text.c_str(), 20) / 2, screenH / 2 - 30, 20, RAYWHITE);

            Rectangle okBtn = { screenW / 2 - 50, screenH / 2 + 50, 100, 40 };
            Vector2 mouse = GetMousePosition();
            Color btnColor = CheckCollisionPointRec(mouse, okBtn) ? RED : DARKGRAY;
            static bool wasHovering = false;
            bool isHovering = CheckCollisionPointRec(mouse, okBtn);
            if (isHovering && !wasHovering) {
                PlaySound(click_sound);
            }
            wasHovering = isHovering;
            DrawRectangleRec(okBtn, btnColor);
            DrawText("OK", screenW / 2 - MeasureText("OK", 20) / 2, screenH / 2 + 60, 20, WHITE);

            if (CheckCollisionPointRec(mouse, okBtn) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                PlaySound(click_sound);
                if (quiz_answer_correct) {
                    dept_status_law = Dept_law;
                    quiz_step = RULE;
                } else {
                    quiz_step = SCENARIO;
                }
            }
        }
    }

    if (playerPos_law.x <= -20) playerPos_law.x = -20;
    else if (playerPos_law.x >= bg_image_law.width * scale) playerPos_law.x = bg_image_law.width * scale;

    scale = (float)GetScreenHeight() / bg_image_law.height;
    float scaledWidth = bg_image_law.width * scale;
    float cam = playerPos_law.x;
    float halfScreen = screenWidth / 2.0f;
    if (cam < halfScreen) cam = halfScreen;
    if (cam > scaledWidth - halfScreen) cam = scaledWidth - halfScreen;
    camera_law.target = (Vector2){ cam, screenHeight / 2.0f };

    BeginMode2D(camera_law);
    scale = (float)GetScreenHeight() / bg_image_law.height;
    DrawTextureEx(bg_image_law, (Vector2){0, 0}, 0.0f, scale, WHITE);
    draw_char_dept(playerPos_law,scale);
    EndMode2D();

    // Overlay is now drawn only inside Game_law block above.

    DrawText(game_pop_up_law.c_str(), 20, screenHeight - 100, 20, GREEN);

    if (law_game && CheckCollisionCircles(playerPos_law, 50.0f, exit_zone_law, 50.0f)) {
        DrawText("Press E to Exit", 20, screenHeight - 70, 20, RAYWHITE);
    } else if (!law_game) {
        DrawText(pop_up_law.c_str(), 20, screenHeight - 50, 20, RAYWHITE);
    }

    DrawText(TextFormat("Law State: %d", dept_status_law), 20, 20, 20, RED);

    loading_screen("Department Of law\nUniversity of Dhaka");
}

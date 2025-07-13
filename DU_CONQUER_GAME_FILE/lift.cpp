#include "lift.h"
#include "global.h"
#include "gear.h"
#include <string>
#include <vector>
#include <cmath>

Texture2D lift_image, lift_behind;
Sound lift_ting_sound;
Music warning_lift_bgm;
int offsetX = 550, offsetY = 450;
double currentFloor = 0;
int wantedFloor = 0;
bool isInMotionLift = false;
double liftSpeed = 0.005;
int showOnScreen = 0;
bool lift_pop_up_show = false;
std::string pop_up_lift = "Press E to Exit the Elevator";
bool oneFloorPassed = false;

Vector2 exitZoneLift;
Vector2 playerPos_lift;

float scrollOffset = 0.0f; 

std::vector<liftButton> liftButtons;

liftButton::liftButton(int num, float x, float y) : floor_num(num) {
    buttonRect = { x, y, 30 * scale, 30 * scale };
}

float alpha_lift = 0.5;
int alpha_up = 1;
void lift_sabotage_draw()
{
    alpha_lift += 0.005 * alpha_up;
    if(alpha_lift > 0.7f) alpha_up = -1;
    else if(alpha_lift < 0.4f) alpha_up = 1;

    Color lift_alert = Fade(RED, alpha_lift);
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), lift_alert);
}

void liftButton::DrawButton() {
    Color btn_color = isPressed ? RED : WHITE;

    Vector2 center = { buttonRect.x + buttonRect.width / 2, buttonRect.y + buttonRect.height / 2 };
    float radius = buttonRect.width / 2;
    DrawCircleV(center, radius + 3 * scale, DARKGRAY); 
    DrawCircleV(center, radius, isPressed ? ORANGE : SKYBLUE);  
    DrawText(TextFormat("%d", floor_num),
             center.x - 5 * scale,
             center.y - 6 * scale,
             15 * scale,
             WHITE);
}

bool liftButton::isLiftButtonPressed() {
    Vector2 mouse_pos = GetMousePosition();

    if (CheckCollisionPointRec(mouse_pos, buttonRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        isPressed = true;
        wantedFloor = floor_num;
        return true;
    }

    return false;
}

void init_lift() {
    init_gear();
    lift_image = LoadTexture("resources/lift.png");
    lift_behind = LoadTexture("resources/EEE_EX.png");
    lift_ting_sound = LoadSound("resources/lift_ting.mp3");
    warning_lift_bgm = LoadMusicStream("resources/warning_lift.mp3");
    playerPos_lift = { 0, screenHeight - 400 * scale };
    exitZoneLift = { 0, screenHeight - 400 * scale };

    scrollOffset = 0.0f;

    liftButtons.clear();
    offsetX = 640 * scale;
    offsetY = 650 * scale;

    const int buttonHeight = 30;
    const int buttonSpacing = 20;

    int floors[] = {0, 2, 6, 7, 9};
    for (int i = 0; i < 5; i++) {
        float btnY = (offsetY + i * (buttonHeight + buttonSpacing)) * scale;
        liftButtons.push_back(liftButton(floors[i], offsetX * scale, btnY));
    }
}

void unload_lift() {
    UnloadTexture(lift_image);
    UnloadTexture(lift_behind);
    UnloadSound(lift_ting_sound);
    UnloadMusicStream(warning_lift_bgm);
    unload_gear();
}

void CheckExitZone() {
    if (!isInMotionLift && CheckCollisionCircles(playerPos_lift, 50.0f, exitZoneLift, 50.0f)) {
        lift_pop_up_show = true;

        if (IsKeyPressed(KEY_E)) {
            if (showOnScreen == 0) state_of_game = LAYER_MAP;
            else if (showOnScreen == 6) state_of_game = LAYER_CSE;
            else if (showOnScreen == 2) state_of_game = LAYER_MICROBIOLOGY;
            else if (showOnScreen == 7) state_of_game = LAYER_GENETICS;
            else if (showOnScreen == 9) state_of_game = LAYER_ROBOTICS;
        }
    } else {
        lift_pop_up_show = false;
    }
}

void logic_draw_lift() {
    if (!IsMusicStreamPlaying(bgm_eee)) {
        PlayMusicStream(bgm_eee);
    }
    UpdateMusicStream(bgm_eee);

    scale = screenHeight/lift_image.height;
    Vector2 offset_lift = walk_character_dept();
    playerPos_lift.x += offset_lift.x;

    scale = (float)GetScreenHeight() / lift_image.height;

    float drawScale = scale;
    float texHeight = lift_behind.height * drawScale;
    float scrollSpeed = 2.0f;

    if (isInMotionLift) {
        double direction = wantedFloor - currentFloor;
        if (direction > 0) {
            scrollOffset += scrollSpeed; 
        } else {
            scrollOffset -= scrollSpeed;  
        }

        if (scrollOffset >= texHeight)
            scrollOffset -= texHeight;
        else if (scrollOffset <= -texHeight)
            scrollOffset += texHeight;
    }

    DrawTextureEx(lift_behind, (Vector2){ 0, scrollOffset }, 0.0f, drawScale, WHITE);
    DrawTextureEx(lift_behind, (Vector2){ 0, scrollOffset - texHeight }, 0.0f, drawScale, WHITE);
    DrawTextureEx(lift_behind, (Vector2){ 0, scrollOffset + texHeight }, 0.0f, drawScale, WHITE);
    DrawTextureEx(lift_image, (Vector2){ 0, 0 }, 0.0f, scale, WHITE);


    for (auto& btn : liftButtons) {

        if (!isInMotionLift) 
        {
            if(btn.isLiftButtonPressed()) oneFloorPassed = true;
        }
        
        btn.DrawButton();
    }

    //Lift Movement
    if (fabs(currentFloor - wantedFloor) < 0.01) {
        currentFloor = wantedFloor;
        showOnScreen = currentFloor;
        isInMotionLift = false;
        PlaySound(lift_ting_sound);
        for (auto& btn : liftButtons) btn.isPressed = false;
    } else {
        double direction = wantedFloor - currentFloor;
        if (direction > 0) direction = 1;
        else direction = -1;

        currentFloor += liftSpeed * direction;
        isInMotionLift = true;

        showOnScreen = (int)floor(currentFloor);
    }

    DrawText(TextFormat("Floor: %d", showOnScreen),
             (offsetX+120) * scale,
             (offsetY-450) * scale,
             40 * scale, WHITE);

    draw_char_dept(playerPos_lift, scale);

    if (lift_pop_up_show && !isInMotionLift) {
        DrawText(pop_up_lift.c_str(), 20 * scale, screenHeight - 30 * scale, 20 * scale, WHITE);
    }

    if (oneFloorPassed && !lift_sabotage && !gear_game) {
        if (!IsMusicStreamPlaying(warning_lift_bgm)) {
            PlayMusicStream(warning_lift_bgm);
        }
        UpdateMusicStream(warning_lift_bgm);

        lift_sabotage_draw();
        logic_gear();
        draw_gear();

        DrawText("ops! Elevator has run into trouble\nPlease fix the gears!", 20 * scale, screenHeight - 50 * scale, 18 * scale, WHITE);

        if (IsKeyPressed(KEY_X) || gear_game) {
            if (gear_game && !lift_sabotage) {
                PlaySound(conquered_sound);
            }
            lift_sabotage = true;
            lift_game = 1;
            StopMusicStream(warning_lift_bgm);
        }
    }

    if(lift_game) DrawText("LIFT Fixed", 20 * scale, screenHeight - 80 * scale, 22 * scale, GREEN);

    if(lift_sabotage) CheckExitZone();
}
#include "lift.h"
#include "global.h"
#include <string>
#include <vector>
#include <cmath>

Texture2D lift_image, lift_behind;
Sound lift_ting_sound;
int offsetX = 550, offsetY = 450;
double currentFloor = 1;
int wantedFloor = 1;
bool isInMotionLift = false;
double liftSpeed = 0.01;
int showOnScreen = 1;
bool lift_pop_up_show = false;
std::string pop_up_lift = "Press E to Exit the Elevator";

Vector2 exitZoneLift;
Vector2 playerPos_lift;

float scrollOffset = 0.0f; 

std::vector<liftButton> liftButtons;

liftButton::liftButton(int num, float x, float y) : floor_num(num) {
    buttonRect = { x, y, 30 * scale, 30 * scale };
}

void liftButton::DrawButton() {
    Color btn_color = isPressed ? RED : WHITE;
    DrawRectangleRec(buttonRect, GRAY);
    DrawText(TextFormat("%d", floor_num),
             buttonRect.x + 8 * scale,
             buttonRect.y + 5 * scale,
             15 * scale,
             btn_color);
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
    lift_image = LoadTexture("resources/lift.png");
    lift_behind = LoadTexture("resources/eeeeeee.png");
    lift_ting_sound = LoadSound("resources/lift_ting.mp3");
    playerPos_lift = { 0, screenHeight - 400 * scale };
    exitZoneLift = { 0, screenHeight - 400 * scale };

    scrollOffset = 0.0f;

    liftButtons.clear();
    offsetX = 600 * scale;
    offsetY = 120 * scale;

    const int buttonHeight = 30;
    const int buttonSpacing = 20;

    for (int i = 0; i < 3; i++) {
        int floor = (i == 0) ? 1 : (i == 1) ? 4 : 9;
        float btnY = (offsetY + i * (buttonHeight + buttonSpacing)) * scale;
        liftButtons.push_back(liftButton(floor, offsetX * scale, btnY));
    }
}

void unload_lift() {
    UnloadTexture(lift_image);
    UnloadTexture(lift_behind);
    UnloadSound(lift_ting_sound);
}

void CheckExitZone() {
    if (!isInMotionLift && CheckCollisionCircles(playerPos_lift, 50.0f, exitZoneLift, 50.0f)) {
        lift_pop_up_show = true;

        if (IsKeyPressed(KEY_E)) {
            if (showOnScreen == 1) state_of_game = LAYER_EEE;
            else if (showOnScreen == 4) state_of_game = LAYER_GENETICS;
            else if (showOnScreen == 9) state_of_game = LAYER_MICROBIOLOGY;
        }
    } else {
        lift_pop_up_show = false;
    }
}

void logic_draw_lift() {
    Vector2 offset_lift = walk_character_dept();
    playerPos_lift.x += offset_lift.x;

    scale = (float)GetMonitorHeight(0) / lift_image.height;

    float drawScale = 0.5f;
    float texHeight = lift_behind.height * drawScale;
    float scrollSpeed = 2.0f;

    if (isInMotionLift) {
        scrollOffset += scrollSpeed;
        if (scrollOffset >= texHeight)
            scrollOffset -= texHeight; 
    }

    DrawTextureEx(lift_behind, (Vector2){ 0, -scrollOffset }, 0.0f, drawScale, WHITE);
    DrawTextureEx(lift_behind, (Vector2){ 0, -scrollOffset + texHeight }, 0.0f, drawScale, WHITE);

    DrawTextureEx(lift_image, (Vector2){ 0, 0 }, 0.0f, scale, WHITE);

    // ---------- Lift Panel ----------
    Color panelColor = Color{30, 30, 30, 200};
    DrawRectangle(offsetX * scale, offsetY * scale, 100 * scale, 150 * scale, panelColor);

    for (auto& btn : liftButtons) {
        if (!isInMotionLift) btn.isLiftButtonPressed();
        btn.DrawButton();
    }

    // ---------- Lift Movement Logic ----------
    if (fabs(currentFloor - wantedFloor) < 0.01) {
        currentFloor = wantedFloor;
        showOnScreen = currentFloor;
        isInMotionLift = false;
        // no longer set showOnScreen here
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

    // ---------- Floor Display ----------
    DrawText(TextFormat("Floor: %d", showOnScreen),
             offsetX * scale + 40 * scale,
             (offsetY + 120) * scale,
             20 * scale, WHITE);

    // ---------- Character ----------
    draw_char_dept(playerPos_lift, scale);

    // ---------- Pop-Up ----------
    if (lift_pop_up_show && !isInMotionLift) {
        DrawText(pop_up_lift.c_str(), 20 * scale, screenHeight - 30 * scale, 20 * scale, WHITE);
    }

    // ---------- Exit Logic ----------
    CheckExitZone();
}
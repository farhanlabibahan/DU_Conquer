#include "raylib.h"
#include <math.h>
#include <stdlib.h>
#include <raymath.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define MINI_WIDTH 700
#define MINI_HEIGHT 500
#define SIGNAL_WIDTH 460
#define SIGNAL_HEIGHT 100

float targetFrequency, targetAmplitude, targetPhase;
float currentFrequency = 88.0f;
float currentAmplitude = 0.5f;
float currentPhase = 0.0f;

bool signalLocked = false;
bool signalFailed = false;
bool showPopup = false;

int totalTime = 45;
float timer = 0;
float resultPopupTimer = 0;

float noise(float t) {
    return 0.5f * ((float)rand() / RAND_MAX - 0.5f);
}

float getSignalStrength(float freq, float amp, float phase) {
    float freqMatch = 1.0f - fabsf(freq - targetFrequency) / 10.0f;
    float ampMatch = 1.0f - fabsf(amp - targetAmplitude);
    float phaseMatch = 1.0f - fabsf(phase - targetPhase) / PI;
    return Clamp(freqMatch, 0.0f, 1.0f) * Clamp(ampMatch, 0.0f, 1.0f) * Clamp(phaseMatch, 0.0f, 1.0f);
}

void DrawSignalWave(float strength, Rectangle area) {
    BeginScissorMode(area.x, area.y, area.width, area.height);
    Vector2 prev = {0};
    Color waveColor = (strength > 0.9f) ? GREEN : (strength > 0.6f) ? YELLOW : RED;

    for (int x = 0; x < area.width; x++) {
        float t = (float)x / area.width * 10.0f;
        float y = sinf(t * currentFrequency * 0.1f + currentPhase) * currentAmplitude;
        y += (1.0f - strength) * noise(t);
        y *= SIGNAL_HEIGHT * 0.5f;
        y += SIGNAL_HEIGHT / 2.0f;

        if (x > 0) {
            DrawLine((int)(prev.x + area.x), (int)(prev.y + area.y),
                     (int)(x + area.x), (int)(y + area.y), waveColor);
        }
        prev = (Vector2){x, y};
    }

    EndScissorMode();
}

float HandleRotaryKnob(Vector2 center, float value, float min, float max, const char *label) {
    DrawCircleV(center, 38, DARKGRAY);
    float angle = (value - min) / (max - min) * 270.0f - 135.0f;
    float rad = angle * DEG2RAD;

    for (int i = 0; i <= 10; i++) {
        float tickAngle = (-135 + i * 27) * DEG2RAD;
        Vector2 tickStart = { center.x + cosf(tickAngle) * 30, center.y + sinf(tickAngle) * 30 };
        Vector2 tickEnd =   { center.x + cosf(tickAngle) * 35, center.y + sinf(tickAngle) * 35 };
        DrawLineV(tickStart, tickEnd, LIGHTGRAY);
    }

    Vector2 needle = {
        center.x + cosf(rad) * 30,
        center.y + sinf(rad) * 30
    };
    DrawLineEx(center, needle, 4, ORANGE);
    DrawCircleV(center, 4, ORANGE);
    DrawText(label, center.x - MeasureText(label, 16)/2, center.y + 45, 16, WHITE);

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointCircle(GetMousePosition(), center, 40)) {
        Vector2 dir = Vector2Subtract(GetMousePosition(), center);
        float dragAngle = atan2f(dir.y, dir.x);
        float norm = (dragAngle + PI) / (2 * PI);
        float newValue = min + norm * (max - min);
        return Clamp(newValue, min, max);
    }

    return value;
}

float HandleSlider(Rectangle bar, float value, float min, float max) {
    DrawRectangleRec(bar, DARKGRAY);
    float knobX = bar.x + ((value - min) / (max - min)) * bar.width;
    DrawRectangle(knobX - 5, bar.y - 5, 10, bar.height + 10, ORANGE);
    DrawRectangleLinesEx(bar, 1, LIGHTGRAY);
    DrawText("Amplitude", bar.x, bar.y - 20, 16, WHITE);

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), bar)) {
        float ratio = (GetMouseX() - bar.x) / bar.width;
        float newValue = min + ratio * (max - min);
        return Clamp(newValue, min, max);
    }
    return value;
}

void DrawDigitalDisplay(Vector2 pos, float freq, float amp, float phase) {
    DrawRectangle(pos.x, pos.y, 160, 70, BLACK);
    DrawRectangleLines(pos.x, pos.y, 160, 70, SKYBLUE);
    DrawText(TextFormat("F: %.1f", freq), pos.x + 10, pos.y + 5, 20, GREEN);
    DrawText(TextFormat("A: %.2f", amp), pos.x + 10, pos.y + 25, 20, GREEN);
    DrawText(TextFormat("P: %.2f", phase), pos.x + 10, pos.y + 45, 20, GREEN);
}

void DrawSignalMatchBar(float strength, Vector2 pos, float width) {
    DrawRectangle(pos.x, pos.y, width, 20, (Color){40, 40, 40, 255});
    DrawRectangle(pos.x, pos.y, strength * width, 20, LIME);
    DrawRectangleLines(pos.x, pos.y, width, 20, WHITE);
    DrawText(TextFormat("%.0f%% Match", strength * 100), pos.x , pos.y+35, 20, WHITE);
}

void ResetMiniGame() {
    targetFrequency = 90.0f + (rand() % 201) / 10.0f;
    targetAmplitude = 0.4f + ((float)rand() / RAND_MAX) * 0.6f;
    targetPhase = -PI + ((float)rand() / RAND_MAX) * (2 * PI);

    currentFrequency = 88.0f;
    currentAmplitude = 0.5f;
    currentPhase = 0.0f;

    signalLocked = false;
    signalFailed = false;
    totalTime = 45;
    timer = 0;
    resultPopupTimer = 0;
    showPopup = true;
}

void PlaySignalSeekerMiniGame(Rectangle popupArea) {
    if (signalLocked || signalFailed) return;

    timer += GetFrameTime();
    if (timer >= 1.0f) {
        totalTime--;
        timer = 0;
    }

    if (totalTime <= 0) signalFailed = true;
    currentFrequency += sinf(GetTime() * 0.5f) * 0.002f;

    float strength = getSignalStrength(currentFrequency, currentAmplitude, currentPhase);
    if (strength > 0.96f) signalLocked = true;

    if (!showPopup) return;

    int px = popupArea.x + 30;
    int py = popupArea.y + 30;

    DrawRectangleRec(popupArea, DARKGRAY);
    DrawRectangleLinesEx(popupArea, 4, SKYBLUE);
    DrawText("SIGNAL SEEKER", px + 200, py, 28, WHITE);
    DrawText(TextFormat("Time Left: %d sec", totalTime), px + 440, py + 5, 20, RED);

    Rectangle waveArea = {px, py + 50, SIGNAL_WIDTH, SIGNAL_HEIGHT};
    DrawRectangleRec(waveArea, BLACK);
    DrawSignalWave(strength, waveArea);

    Vector2 barPos = {px, waveArea.y + waveArea.height + 50};
    DrawSignalMatchBar(strength, barPos, 400);

    int knobX = px + 420;
    int knobTopY = barPos.y + 60;

    currentFrequency = HandleRotaryKnob((Vector2){knobX+140, knobTopY+80}, currentFrequency, 80, 120, "Freq");
    currentPhase     = HandleRotaryKnob((Vector2){knobX, knobTopY + 80}, currentPhase, -PI, PI, "Phase");
    currentAmplitude = HandleSlider((Rectangle){px, knobTopY + 80, 300, 20}, currentAmplitude, 0.0f, 1.0f);
    DrawDigitalDisplay((Vector2){knobX+30, knobTopY-80 }, currentFrequency, currentAmplitude, currentPhase);
}

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "DU_Conquer - Signal Seeker");
    SetTargetFPS(60);

    Rectangle popup = {
        SCREEN_WIDTH / 2 - MINI_WIDTH / 2,
        SCREEN_HEIGHT / 2 - MINI_HEIGHT / 2,
        MINI_WIDTH,
        MINI_HEIGHT
    };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground((Color){10, 10, 30, 255});
        DrawText("Press E to enter Signal Seeker", 40, 40, 24, LIGHTGRAY);

        if (IsKeyPressed(KEY_E) && !showPopup && !signalLocked && !signalFailed) {
            ResetMiniGame();
        }

        // ✅ Restart on R key after win/loss
        if ((signalLocked || signalFailed) && IsKeyPressed(KEY_R)) {
            ResetMiniGame();
        }

        if (showPopup && !signalLocked && !signalFailed)
            PlaySignalSeekerMiniGame(popup);

        if ((signalLocked || signalFailed) && !showPopup) {
            DrawRectangle(SCREEN_WIDTH/2 - 260, SCREEN_HEIGHT/2 - 80, 520, 120, BLACK);
            DrawRectangleLines(SCREEN_WIDTH/2 - 260, SCREEN_HEIGHT/2 - 80, 520, 120, signalLocked ? GREEN : RED);
            DrawText(signalLocked ? "✅ SIGNAL LOCKED!" : "❌ SIGNAL LOST!",
                     SCREEN_WIDTH/2 - 180, SCREEN_HEIGHT/2 - 50, 32, signalLocked ? GREEN : RED);
            DrawText(signalLocked ? "Press R to play again." : "Press R to retry.",
                     SCREEN_WIDTH/2 - 160, SCREEN_HEIGHT/2, 20, LIGHTGRAY);
        }

        // ⏱ Auto-hide popup result after 2 seconds
        if ((signalLocked || signalFailed) && showPopup) {
            resultPopupTimer += GetFrameTime();
            if (resultPopupTimer > 2.0f) {
                showPopup = false;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

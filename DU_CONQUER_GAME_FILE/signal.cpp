#include "raylib.h"
#include <math.h>
#include <stdlib.h>
#include <raymath.h>
#include "signal.h"
#include "global.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define MINI_WIDTH 700
#define MINI_HEIGHT 500
#define SIGNAL_WIDTH 460
#define SIGNAL_HEIGHT 100


static Rectangle popup;

float targetFrequency, targetAmplitude, targetPhase;
float currentFrequency = 88.0f;
float currentAmplitude = 0.5f;
float currentPhase = 0.0f;

bool signalFailed = false;
bool showPopup = false;

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

void init_signal() {
    int monitorWidth = GetScreenWidth();
    int monitorHeight = GetScreenHeight();

    popup = (Rectangle){
        monitorWidth / 2 - MINI_WIDTH / 2,
        monitorHeight / 2 - MINI_HEIGHT / 2,
        MINI_WIDTH,
        MINI_HEIGHT
    };

    targetFrequency = 90.0f + (rand() % 201) / 10.0f;
    targetAmplitude = 0.4f + ((float)rand() / RAND_MAX) * 0.6f;
    targetPhase = -PI + ((float)rand() / RAND_MAX) * (2 * PI);

    currentFrequency = 88.0f;
    currentAmplitude = 0.5f;
    currentPhase = 0.0f;

    // eee_game_1 = false;
    signalFailed = false;
    showPopup = true;
}

void logic_signal() {
    if (eee_game_1) return;

    float strength = getSignalStrength(currentFrequency, currentAmplitude, currentPhase);
    if (strength > 0.96f) eee_game_1 = true;

    currentFrequency += sinf(GetTime() * 0.5f) * 0.002f;
}

void draw_signal() {
    if (!showPopup) return;

    int px = popup.x + 30;
    int py = popup.y + 30;

    float strength = getSignalStrength(currentFrequency, currentAmplitude, currentPhase);

    DrawRectangleRec(popup, DARKGRAY);
    DrawRectangleLinesEx(popup, 4, SKYBLUE);
    DrawText("SIGNAL SEEKER", px + 200, py, 28, WHITE);

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

void unload_signal() {
    // Stub for future asset cleanup
}

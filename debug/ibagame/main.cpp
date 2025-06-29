#include "raylib.h"
#include <string>
#include <vector>
#include <cmath>

// Business info
struct Business {
    const char *name;
    int baseCost;
    int baseRevenue;
    int basePopularity;
};

// Global constants
const int BUDGET = 25000;
const int PROFIT_TARGET = 50000;

// Game State
bool showGuide = false;
bool showBusinessPopup = false;
bool showStrategyPopup = false;
bool showPlanningPopup = false;
bool showExecutionPopup = false;
bool showResultPopup = false;
bool guideRead = false;

// Selections
int selectedBusiness = -1;
int selectedStrategy = -1;
int selectedPlanning = -1;
int selectedExecution = -1;

// Popup transition flags
bool justOpenedGuide = false;
bool justOpenedResult = false;

// Data
std::vector<Business> businesses = {
    {"T-Shirt Stall", 20000, 60000, 80},
    {"Juice Cart", 15000, 45000, 70},
    {"Notes App", 25000, 80000, 60}
};

const char *strategies[] = {
    "Marketing (+5k, +15 pop)",
    "Small Team (-5k, -15 pop)",
    "Smart Pricing (+2k, +15% rev)"
};

const char *planningOptions[] = {
    "Student Summer (+15 pop)",
    "Adult Winter (+15% rev)"
};

const char *executionOptions[] = {
    "Lean (-5k cost)",
    "Aggressive (+5k, +20% rev)"
};

// Result
int totalCost = 0;
int finalRevenue = 0;
int profit = 0;
bool metTarget = false;

void ResetGame() {
    showGuide = false;
    showBusinessPopup = false;
    showStrategyPopup = false;
    showPlanningPopup = false;
    showExecutionPopup = false;
    showResultPopup = false;
    guideRead = false;
    selectedBusiness = -1;
    selectedStrategy = -1;
    selectedPlanning = -1;
    selectedExecution = -1;
    justOpenedGuide = false;
    justOpenedResult = false;
    totalCost = 0;
    finalRevenue = 0;
    profit = 0;
    metTarget = false;
}

Rectangle Button(float x, float y, float w, float h, const char *label) {
    DrawRectangleRec({x, y, w, h}, LIGHTGRAY);
    DrawRectangleLines(x, y, w, h, DARKGRAY);
    DrawText(label, x + 10, y + 10, 20, DARKBLUE);
    return {x, y, w, h};
}

void CalculateResults() {
    const Business &b = businesses[selectedBusiness];
    totalCost = b.baseCost;
    float popularity = b.basePopularity;
    float revenue = b.baseRevenue;

    // Strategy
    switch (selectedStrategy) {
        case 0: totalCost += 5000; popularity += 15; break;
        case 1: totalCost -= 5000; popularity -= 15; break;
        case 2: totalCost += 2000; revenue *= 1.15f; break;
    }

    // Planning
    if (selectedPlanning == 0) popularity += 15;
    else revenue *= 1.15f;

    // Execution
    if (selectedExecution == 0) totalCost -= 5000;
    else { totalCost += 5000; revenue *= 1.2f; }

    if (popularity < 0) popularity = 0;
    revenue *= (popularity / 100.0f);
    finalRevenue = (int)revenue;
    profit = finalRevenue - totalCost;
    metTarget = (profit >= PROFIT_TARGET && totalCost <= BUDGET);
}

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_UNDECORATED);
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "IBA Startup Game");
    ToggleFullscreen();
    SetTargetFPS(60);

    Texture2D ibaRoom = LoadTexture("iba_room.png");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        float scaleX = (float)GetScreenWidth() / ibaRoom.width;
        float scaleY = (float)GetScreenHeight() / ibaRoom.height;
        float scale = fminf(scaleX, scaleY);
        DrawTexturePro(
            ibaRoom,
            (Rectangle){0, 0, (float)ibaRoom.width, (float)ibaRoom.height},
            (Rectangle){0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
            (Vector2){0, 0}, 0.0f, WHITE);

        Vector2 mouse = GetMousePosition();
        Vector2 logicalMouse = { mouse.x / scale, mouse.y / scale };

        // Debug: Draw visual outlines for clickable areas
        if (!guideRead) DrawRectangleLines(50 * scale, 100 * scale, 200 * scale, 50 * scale, RED);
        else if (selectedBusiness == -1) DrawRectangleLines(1000 * scale, 100 * scale, 200 * scale, 50 * scale, RED);

        if (!guideRead) {
            if (CheckCollisionPointRec(logicalMouse, {50, 100, 200, 50}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                showGuide = true;
                justOpenedGuide = true;
            }
        } else if (selectedBusiness == -1) {
            if (CheckCollisionPointRec(logicalMouse, {1000, 100, 200, 50}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                showBusinessPopup = true;
            }
        }

        if (showGuide) {
            DrawRectangle(100, 100, 1080, 500, Fade(LIGHTGRAY, 0.95f));
            DrawText("Welcome to the Startup Game!\nChoose a business, a strategy, a plan, and an execution method.", 130, 130, 24, DARKGRAY);
            if (justOpenedGuide) {
                justOpenedGuide = false;
            } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                showGuide = false;
                guideRead = true;
            }
        }

        if (showBusinessPopup && selectedBusiness == -1) {
            DrawRectangle(100, 100, 1080, 500, Fade(WHITE, 0.9f));
            DrawText("Choose Your Business:", 130, 130, 28, DARKBLUE);
            for (int i = 0; i < businesses.size(); i++) {
                if (CheckCollisionPointRec(logicalMouse, Button(150, 180 + i * 70, 400, 50, businesses[i].name)) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    selectedBusiness = i;
                    showStrategyPopup = true;
                }
            }
        } else if (showStrategyPopup && selectedStrategy == -1) {
            DrawRectangle(100, 100, 1080, 500, Fade(WHITE, 0.9f));
            DrawText("Pick a Strategy:", 130, 130, 28, DARKBLUE);
            for (int i = 0; i < 3; i++) {
                if (CheckCollisionPointRec(logicalMouse, Button(150, 180 + i * 70, 500, 50, strategies[i])) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    selectedStrategy = i;
                    showPlanningPopup = true;
                }
            }
        } else if (showPlanningPopup && selectedPlanning == -1) {
            DrawRectangle(100, 100, 1080, 500, Fade(WHITE, 0.9f));
            DrawText("Choose Planning:", 130, 130, 28, DARKBLUE);
            for (int i = 0; i < 2; i++) {
                if (CheckCollisionPointRec(mouse, Button(150, 180 + i * 70, 500, 50, planningOptions[i])) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    selectedPlanning = i;
                    showExecutionPopup = true;
                }
            }
        } else if (showExecutionPopup && selectedExecution == -1) {
            DrawRectangle(100, 100, 1080, 500, Fade(WHITE, 0.9f));
            DrawText("Pick Execution Style:", 130, 130, 28, DARKBLUE);
            for (int i = 0; i < 2; i++) {
                if (CheckCollisionPointRec(mouse, Button(150, 180 + i * 70, 500, 50, executionOptions[i])) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    selectedExecution = i;
                    CalculateResults();
                    showResultPopup = true;
                    justOpenedResult = true;
                }
            }
        } else if (showResultPopup) {
            DrawRectangle(200, 150, 880, 400, Fade(WHITE, 0.98f));
            DrawText(TextFormat("Total Cost: %d", totalCost), 250, 200, 28, DARKGRAY);
            DrawText(TextFormat("Revenue: %d", finalRevenue), 250, 250, 28, DARKGRAY);
            DrawText(TextFormat("Profit: %d", profit), 250, 300, 28, DARKGRAY);
            DrawText(metTarget ? "You Succeeded!" : "You Failed!", 250, 360, 36, metTarget ? GREEN : RED);
            if (justOpenedResult) {
                justOpenedResult = false;
            } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                ResetGame();
            }
        }

        EndDrawing();
    }

    UnloadTexture(ibaRoom);
    CloseWindow();
    return 0;
}

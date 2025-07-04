#include "raylib.h"
#include <string>
#include <vector>
#include <cmath>
#include "business.h"
#include "global.h"


// Business info
struct Business {
    const char *name;
    int baseCost;
    int baseRevenue;
    int basepopularity;
};

// Global constants
const int BUDGET = 25000;
const int PROFIT_TARGET = 50000;

// Game State
bool showGuide = false;
bool showBusinesspopup = false;
bool showStrategypopup = false;
bool showPlanningpopup = false;
bool showExecutionpopup = false;
bool showResultpopup = false;
bool guideRead = false;

// Selections
int selectedBusiness = -1;
int selectedStrategy = -1;
int selectedPlanning = -1;
int selectedExecution = -1;

// popup transition flags
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

float resultTimer = 0.0f;
bool resultDisplayed = false;

void init_business() {
    
    showBusinesspopup = false;
    showStrategypopup = false;
    showPlanningpopup = false;
    showExecutionpopup = false;
    showResultpopup = false;
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
    resultTimer = 0.0f;
    resultDisplayed = false;
}

Rectangle Button(float x, float y, float w, float h, const char *label) {
    DrawRectangleRec({x, y, w, h}, (Color){50, 0, 0, 255});
    DrawRectangleLines(x, y, w, h, RED);
    DrawText(label, x + 10, y + 10, 20, RED);
    return {x, y, w, h};
}

void logic_business() {
    const Business &b = businesses[selectedBusiness];
    totalCost = b.baseCost;
    float popularity = b.basepopularity;
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
void draw_business() {
    int popupWidth = 600;
    int popupHeight = 400;
    int popupX = screenWidth / 2 - popupWidth / 2;
    int popupY = screenHeight / 2 - popupHeight / 2;

    // Business Selection
    if (selectedBusiness == -1) {
        DrawRectangle(popupX, popupY, popupWidth, popupHeight, Fade(BLACK, 0.8f));
        DrawRectangleLines(popupX, popupY, popupWidth, popupHeight, DARKGRAY);
        DrawText("Choose Your Business:", popupX + 30, popupY + 30, 28, DARKBLUE);

        for (int i = 0; i < businesses.size(); i++) {
            Rectangle btn = Button(popupX + 50, popupY + 80 + i * 70, 400, 50, businesses[i].name);
            if (CheckCollisionPointRec(GetMousePosition(), btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                selectedBusiness = i;
                PlaySound(pop_up_sound);
                showStrategypopup = true;
            }
        }

    // Strategy Selection
    } else if (showStrategypopup && selectedStrategy == -1) {
        DrawRectangle(popupX, popupY, popupWidth, popupHeight, Fade(BLACK, 0.8f));
        DrawRectangleLines(popupX, popupY, popupWidth, popupHeight, DARKGRAY);
        DrawText("Pick a Strategy:", popupX + 30, popupY + 30, 28, DARKBLUE);

        for (int i = 0; i < 3; i++) {
            Rectangle btn = Button(popupX + 50, popupY + 80 + i * 70, 500, 50, strategies[i]);
            if (CheckCollisionPointRec(GetMousePosition(), btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                selectedStrategy = i;
                PlaySound(pop_up_sound);
                showPlanningpopup = true;
            }
        }

    // Planning Selection
    } else if (showPlanningpopup && selectedPlanning == -1) {
        DrawRectangle(popupX, popupY, popupWidth, popupHeight, Fade(BLACK, 0.8f));
        DrawRectangleLines(popupX, popupY, popupWidth, popupHeight, DARKGRAY);
        DrawText("Choose Planning:", popupX + 30, popupY + 30, 28, DARKBLUE);

        for (int i = 0; i < 2; i++) {
            Rectangle btn = Button(popupX + 50, popupY + 80 + i * 70, 500, 50, planningOptions[i]);
            if (CheckCollisionPointRec(GetMousePosition(), btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                selectedPlanning = i;
                PlaySound(pop_up_sound);
                showExecutionpopup = true;
            }
        }

    // Execution Selection
    } else if (showExecutionpopup && selectedExecution == -1) {
        DrawRectangle(popupX, popupY, popupWidth, popupHeight, Fade(BLACK, 0.8f));
        DrawRectangleLines(popupX, popupY, popupWidth, popupHeight, DARKGRAY);
        DrawText("Pick Execution Style:", popupX + 30, popupY + 30, 28, DARKBLUE);

        for (int i = 0; i < 2; i++) {
            Rectangle btn = Button(popupX + 50, popupY + 80 + i * 70, 500, 50, executionOptions[i]);
            if (CheckCollisionPointRec(GetMousePosition(), btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                selectedExecution = i;
                PlaySound(pop_up_sound);
                logic_business();
                showResultpopup = true;
                justOpenedResult = true;
            }
        }

    // Result Popup
    } else if (showResultpopup) {
        // Show loading spinner for 10 seconds, then show result
        extern bool bank_game;
        if (!resultDisplayed) {
            resultTimer += GetFrameTime();

            // Spinner animation
            DrawRectangle(popupX, popupY, popupWidth, popupHeight, Fade(BLACK, 0.8f));
            DrawText("Investing...", popupX + 200, popupY + 150, 28, WHITE);
            DrawCircleLines(popupX + 300, popupY + 220, 30, RED);
            DrawCircleSectorLines((Vector2){popupX + 300, popupY + 220}, 30, 0, resultTimer * 360, 20, RED);

            if (resultTimer >= 10.0f) {
                resultDisplayed = true;
                PlaySound(conquered_sound);
            }
        } else {
            DrawRectangle(popupX, popupY, popupWidth, popupHeight, Fade(BLACK, 0.8f));
            DrawRectangleLines(popupX, popupY, popupWidth, popupHeight, DARKGRAY);
            DrawText(TextFormat("Total Cost: %d", totalCost), popupX + 30, popupY + 50, 26, DARKGRAY);
            DrawText(TextFormat("Revenue: %d", finalRevenue), popupX + 30, popupY + 100, 26, DARKGRAY);
            DrawText(TextFormat("Profit: %d", profit), popupX + 30, popupY + 150, 26, DARKGRAY);

            if (!bank_game) {
                DrawText("Insufficient Funds!\n Withdraw money from Rh Bank", popupX + 30, popupY + 220, 34, RED);
            } else {
                DrawText(metTarget ? "You Succeeded!" : "You Failed!", popupX + 30, popupY + 220, 34, metTarget ? GREEN : RED);
            }

            Rectangle resetBtn = { popupX + popupWidth - 160, popupY + popupHeight - 60, 120, 40 };
            DrawRectangleRec(resetBtn, DARKGRAY);
            DrawText("Reset", resetBtn.x + 20, resetBtn.y + 10, 20, WHITE);

            if (!justOpenedResult && CheckCollisionPointRec(GetMousePosition(), resetBtn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                PlaySound(pop_up_sound);
                init_business();
            }

            justOpenedResult = false;
        }
    }
}

void unload_business(){
    
}
#include "raylib.h"
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>

const int screenWidth = 800;
const int screenHeight = 600;

// === GAME STATE ===
enum GamePhase { PHASE_START, PHASE_SHOES, PHASE_SNACKS, PHASE_CANDIDATE, PHASE_SUCCESS, PHASE_FAIL, PHASE_RESTART };
GamePhase currentPhase = PHASE_START;

// === ROUND 1: SHOE MEMORY ===
struct Shoe {
    Rectangle rect;
    Color color;
};
std::vector<Shoe> shoes;
int missingShoeIndex = -1;
bool round1Complete = false;

// === ROUND 2: SNACK MEMORY ===
struct SnackBox {
    Rectangle rect;
    std::string content;
    float revealAlpha = 1.0f; // For animation
};
std::vector<SnackBox> snackBoxes;
int correctSnackIndex = -1;
bool round2Complete = false;

// === ROUND 3: CANDIDATE MEMORY ===
struct Candidate {
    Rectangle rect;
    std::string name;
    Color color;
    std::string symbol;
};
std::vector<Candidate> candidates;
int chosenIndex = -1;
int correctCandidateIndex = -1;
bool round3Complete = false;

// === GENERAL ===
bool inputEnabled = false;
float phaseTimer = 0.0f;
float showTime = 3.0f;
float roundTimer = 0.0f;
const float maxRoundTime = 20.0f;
Rectangle startButton = { screenWidth/2 - 80, screenHeight/2 - 30, 160, 60 };

// === Animation Helpers ===
void AnimateSnackBoxes(float delta) {
    for (auto &box : snackBoxes) {
        if (phaseTimer >= showTime && box.revealAlpha > 0.0f) {
            box.revealAlpha -= delta * 1.5f;
            if (box.revealAlpha < 0.0f) box.revealAlpha = 0.0f;
        }
    }
}

// === Updated Snack Drawing with Animation ===
void DrawSnackBoxes(bool showContents) {
    for (auto &b : snackBoxes) {
        DrawRectangleRec(b.rect, showContents ? DARKGRAY : BROWN);
        if (showContents || b.revealAlpha > 0.0f) {
            Color fadeColor = Fade(YELLOW, showContents ? 1.0f : b.revealAlpha);
            DrawText(b.content.c_str(), b.rect.x + 5, b.rect.y + 20, 14, fadeColor);
        }
    }
}

void logic_draw_memory_challenge() {
    float delta = GetFrameTime();
    phaseTimer += delta;
    roundTimer += delta;

    BeginDrawing();
    ClearBackground(BLACK);

    Vector2 mouse = GetMousePosition();

    if (currentPhase == PHASE_START) {
        DrawText("🧠 Welcome to the DUCSU Memory Trials", 180, 150, 24, WHITE);
        DrawRectangleRec(startButton, DARKGRAY);
        DrawText("START", startButton.x + 40, startButton.y + 15, 20, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, startButton)) {
            currentPhase = PHASE_SHOES;
            // Reset phase timer and other round 1 state
        }
    }

    // Add your other round logic here ...

    else if (currentPhase == PHASE_SNACKS) {
        AnimateSnackBoxes(delta);
        DrawText("Round 2: Student Snack Sabotage", 200, 20, 20, ORANGE);
        DrawText(TextFormat("Time Left: %d", (int)(maxRoundTime - roundTimer)), 30, 20, 20, YELLOW);
        if (roundTimer > maxRoundTime) currentPhase = PHASE_FAIL;

        if (phaseTimer < showTime) {
            DrawSnackBoxes(true);
        } else {
            inputEnabled = true;
            DrawSnackBoxes(false);
            DrawText("Where was the shingara?", 250, 100, 20, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                for (int i = 0; i < snackBoxes.size(); i++) {
                    if (CheckCollisionPointRec(mouse, snackBoxes[i].rect)) {
                        if (i == correctSnackIndex) {
                            currentPhase = PHASE_CANDIDATE;
                            // InitCandidates();
                        } else currentPhase = PHASE_FAIL;
                    }
                }
            }
        }
    }

    else if (currentPhase == PHASE_CANDIDATE) {
        DrawText("Round 3: Rise of the DUCSU Champion", 180, 20, 20, ORANGE);
        DrawText(TextFormat("Time Left: %d", (int)(maxRoundTime - roundTimer)), 30, 20, 20, YELLOW);
        if (roundTimer > maxRoundTime) currentPhase = PHASE_FAIL;

        if (phaseTimer < showTime) {
            for (int i = 0; i < candidates.size(); i++) {
                DrawRectangleRec(candidates[i].rect, candidates[i].color);
                DrawText(candidates[i].name.c_str(), candidates[i].rect.x + 5, candidates[i].rect.y - 20, 14, WHITE);
                DrawText(candidates[i].symbol.c_str(), candidates[i].rect.x + 30, candidates[i].rect.y + 40, 30, WHITE);
            }
        } else {
            inputEnabled = true;
            DrawText("Who will lead the resistance?", 230, 100, 20, WHITE);
            for (int i = 0; i < candidates.size(); i++) {
                DrawRectangleRec(candidates[i].rect, candidates[i].color);
            }
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                for (int i = 0; i < candidates.size(); i++) {
                    if (CheckCollisionPointRec(mouse, candidates[i].rect)) {
                        if (i == correctCandidateIndex) {
                            currentPhase = PHASE_SUCCESS;
                        } else {
                            currentPhase = PHASE_FAIL;
                        }
                    }
                }
            }
        }
    } else if (currentPhase == PHASE_SUCCESS) {
        DrawText("🎉 You've cleared all stages of absurdity!", 120, 220, 24, GREEN);
        DrawText("Clue: SHORT CIRCUIT", 280, 270, 20, YELLOW);
        DrawRectangleRec(startButton, DARKGRAY);
        DrawText("RESTART", startButton.x + 30, startButton.y + 15, 20, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, startButton)) {
            currentPhase = PHASE_START;
        }
    } else if (currentPhase == PHASE_FAIL) {
        DrawText("❌ Memory failed in chaos. Try again.", 160, 250, 20, RED);
        DrawRectangleRec(startButton, DARKGRAY);
        DrawText("RESTART", startButton.x + 30, startButton.y + 15, 20, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, startButton)) {
            currentPhase = PHASE_START;
        }
    }

    EndDrawing();
}

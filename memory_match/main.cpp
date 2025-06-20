#include "raylib.h"
#include <vector>
#include <string>
#include <algorithm>

// === CONSTANTS ===
const int screenWidth = 800;
const int screenHeight = 600;
const float maxRoundTime = 15.0f;
const float showTime = 3.0f;

// === GLOBAL STATE ===
float roundTimer = 0.0f;
float phaseTimer = 0.0f;
bool inputEnabled = false;

Rectangle startButton = { 330, 500, 140, 50 };

enum GamePhase { PHASE_START, PHASE_SHOES, PHASE_SNACKS, PHASE_CANDIDATE, PHASE_SUCCESS, PHASE_FAIL };
GamePhase currentPhase = PHASE_START;

// === STRUCTS ===
struct Shoe {
    Rectangle rect;
    Color color;
};
std::vector<Shoe> shoes;
int missingShoeIndex = 0;

struct SnackBox {
    Rectangle rect;
    std::string content;
};
std::vector<SnackBox> snackBoxes;
int correctSnackIndex = 0;

float revealAlpha = 1.0f;
void AnimateSnackBoxes(float delta) {
    if (revealAlpha > 0.0f) revealAlpha -= delta * 0.3f;
}

void DrawSnackBoxes(bool showContent) {
    for (auto& b : snackBoxes) {
        DrawRectangleRec(b.rect, BROWN);
        if (showContent)
            DrawText(b.content.c_str(), b.rect.x + 10, b.rect.y + 20, 14, WHITE);
        else
            DrawRectangleLinesEx(b.rect, 2, WHITE);
    }
}

struct Candidate {
    Rectangle rect;
    std::string name;
    std::string symbol;
    Color color;
};
std::vector<Candidate> candidates;
int correctCandidateIndex = 0;

// === FORWARD DECLARATIONS ===
void ShuffleLayout();
void DrawRoundShoes(float delta, Vector2 mouse);
void DrawRoundSnack(float delta, Vector2 mouse);
void DrawRoundCandidate(float delta, Vector2 mouse);
void logic_draw_memory_challenge();

// === MAIN ===
int main() {
    InitWindow(screenWidth, screenHeight, "DUCSU Memory Match");
    SetTargetFPS(60);
    ShuffleLayout();

    while (!WindowShouldClose()) {
        logic_draw_memory_challenge();
    }

    CloseWindow();
    return 0;
}


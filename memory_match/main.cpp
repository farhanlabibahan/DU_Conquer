
#include "raylib.h"
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>

};
std::vector<SnackBox> snackBoxes;
int correctSnackIndex = -1;
bool round2Complete = false;
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

void InitShoes() {
    shoes.clear();
    for (int i = 0; i < 6; i++) {
        Shoe s;
        s.rect = { 100 + i * 100.0f, 200, 60, 60 };
        s.color = Color{ (unsigned char)GetRandomValue(50, 255), (unsigned char)GetRandomValue(50, 255), (unsigned char)GetRandomValue(50, 255), 255 };
        shoes.push_back(s);
    }
    missingShoeIndex = GetRandomValue(0, shoes.size() - 1);
    phaseTimer = 0;
    roundTimer = 0;
    inputEnabled = false;
}

void InitSnacks() {
    snackBoxes.clear();
    const char* snackOptions[] = { "shingara", "chalk", "CPU", "sock", "biryani", "pen" };
    for (int i = 0; i < 6; i++) {
        SnackBox b;
        b.rect = { 100 + i * 100.0f, 250, 60, 60 };
        b.content = snackOptions[GetRandomValue(0, 5)];
        snackBoxes.push_back(b);
    }
    correctSnackIndex = GetRandomValue(0, snackBoxes.size() - 1);
    snackBoxes[correctSnackIndex].content = "shingara";
    phaseTimer = 0;
    roundTimer = 0;
    inputEnabled = false;
}

void InitCandidates() {
    candidates.clear();
    const char* names[] = { "Mimi", "Toha", "Rashed", "Tumpa", "Kabbo" };
    const char* symbols[] = { "🧃", "🪙", "🧠", "🐸", "🔧" };
    for (int i = 0; i < 5; i++) {
        Candidate c;
        c.rect = { 100 + i * 120.0f, 220, 80, 120 };
        c.name = names[i];
        c.color = Color{ (unsigned char)GetRandomValue(50, 255), (unsigned char)GetRandomValue(50, 255), (unsigned char)GetRandomValue(50, 255), 255 };
        c.symbol = symbols[i];
        candidates.push_back(c);
    }
    correctCandidateIndex = GetRandomValue(0, candidates.size() - 1);
    phaseTimer = 0;
    roundTimer = 0;
    inputEnabled = false;
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
            InitShoes();
        }
    }

    else if (currentPhase == PHASE_SHOES) {
        DrawText("Round 1: Memory of the Marching Shoes", 180, 20, 20, ORANGE);
        DrawText(TextFormat("Time Left: %d", (int)(maxRoundTime - roundTimer)), 30, 20, 20, YELLOW);
        if (roundTimer > maxRoundTime) currentPhase = PHASE_FAIL;

        if (phaseTimer < showTime) {
            for (auto& s : shoes) DrawRectangleRec(s.rect, s.color);
        } else {
            inputEnabled = true;
            for (int i = 0; i < shoes.size(); i++) if (i != missingShoeIndex) DrawRectangleRec(shoes[i].rect, shoes[i].color);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                for (int i = 0; i < shoes.size(); i++) {
                    if (CheckCollisionPointRec(mouse, shoes[i].rect)) {
                        if (i == missingShoeIndex) {
                            currentPhase = PHASE_SNACKS;
                            InitSnacks();
                        } else currentPhase = PHASE_FAIL;
                    }
                }
            }
        }
    }

    else if (currentPhase == PHASE_SNACKS) {
        DrawText("Round 2: Student Snack Sabotage", 200, 20, 20, ORANGE);
        DrawText(TextFormat("Time Left: %d", (int)(maxRoundTime - roundTimer)), 30, 20, 20, YELLOW);
        if (roundTimer > maxRoundTime) currentPhase = PHASE_FAIL;

        if (phaseTimer < showTime) {
            for (auto& b : snackBoxes) {
                DrawRectangleRec(b.rect, DARKGRAY);
                DrawText(b.content.c_str(), b.rect.x + 5, b.rect.y + 20, 14, YELLOW);
            }
        } else {
            inputEnabled = true;
            for (auto& b : snackBoxes) DrawRectangleRec(b.rect, BROWN);
            DrawText("Where was the shingara?", 250, 100, 20, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                for (int i = 0; i < snackBoxes.size(); i++) {
                    if (CheckCollisionPointRec(mouse, snackBoxes[i].rect)) {
                        if (i == correctSnackIndex) {
                            currentPhase = PHASE_CANDIDATE;
                            InitCandidates();
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
            DrawText("Who will stand against chaos?", 220, 100, 20, WHITE);
            for (int i = 0; i < candidates.size(); i++) DrawRectangleRec(candidates[i].rect, candidates[i].color);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                for (int i = 0; i < candidates.size(); i++) {
                    if (CheckCollisionPointRec(mouse, candidates[i].rect)) {
                        currentPhase = (i == correctCandidateIndex) ? PHASE_SUCCESS : PHASE_FAIL;
                    }
                }
            }
        }
    }

    else if (currentPhase == PHASE_SUCCESS) {
        DrawText("🎉 You’ve cleared all stages of absurdity!", 160, 180, 24, GREEN);
        DrawText("Your champion rises! Clue: SHORT CIRCUIT", 180, 230, 20, YELLOW);
        DrawRectangleRec(startButton, DARKGRAY);
        DrawText("RESTART", startButton.x + 25, startButton.y + 15, 20, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, startButton)) {
            currentPhase = PHASE_START;
        }
    }
    else if (currentPhase == PHASE_FAIL) {
        DrawText("❌ Memory failed. The chaos wins this round.", 160, 220, 20, RED);
        DrawRectangleRec(startButton, DARKGRAY);
        DrawText("RESTART", startButton.x + 25, startButton.y + 15, 20, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, startButton)) {
            currentPhase = PHASE_START;
        }
    }

    EndDrawing();
}

int main() {
    InitWindow(screenWidth, screenHeight, "DUCSU Memory Trials");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        logic_draw_memory_challenge();
    }
    CloseWindow();
    return 0;
}

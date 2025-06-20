#include "raylib.h"
#include <vector>
#include <string>
#include <algorithm>
#include <random>

// === CONSTANTS ===
const int screenWidth = 800;
const int screenHeight = 600;
const float maxRoundTime = 15.0f;
const float showTime = 3.0f;

// === GLOBAL STATE ===
float roundTimer = 0.0f;
float phaseTimer = 0.0f;
bool inputEnabled = false;

Texture2D bgImage;
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

void DrawGlow(Rectangle rect, Color color) {
    for (int i = 10; i > 0; i--) {
        DrawRectangleLinesEx({ rect.x - i, rect.y - i, rect.width + i * 2, rect.height + i * 2 }, 1, Fade(color, 0.05f));
    }
}

void logic_draw_memory_challenge() {
    float delta = GetFrameTime();
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexture(bgImage, 0, 0, WHITE);

    Vector2 mouse = GetMousePosition();

    if (currentPhase == PHASE_START) {
        DrawText("DUCSU Memory Match!", 240, 80, 30, WHITE);
        DrawText("Remember and choose correctly in each round.", 180, 130, 20, LIGHTGRAY);
        DrawRectangleRec(startButton, LIGHTGRAY);
        DrawText("START", startButton.x + 30, startButton.y + 10, 20, BLACK);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, startButton)) {
            ShuffleLayout();
            currentPhase = PHASE_SHOES;
            roundTimer = 0.0f;
            inputEnabled = false;
        }
    } else if (currentPhase == PHASE_SHOES) {
        DrawText("Round 1: Which shoe is missing?", 220, 40, 20, YELLOW);
        roundTimer += delta;
        if (roundTimer >= showTime) inputEnabled = true;

        for (int i = 0; i < (int)shoes.size(); i++) {
            if (inputEnabled || i != missingShoeIndex) {
                DrawRectangleRec(shoes[i].rect, shoes[i].color);
                if (inputEnabled) DrawGlow(shoes[i].rect, shoes[i].color);
            }
        }

        if (inputEnabled && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (int i = 0; i < (int)shoes.size(); i++) {
                if (CheckCollisionPointRec(mouse, shoes[i].rect)) {
                    if (i == missingShoeIndex) {
                        ShuffleLayout();
                        currentPhase = PHASE_SNACKS;
                        roundTimer = 0.0f;
                        phaseTimer = 0.0f;
                        inputEnabled = false;
                        revealAlpha = 1.0f;
                    } else {
                        currentPhase = PHASE_FAIL;
                    }
                }
            }
        }
    } else if (currentPhase == PHASE_SNACKS) {
        DrawText("Round 2: Which box had the shingara?", 200, 40, 20, ORANGE);
        phaseTimer += delta;
        AnimateSnackBoxes(delta);

        if (phaseTimer < showTime) {
            DrawSnackBoxes(true);
        } else {
            inputEnabled = true;
            DrawSnackBoxes(false);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                for (int i = 0; i < (int)snackBoxes.size(); i++) {
                    if (CheckCollisionPointRec(mouse, snackBoxes[i].rect)) {
                        if (i == correctSnackIndex) {
                            ShuffleLayout();
                            currentPhase = PHASE_CANDIDATE;
                            roundTimer = 0.0f;
                            inputEnabled = false;
                        } else {
                            currentPhase = PHASE_FAIL;
                        }
                    }
                }
            }
        }
    } else if (currentPhase == PHASE_CANDIDATE) {
        DrawText("Round 3: Who is the right candidate?", 220, 40, 20, SKYBLUE);
        for (int i = 0; i < (int)candidates.size(); i++) {
            DrawRectangleRec(candidates[i].rect, candidates[i].color);
            DrawGlow(candidates[i].rect, candidates[i].color);
            DrawText(candidates[i].name.c_str(), candidates[i].rect.x + 10, candidates[i].rect.y + 10, 18, BLACK);
            DrawText(candidates[i].symbol.c_str(), candidates[i].rect.x + 40, candidates[i].rect.y + 40, 30, WHITE);
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (int i = 0; i < (int)candidates.size(); i++) {
                if (CheckCollisionPointRec(mouse, candidates[i].rect)) {
                    if (i == correctCandidateIndex) {
                        currentPhase = PHASE_SUCCESS;
                    } else {
                        currentPhase = PHASE_FAIL;
                    }
                }
            }
        }
    } else if (currentPhase == PHASE_SUCCESS) {
        DrawText("You WIN! Clue Unlocked for Next Department!", 120, 260, 24, GREEN);
        DrawRectangleRec(startButton, LIGHTGRAY);
        DrawText("RESTART", startButton.x + 20, startButton.y + 10, 20, BLACK);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, startButton)) {
            ShuffleLayout();
            currentPhase = PHASE_START;
            roundTimer = 0.0f;
            phaseTimer = 0.0f;
            inputEnabled = false;
        }
    } else if (currentPhase == PHASE_FAIL) {
        DrawText("Oops! Wrong choice. Try Again!", 200, 260, 24, RED);
        DrawRectangleRec(startButton, LIGHTGRAY);
        DrawText("RESTART", startButton.x + 20, startButton.y + 10, 20, BLACK);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, startButton)) {
            ShuffleLayout();
            currentPhase = PHASE_START;
            roundTimer = 0.0f;
            phaseTimer = 0.0f;
            inputEnabled = false;
        }
    }

    EndDrawing();
}

void ShuffleLayout() {
    std::random_device rd;
    std::mt19937 g(rd());

    shoes.clear();
    for (int i = 0; i < 6; i++) {
        Shoe s;
        s.rect = { 100 + (i % 3) * 200.0f + (float)GetRandomValue(-20, 20), 200 + (i / 3) * 120.0f + (float)GetRandomValue(-10, 10), 60, 60 };
        s.color = Color{ (unsigned char)GetRandomValue(100, 255), (unsigned char)GetRandomValue(100, 255), (unsigned char)GetRandomValue(100, 255), 255 };
        shoes.push_back(s);
    }
    missingShoeIndex = GetRandomValue(0, (int)shoes.size() - 1);

    snackBoxes.clear();
    const char* snacks[] = { "shingara", "samosa", "cake", "banana", "chips", "roll" };
    std::vector<Vector2> positions;
    for (int i = 0; i < 6; i++) positions.push_back({ 100 + (i % 3) * 200.0f + (float)GetRandomValue(-20, 20), 240 + (i / 3) * 110.0f + (float)GetRandomValue(-10, 10) });
    std::shuffle(positions.begin(), positions.end(), g);
    for (int i = 0; i < 6; i++) {
        SnackBox box;
        box.rect = { positions[i].x, positions[i].y, 80, 60 };
        box.content = snacks[i % 6];
        snackBoxes.push_back(box);
    }
    correctSnackIndex = GetRandomValue(0, (int)snackBoxes.size() - 1);
    snackBoxes[correctSnackIndex].content = "shingara";

    candidates.clear();
    const char* names[] = { "Mehedi", "Tumpa", "Shuvo", "Jamal" };
    const char* symbols[] = { "📢", "📚", "🐸", "🚩" };
    for (int i = 0; i < 4; i++) {
        Candidate c;
        c.name = names[i];
        c.symbol = symbols[i];
        c.color = Color{ (unsigned char)GetRandomValue(80, 220), (unsigned char)GetRandomValue(80, 220), (unsigned char)GetRandomValue(80, 220), 255 };
        c.rect = { 120 + i * 160.0f + (float)GetRandomValue(-15, 15), 300 + (float)GetRandomValue(-10, 10), 100, 100 };
        candidates.push_back(c);
    }
    correctCandidateIndex = GetRandomValue(0, 3);
}

int main() {
    InitWindow(screenWidth, screenHeight, "DUCSU Memory Match");
    InitAudioDevice();
    bgImage = LoadTexture("/mnt/c/Users/User/Pictures/Camera Roll/depts image/DUCSU.png");
    SetTargetFPS(60);
    ShuffleLayout();

    while (!WindowShouldClose()) {
        logic_draw_memory_challenge();
    }

    UnloadTexture(bgImage);
    CloseWindow();
    return 0;
}

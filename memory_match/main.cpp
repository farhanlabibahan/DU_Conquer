#include "raylib.h"
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <cctype>

using namespace std;

const int screenWidth = 800;
const int screenHeight = 600;
const int cardRows = 3;
const int cardCols = 4;

// === Structs ===
struct MemoryCard {
    Rectangle rect;
    string word;
    bool revealed;
    bool found;
    float revealTimer = 0.0f;
};

vector<MemoryCard> cards;
vector<pair<string, string>> synonymPairs = {
    {"Justice", "Fairness"},
    {"Rights", "Freedoms"},
    {"Unity", "Solidarity"},
    {"Peace", "Nonviolence"},
    {"Voice", "Protest"},
    {"Truth", "Transparency"}
};

vector<int> selectedIndices;
bool allCardsInitiallyRevealed = true;
float cardFlipTimer = 0.0f;
float revealTime = 3.0f;
int totalFlips = 0;
const int maxFlips = 30;
bool waitingToFlipBack = false;
float mismatchTimer = 0.0f;
int lastShuffleFlipCheckpoint = 0;
bool showClues = false;
bool showNextButton = false;
Rectangle nextButton = { 330, 500, 140, 50 };

Texture2D bgImage;
Rectangle startButton = { 330, 500, 140, 50 };

// === Snack Round State ===
struct SnackCard {
    Rectangle rect;
    string label;
    Texture2D icon;
    bool revealed;
    bool found;
};

vector<SnackCard> snackCards;
int snackFlipCount = 0;
int snackMatched = 0;
int maxSnackFlips = 10;
vector<int> snackSelection;

void InitSnackRound(); // Declaration moved up

// === Final Round ===
bool inputMode = false;
string playerInput;
bool clueEntered = false;
bool finalAnswerCorrect = false;
float finalRoundTimer = 30.0f;
bool timerStarted = false;

enum GamePhase {
    PHASE_START,
    PHASE_CARDS,
    PHASE_CLUES,
    PHASE_SNACKS,
    PHASE_CANDIDATE,
    PHASE_SUCCESS,
    PHASE_FAIL,
    PHASE_GAMEOVER
};

GamePhase currentPhase = PHASE_START;

// === Helper Functions ===
string ToLower(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(), [](unsigned char c){ return tolower(c); });
    return result;
}

void InitSnackRound() {
    snackCards.clear();
    const char* snackNames[] = {
        "Juice", "Burger", "Cat", "Girl", "Mirror", "DUCSU",
        "Boy", "Buns", "Croissant", "DUSCU", "Water", "Cake"
    };
    for (int i = 0; i < 12; i++) {
        SnackCard card;
        card.label = snackNames[i];
        card.rect = { 80 + (i % 4) * 160.0f, 120 + (i / 4) * 130.0f, 100, 100 };
        card.revealed = false;
        card.found = false;

        string path = "images/" + ToLower(card.label) + ".png";
        card.icon = LoadTexture(path.c_str());

        snackCards.push_back(card);
    }
    snackFlipCount = 0;
    snackMatched = 0;
    snackSelection.clear();
}

void DrawClueScreen() {
    DrawRectangle(100, 100, 600, 300, Fade(BLACK, 0.7f));
    DrawText("\"A girl and her cat, in a mirror’s embrace,", 120, 120, 20, WHITE);
    DrawText("Watched juice and burger shatter DUCSU's place.\"", 120, 150, 20, WHITE);
    DrawText("\"The one in the glass was always the key,", 120, 200, 20, YELLOW);
    DrawText("DUCSU awaits, unknowingly thee.\"", 120, 230, 20, YELLOW);
    DrawRectangleRec(nextButton, DARKBLUE);
    DrawText("Next", nextButton.x + 30, nextButton.y + 15, 24, WHITE);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), nextButton)) {
        InitSnackRound();
        currentPhase = PHASE_SNACKS;
    }
}

void logic_draw_memory_challenge();

int main() {
    InitWindow(screenWidth, screenHeight, "DUCSU Memory Match");
    bgImage = LoadTexture("images/DUCSU.png");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        logic_draw_memory_challenge();
    }
    UnloadTexture(bgImage);
    CloseWindow();
    return 0;
}

void logic_draw_memory_challenge() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexturePro(
        bgImage,
        { 0, 0, (float)bgImage.width, (float)bgImage.height },
        { 0, 0, (float)screenWidth, (float)screenHeight },
        { 0, 0 },
        0.0f,
        WHITE
    );

    Vector2 mouse = GetMousePosition();
    float delta = GetFrameTime();

    switch (currentPhase) {
        case PHASE_START:
            DrawText("DUCSU Memory Match", 250, 200, 30, MAROON);
            DrawRectangleRec(startButton, DARKGRAY);
            DrawText("Start", startButton.x + 30, startButton.y + 15, 24, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, startButton)) {
                currentPhase = PHASE_CARDS;
                // Assume ShuffleCards() and game logic is elsewhere
            }
            break;
        case PHASE_CARDS:
            // Placeholder for actual game logic
            DrawText("Memory Card Round", 260, 280, 24, DARKGRAY);
            break;
        case PHASE_CLUES:
            DrawClueScreen();
            break;
        case PHASE_SNACKS:
            DrawText("Snack Matching Round", 260, 280, 24, DARKGRAY);
            break;
        case PHASE_CANDIDATE:
            DrawText("Final Round: Who Will Save You?", 220, 280, 24, DARKGRAY);
            break;
        case PHASE_SUCCESS:
            DrawText("You did it! DUCSU awaits.", 240, 280, 24, DARKGREEN);
            break;
        case PHASE_FAIL:
            DrawText("Too many mistakes! Try again.", 260, 280, 24, RED);
            break;
        case PHASE_GAMEOVER:
            DrawText("Game Over! You used all 30 flips.", 200, 280, 24, RED);
            DrawRectangleRec(startButton, DARKGRAY);
            DrawText("Restart", startButton.x + 25, startButton.y + 15, 24, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, startButton)) {
                currentPhase = PHASE_CARDS;
                totalFlips = 0;
            }
            break;
    }
    EndDrawing();
}

#include "raylib.h"
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>

struct Card {
    Rectangle rect;
    std::string text;
    std::string pairId;
    bool revealed = false;
    bool matched = false;
    bool isTrap = false;
};

const int screenWidth = 800;
const int screenHeight = 600;
const int CARD_WIDTH = 140;
const int CARD_HEIGHT = 180;

std::vector<Card> cards;
Card* selectedCard = nullptr;
float revealTimer = 0;
int matchCount = 0;
int mistakeCount = 0;
bool shuffleTriggered = false;
bool gameWon = false;
bool gameFailed = false;
bool allowInput = true;

int phase = 0;
float phaseDelay = 1.5f;
float phaseTimer = 0.0f;
float gameTime = 90.0f;
float currentTime = 0.0f;

Texture2D bg_ducsu;

// Function declarations
void InitDucsuMemoryMatch();
void LoadPhaseCards(int count);
void ShuffleCards();
void UpdateDucsuMemoryMatch(float delta);
void DrawCard(const Card& card);
void DrawDucsuMemoryMatch();
void CheckDucsuMemoryClick(Vector2 mouse);
void logic_draw_ducsu_memory_match();

void InitDucsuMemoryMatch() {
    cards.clear();
    matchCount = 0;
    mistakeCount = 0;
    phase = 0;
    phaseTimer = 0.0f;
    currentTime = 0.0f;
    shuffleTriggered = false;
    gameWon = false;
    gameFailed = false;
    allowInput = true;
    selectedCard = nullptr;
    bg_ducsu = LoadTexture("/mnt/c/Users/User/Pictures/Camera Roll/depts image/DUCSU.png");
    LoadPhaseCards(4);
}

void LoadPhaseCards(int count) {
    int startX = 100 + phase * 60;
    int y = 100;
    for (int i = 0; i < count; i += 2) {
        std::string pairId = "P" + std::to_string(GetRandomValue(100, 999));
        std::string voterText = "Voter: " + pairId;
        std::string idealText = "Ideal: " + pairId;

        Card a = {{(float)(startX + (i * 160) % screenWidth), (float)y, CARD_WIDTH, CARD_HEIGHT}, voterText, pairId};
        Card b = {{(float)(startX + ((i + 1) * 160) % screenWidth), (float)(y + 220), CARD_WIDTH, CARD_HEIGHT}, idealText, pairId};
        cards.push_back(a);
        cards.push_back(b);
    }

    if (phase == 2) {
        Card trap = {{(float)GetRandomValue(100, 700), (float)GetRandomValue(100, 400), CARD_WIDTH, CARD_HEIGHT}, "Corrupt Vote", "TRAP"};
        trap.isTrap = true;
        cards.push_back(trap);
    }

    ShuffleCards();
}

void ShuffleCards() {
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(GetRandomValue(0, 99999)));
}

void UpdateDucsuMemoryMatch(float delta) {
    if (gameWon || gameFailed) return;

    currentTime += delta;
    if (currentTime >= gameTime) {
        gameFailed = true;
        return;
    }

    if (phase < 3) {
        phaseTimer += delta;
        if (phaseTimer >= phaseDelay) {
            phase++;
            if (phase == 1) LoadPhaseCards(6);
            else if (phase == 2) LoadPhaseCards(8);
            phaseTimer = 0.0f;
        }
    }

    if (!shuffleTriggered && currentTime >= 30.0f) {
        ShuffleCards();
        shuffleTriggered = true;
    }

    if (matchCount == (cards.size() - 1) / 2) {
        gameWon = true;
        allowInput = false;
    }
}

void DrawCard(const Card& card) {
    if (card.matched) DrawRectangleRec(card.rect, DARKGREEN);
    else if (card.revealed) DrawRectangleRec(card.rect, GRAY);
    else DrawRectangleRec(card.rect, BLACK);
    DrawRectangleLinesEx(card.rect, 2, WHITE);
    if (card.revealed || card.matched) {
        DrawText(card.text.c_str(), card.rect.x + 10, card.rect.y + 10, 16, WHITE);
    }
}

void DrawDucsuMemoryMatch() {
    for (const auto& card : cards) DrawCard(card);
    DrawText(TextFormat("Time Left: %d", (int)(gameTime - currentTime)), 30, 20, 24, (gameTime - currentTime <= 20) ? RED : WHITE);
    DrawText(TextFormat("Mistakes: %d / 3", mistakeCount), 30, 50, 24, RED);
    if (gameWon) DrawText("All votes recalled. Clue Unlocked!", 200, 30, 24, GOLD);
    if (gameFailed || mistakeCount >= 3) DrawText("Corruption prevailed. Try again.", 200, 30, 24, RED);
    if (shuffleTriggered) DrawText("Memory Scrambled!", 300, 560, 20, GRAY);
}

void CheckDucsuMemoryClick(Vector2 mouse) {
    if (!allowInput) return;

    for (auto& card : cards) {
        if (CheckCollisionPointRec(mouse, card.rect) && !card.revealed && !card.matched) {
            card.revealed = true;
            if (!selectedCard) {
                selectedCard = &card;
            } else {
                if (card.isTrap || selectedCard->isTrap) {
                    mistakeCount++;
                    selectedCard->revealed = false;
                    card.revealed = false;
                    selectedCard = nullptr;
                    if (mistakeCount >= 3) gameFailed = true;
                    return;
                }
                if (card.pairId == selectedCard->pairId) {
                    card.matched = true;
                    selectedCard->matched = true;
                    matchCount++;
                } else {
                    mistakeCount++;
                    selectedCard->revealed = false;
                    card.revealed = false;
                }
                selectedCard = nullptr;
                if (mistakeCount >= 3) gameFailed = true;
            }
            break;
        }
    }
}

// Call this from main game loop when in Game_ducsu state
void logic_draw_ducsu_memory_match() {
    float delta = GetFrameTime();
    UpdateDucsuMemoryMatch(delta);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        CheckDucsuMemoryClick(GetMousePosition());
    }

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(
        bg_ducsu,
        Rectangle{ 0, 0, (float)bg_ducsu.width, (float)bg_ducsu.height },
        Rectangle{ 0, 0, (float)screenWidth, (float)screenHeight },
        Vector2{ 0, 0 },
        0.0f,
        WHITE
    );
    DrawDucsuMemoryMatch();
    EndDrawing();
}

// Minimal main() function for standalone compilation and testing
int main() {
    InitWindow(screenWidth, screenHeight, "DUCSU Memory Match");
    SetTargetFPS(60);
    InitDucsuMemoryMatch();

    while (!WindowShouldClose()) {
        logic_draw_ducsu_memory_match();
    }

    UnloadTexture(bg_ducsu);
    CloseWindow();
    return 0;
}

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(
        bg_ducsu,
        Rectangle{ 0, 0, (float)bg_ducsu.width, (float)bg_ducsu.height },
        Rectangle{ 0, 0, (float)screenWidth, (float)screenHeight },
        Vector2{ 0, 0 },
        0.0f,
        WHITE
    );
    DrawDucsuMemoryMatch();
    EndDrawing();
}

        

#include "raylib.h"
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include "global.h"
#include "memory_match.h"
#include <random>

const int sW = 1280;
const int sH = 720;
const int CARD_WIDTH = 140;
const int CARD_HEIGHT = 60;

struct Card {
    Rectangle rect;
    std::string text;
    std::string pairId;
    bool revealed = false;
    bool matched = false;
    bool isTrap = false;
};

// --- Globals ---
static std::vector<Card> cards;
static Card* selectedCard = nullptr;
static int matchCount = 0;
static int mistakeCount = 0;
static bool shuffleTriggered = false;

static bool gameFailed = false;
static bool allowInput = true;
static int phase = 0;
static float phaseDelay = 1.5f;
static float phaseTimer = 0.0f;
static float gameTime = 90.0f;
static float currentTime = 0.0f;
static Texture2D bg_ducsu;

// --- Declarations ---
void init_memory_match();
void logic_memory_match();
void draw_memory_match();
void unload_memory_match();

// --- Internal helpers ---
static void LoadPhaseCards(int count);
static void ShuffleCards();
static void DrawCard(const Card& card);
static void CheckClick(Vector2 mouse);
static void UpdateGame(float delta);
static void DrawCardsUI();

// --- Initialize ---
void init_memory_match() {
    cards.clear();
    matchCount = 0;
    mistakeCount = 0;
    phase = 0;
    phaseTimer = 0.0f;
    currentTime = 0.0f;
    shuffleTriggered = false;
    ducsu_game = false;
    gameFailed = false;
    allowInput = true;
    selectedCard = nullptr;
    LoadPhaseCards(4);
}

static void LoadPhaseCards(int count) {
    int cols = (count == 4) ? 2 : (count == 6) ? 3 : 4;
    int rows = (count + 1) / cols;
    float gridW = cols * CARD_WIDTH + (cols - 1) * 30;
    float gridH = rows * CARD_HEIGHT + (rows - 1) * 30;
    float startX = (sW / 2.0f) - (gridW / 2.0f);
    float startY = (sH / 2.0f) - (gridH / 2.0f);
    int cardIdx = 0;

    for (int i = 0; i < count; i += 2) {
        std::string pairId = "P" + std::to_string(GetRandomValue(100, 999));
        std::string voterText = "Voter: " + pairId;
        std::string idealText = "Ideal: " + pairId;
        int colA = cardIdx % cols;
        int rowA = cardIdx / cols;
        int colB = (cardIdx + 1) % cols;
        int rowB = (cardIdx + 1) / cols;

        Card a = { { startX + colA * (CARD_WIDTH + 30), startY + rowA * (CARD_HEIGHT + 30), CARD_WIDTH, CARD_HEIGHT }, voterText, pairId };
        Card b = { { startX + colB * (CARD_WIDTH + 30), startY + rowB * (CARD_HEIGHT + 30), CARD_WIDTH, CARD_HEIGHT }, idealText, pairId };
        cards.push_back(a);
        cards.push_back(b);
        cardIdx += 2;
    }

    if (phase == 2) {
        Card trap = { { startX + GetRandomValue(0, cols - 1) * (CARD_WIDTH + 30), startY + GetRandomValue(0, rows - 1) * (CARD_HEIGHT + 30), CARD_WIDTH, CARD_HEIGHT }, "Corrupt Vote", "TRAP" };
        trap.isTrap = true;
        cards.push_back(trap);
    }

    ShuffleCards();
}

static void ShuffleCards() {
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(std::random_device{}()));
    int count = (int)cards.size();
    int cols = (count <= 4) ? 2 : (count <= 6) ? 3 : 4;
    int rows = (count + cols - 1) / cols;
    float gridW = cols * CARD_WIDTH + (cols - 1) * 30;
    float gridH = rows * CARD_HEIGHT + (rows - 1) * 30;
    float startX = (sW / 2.0f) - (gridW / 2.0f);
    float startY = (sH / 2.0f) - (gridH / 2.0f);

    for (int i = 0; i < count; ++i) {
        int col = i % cols;
        int row = i / cols;
        cards[i].rect.x = startX + col * (CARD_WIDTH + 30);
        cards[i].rect.y = startY + row * (CARD_HEIGHT + 30);
    }
}

void logic_memory_match() {
    float delta = GetFrameTime();
    UpdateGame(delta);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        CheckClick(GetMousePosition());
    }
}

static void UpdateGame(float delta) {
    if (ducsu_game || gameFailed) return;
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

    int realPairs = 0;
    for (const auto& c : cards) if (!c.isTrap) realPairs++;
    if (matchCount == realPairs / 2) {
        ducsu_game = true;
        allowInput = false;
    }
}

static void CheckClick(Vector2 mouse) {
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
                    if (mistakeCount >= 40) gameFailed = true;
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
                if (mistakeCount >= 40) gameFailed = true;
            }
            break;
        }
    }
}

void draw_memory_match() {


    Rectangle popup = { sW / 2.0f - 400, sH / 2.0f - 300, 800, 600 };
    DrawRectangleRec(popup, Fade(DARKGRAY, 0.95f));
    DrawRectangleLinesEx(popup, 4, GOLD);
    BeginScissorMode((int)popup.x, (int)popup.y, (int)popup.width, (int)popup.height);

    DrawCardsUI();

    EndScissorMode();
}

static void DrawCard(const Card& card) {
    if (card.matched) DrawRectangleRec(card.rect, DARKGREEN);
    else if (card.revealed) DrawRectangleRec(card.rect, GRAY);
    else DrawRectangleRec(card.rect, BLACK);
    DrawRectangleLinesEx(card.rect, 2, WHITE);
    if (card.revealed || card.matched) {
        DrawText(card.text.c_str(), (int)card.rect.x + 10, (int)card.rect.y + 10, 16, WHITE);
    }
}

static void DrawCardsUI() {
    for (const auto& card : cards) DrawCard(card);
    DrawText(TextFormat("Time Left: %d", (int)(gameTime - currentTime)), sW / 2 - 360, sH / 2 - 280, 24, (gameTime - currentTime <= 20) ? RED : WHITE);
    DrawText(TextFormat("Mistakes: %d / 40", mistakeCount), sW / 2 - 360, sH / 2 - 240, 24, RED);

    if (ducsu_game) DrawText("All votes recalled. Clue Unlocked!", sW / 2 - 200, sH / 2 - 280, 24, GOLD);
    if (gameFailed || mistakeCount >= 40) DrawText("Corruption prevailed. Try again.", sW / 2 - 200, sH / 2 - 280, 24, RED);
    if (shuffleTriggered) DrawText("Memory Scrambled!", sW / 2 - 100, sH / 2 + 260, 20, GRAY);
}

void unload_memory_match() {

}


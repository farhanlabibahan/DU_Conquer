<<<<<<< HEAD
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

        Card a = { {(float)(startX + (i * 160) % screenWidth), (float)y, CARD_WIDTH, CARD_HEIGHT}, voterText, pairId };
        Card b = { {(float)(startX + ((i + 1) * 160) % screenWidth), (float)(y + 220), CARD_WIDTH, CARD_HEIGHT}, idealText, pairId };

        cards.push_back(a);
        cards.push_back(b);
    }

    if (phase == 2) {
        Card trap = { {(float)GetRandomValue(100, 700), (float)GetRandomValue(100, 400), CARD_WIDTH, CARD_HEIGHT}, "Corrupt Vote", "TRAP" };
        trap.isTrap = true;
        cards.push_back(trap);
    }

    ShuffleCards();
}

void ShuffleCards() {
    std::shuffle(cards.begin(), cards.end(), std::default_random_engine(std::random_device{}()));
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

=======
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <cstdio>

#define MAX_PAIRS 8
#define MAX_CARDS (MAX_PAIRS * 2)
#define CARD_WIDTH 180
#define CARD_HEIGHT 120
#define PADDING 30
#define ROWS 4
#define COLUMNS 4
#define FLIP_TIME 0.3f

typedef struct {
    Rectangle rect;
    char label[32];
    bool revealed;
    bool matched;
    float flipTimer;
    bool flipping;
} Card;

const char *pairs[MAX_PAIRS][2] = {
    {"Raima", "irony"}, {"Rapunzel", "disney"},
    {"csedu", "death"}, {"Rana sir", "GOAT"},
    {"DU", "dream"}, {"Raisa", "cat"},
    {"Salwa", "high"}, {"Ahan", "gay"}
};

Card cards[MAX_CARDS];
int selectedIndices[2] = {-1, -1};
int matchedCount = 0;
float gameTime = 60.0f; // 60 seconds timer
bool gameOver = false;
Rectangle restartButton = {350, 560, 150, 30};

Color cardColor = (Color){200, 230, 255, 255};

void Shuffle(char labels[MAX_CARDS][32]) {
    for (int i = MAX_CARDS - 1; i > 0; i--) {
        int j = GetRandomValue(0, i);
        char temp[32];
        strcpy(temp, labels[i]);
        strcpy(labels[i], labels[j]);
        strcpy(labels[j], temp);
    }
}

void InitCards() {
    char labels[MAX_CARDS][32];
    for (int i = 0; i < MAX_PAIRS; i++) {
        strcpy(labels[2 * i], pairs[i][0]);
        strcpy(labels[2 * i + 1], pairs[i][1]);
    }
    Shuffle(labels);

    for (int i = 0; i < MAX_CARDS; i++) {
        int row = i / COLUMNS;
        int col = i % COLUMNS;
        cards[i].rect = (Rectangle){
            (float)(PADDING + col * (CARD_WIDTH + PADDING)),
            (float)(PADDING + row * (CARD_HEIGHT + PADDING)),
            CARD_WIDTH, CARD_HEIGHT
        };
        strcpy(cards[i].label, labels[i]);
        cards[i].revealed = false;
        cards[i].matched = false;
        cards[i].flipTimer = 0;
        cards[i].flipping = false;
    }
    selectedIndices[0] = -1;
    selectedIndices[1] = -1;
    matchedCount = 0;
    gameTime = 60.0f;
    gameOver = false;
}

bool IsMatchingPair(const char *a, const char *b) {
    for (int i = 0; i < MAX_PAIRS; i++) {
        if ((strcmp(pairs[i][0], a) == 0 && strcmp(pairs[i][1], b) == 0) ||
            (strcmp(pairs[i][1], a) == 0 && strcmp(pairs[i][0], b) == 0)) {
            return true;
        }
    }
    return false;
}

void UpdateGame(float delta) {
    if (gameOver) return;

    gameTime -= delta;
    if (gameTime <= 0) {
        gameOver = true;
        return;
    }

    for (int i = 0; i < MAX_CARDS; i++) {
        if (cards[i].flipping) {
            cards[i].flipTimer += delta;
            if (cards[i].flipTimer >= FLIP_TIME) {
                cards[i].flipTimer = 0;
                cards[i].flipping = false;
                cards[i].revealed = !cards[i].revealed;
            }
        }
    }

    if (selectedIndices[0] != -1 && selectedIndices[1] != -1) {
        int i1 = selectedIndices[0];
        int i2 = selectedIndices[1];

        if (!cards[i1].flipping && !cards[i2].flipping) {
            if (IsMatchingPair(cards[i1].label, cards[i2].label)) {
                cards[i1].matched = true;
                cards[i2].matched = true;
                matchedCount++;
            } else {
                cards[i1].flipping = true;
                cards[i2].flipping = true;
            }
            selectedIndices[0] = -1;
            selectedIndices[1] = -1;
        }
    }

    if (matchedCount == MAX_PAIRS) {
        gameOver = true;
    }
}

void DrawCard(Card *card, Font font) {
    Color borderColor = BLACK;
    DrawRectangleRec(card->rect, card->revealed || card->matched ? cardColor : DARKGRAY);
    DrawRectangleLinesEx(card->rect, 2, borderColor);
    if (card->revealed || card->matched) {
        Vector2 textSize = MeasureTextEx(font, card->label, 24, 2);
        DrawTextEx(font, card->label,
            (Vector2){card->rect.x + (CARD_WIDTH - textSize.x)/2, card->rect.y + (CARD_HEIGHT - textSize.y)/2},
            24, 2, BLACK);
    }
}

int main(void) {
    InitWindow(900, 700, "Memory Match Game - Spirit Animals");
    InitAudioDevice();
    SetTargetFPS(60);
    srand(time(NULL));

    Font font = LoadFont("resources/Roboto-Bold.ttf");
    if (!font.texture.id) font = GetFontDefault();

    InitCards();

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();
        UpdateGame(delta);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mouse = GetMousePosition();

            if (gameOver && CheckCollisionPointRec(mouse, restartButton)) {
                InitCards();
            } else if (!gameOver) {
                for (int i = 0; i < MAX_CARDS; i++) {
                    if (!cards[i].revealed && !cards[i].matched && CheckCollisionPointRec(mouse, cards[i].rect)) {
                        if (selectedIndices[0] == -1) {
                            selectedIndices[0] = i;
                            cards[i].flipping = true;
                        } else if (selectedIndices[1] == -1 && i != selectedIndices[0]) {
                            selectedIndices[1] = i;
                            cards[i].flipping = true;
                        }
                        break;
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int i = 0; i < MAX_CARDS; i++) DrawCard(&cards[i], font);

        char timerText[64];
        snprintf(timerText, sizeof(timerText), "Time Left: %.1f", gameTime);
        DrawTextEx(font, timerText, (Vector2){680, 20}, 22, 2, RED);

        if (gameOver) {
            const char* resultText = (matchedCount == MAX_PAIRS) ? "YOU WIN LESGOO!" : "WHARRA MEMORY HAHA";
            Vector2 size = MeasureTextEx(font, resultText, 38, 2);
            DrawTextEx(font, resultText, (Vector2){(900 - size.x)/2, 600}, 38, 2, BLUE);

            DrawRectangleRec(restartButton, LIGHTGRAY);
            DrawTextEx(font, "RESTART", (Vector2){restartButton.x + 20, restartButton.y + 5}, 22, 2, BLACK);
        }

        EndDrawing();
    }

    UnloadFont(font);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}


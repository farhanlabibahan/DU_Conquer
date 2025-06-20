#include "raylib.h"
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <cctype> // For tolower usage

using namespace std;

const int screenWidth = 800;
const int screenHeight = 600;
const int cardRows = 3;
const int cardCols = 4;

struct MemoryCard {
    Rectangle rect;
    string word;
    bool revealed;
    bool found;
};

struct SnackCard {
    Rectangle rect;
    string snack;
    bool revealed;
    bool collected;
    Texture2D image;
};

vector<MemoryCard> cards;
vector<SnackCard> snacks;
vector<string> memoryWords = {
    "Students", "Unite", "Against", "Corruption",
    "Demand", "Fair", "Vote", "Rights",
    "Protest", "Peace", "Justice", "Now"
};

vector<string> correctSnackTargets = {
    "Juice", "Burger", "Cat", "Girl", "Mirror", "DUCSU"
};

vector<string> distractorSnacks = {
    "Boy", "Bun", "Croissant", "DUSCU", "Water", "Cake"
};

vector<string> selectedClueWords;
string cluePhrase = "";
int snackAttempts = 0;
bool allSnacksCollected = false;
bool correctAnswerEntered = false;

string poeticClue = "A girl and her cat, through DUCSU's glass dream,\nSaw juice and burger disrupt the mirror’s gleam.";
string finalClueLine = "The one in the glass was always the key, DUCSU awaits, unknowingly thee.";
string finalQuestion = "Who will save you now?";
string correctFinalAnswer = "myself";
string typewriterText = "";
int typewriterIndex = 0;
float typewriterTimer = 0.0f;
float typewriterSpeed = 0.05f;

string shuffledHint = "cat burger mirror DUCSU juice girl";
string playerInput = "";
bool answerPrompt = false;
bool inputActive = false;
bool gameWon = false;

enum GamePhase {
    PHASE_START,
    PHASE_CARDS,
    PHASE_SNACKS,
    PHASE_CANDIDATE,
    PHASE_INPUT_FINAL,
    PHASE_SUCCESS,
    PHASE_FAIL
};

GamePhase currentPhase = PHASE_START;
float revealTime = 2.0f;
float phaseTimer = 0.0f;
Texture2D bgImage;
Texture2D snackIcons[12];
Rectangle startButton = { 330, 500, 140, 50 };
Rectangle nextButton = { 500, 500, 140, 50 };
Rectangle restartButton = { 160, 500, 140, 50 };

void ShuffleCards() {
    random_device rd;
    mt19937 g(rd());
    shuffle(memoryWords.begin(), memoryWords.end(), g);

    cards.clear();
    int index = 0;
    for (int y = 0; y < cardRows; y++) {
        for (int x = 0; x < cardCols; x++) {
            MemoryCard card;
            card.rect = { 80 + x * 150.0f, 120 + y * 120.0f, 120, 60 };
            card.word = memoryWords[index++];
            card.revealed = false;
            card.found = false;
            cards.push_back(card);
        }
    }
}

void ShuffleSnacks() {
    vector<string> allSnacks = correctSnackTargets;
    allSnacks.insert(allSnacks.end(), distractorSnacks.begin(), distractorSnacks.end());

    random_device rd;
    mt19937 g(rd());
    shuffle(allSnacks.begin(), allSnacks.end(), g);

    vector<Vector2> positions;
    for (int i = 0; i < 12; i++) {
        positions.push_back({ 80 + (i % 4) * 160.0f, 160 + (i / 4) * 100.0f });
    }
    shuffle(positions.begin(), positions.end(), g);

    snacks.clear();
    string imageNames[12] = {
        "juice.png", "burger.png", "catto.png", "girl.png", "mirror.png", "ducsufont.png",
        "boy.png", "buns.png", "croissant.png", "duscu.png", "water.png", "cake.png"
    };

    for (int i = 0; i < 12; i++) {
        SnackCard s;
        s.rect = { positions[i].x, positions[i].y, 120, 60 };
        s.snack = allSnacks[i];
        s.revealed = false;
        s.collected = false;
        snackIcons[i] = LoadTexture(TextFormat("/mnt/c/Users/User/Pictures/Camera Roll/depts image/%s", imageNames[i].c_str()));
        s.image = snackIcons[i];
        snacks.push_back(s);
    }
    snackAttempts = 0;
    allSnacksCollected = false;
}

string ToLower(const string& s) {
    string result = s;
    transform(result.begin(), result.end(), result.begin(), [](unsigned char c){ return tolower(c); });
    return result;
}

void logic_draw_memory_challenge() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawTexture(bgImage, 0, 0, WHITE);
    DrawText("Memory Match - DUCSU", 20, 20, 28, DARKBLUE);

    Vector2 mouse = GetMousePosition();

    if (currentPhase == PHASE_START) {
        DrawRectangleRec(startButton, GRAY);
        DrawText("Start", startButton.x + 30, startButton.y + 15, 24, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, startButton)) {
            currentPhase = PHASE_CARDS;
            ShuffleCards();
        }
    }
    else if (currentPhase == PHASE_CARDS) {
        for (auto &card : cards) {
            DrawRectangleRec(card.rect, card.revealed ? LIGHTGRAY : GRAY);
            if (card.revealed || card.found) DrawText(card.word.c_str(), card.rect.x + 10, card.rect.y + 20, 16, DARKBLUE);
            if (!card.revealed && !card.found && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, card.rect)) {
                card.revealed = true;
                selectedClueWords.push_back(card.word);
                ShuffleCards();
                if (selectedClueWords.size() >= 12) {
                    cluePhrase = poeticClue + "\n" + finalClueLine;
                    currentPhase = PHASE_SNACKS;
                    ShuffleSnacks();
                }
            }
        }
    }
    else if (currentPhase == PHASE_SNACKS) {
        DrawText(cluePhrase.c_str(), 40, 40, 20, MAROON);
        for (int i = 0; i < snacks.size(); i++) {
            SnackCard &snack = snacks[i];
            if (!snack.collected) DrawTexture(snack.image, snack.rect.x, snack.rect.y, WHITE);
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, snack.rect)) {
                snackAttempts++;
                if (find(correctSnackTargets.begin(), correctSnackTargets.end(), snack.snack) != correctSnackTargets.end()) {
                    snack.collected = true;
                }
            }
        }
        if (snackAttempts >= 10 || all_of(correctSnackTargets.begin(), correctSnackTargets.end(), [](const string &s) {
            return any_of(snacks.begin(), snacks.end(), [&](SnackCard &sc) {
                return sc.snack == s && sc.collected;
            });
        })) {
            allSnacksCollected = true;
            currentPhase = PHASE_INPUT_FINAL;
        }
    }
    else if (currentPhase == PHASE_INPUT_FINAL) {
        DrawText(finalClueLine.c_str(), 80, 80, 20, DARKPURPLE);
        DrawText(finalQuestion.c_str(), 80, 140, 22, BLACK);
        DrawText(playerInput.c_str(), 80, 180, 24, BLUE);
        if (IsKeyPressed(KEY_ENTER)) {
            if (ToLower(playerInput) == correctFinalAnswer) {
                correctAnswerEntered = true;
                currentPhase = PHASE_SUCCESS;
            } else {
                currentPhase = PHASE_FAIL;
            }
        }
        int key = GetCharPressed();
        if (key > 0 && isprint(key)) playerInput += (char)key;
        if (IsKeyPressed(KEY_BACKSPACE) && !playerInput.empty()) playerInput.pop_back();
    }
    else if (currentPhase == PHASE_SUCCESS) {
        DrawText("You discovered the truth. You are the candidate!", 100, 250, 24, GREEN);
    }
    else if (currentPhase == PHASE_FAIL) {
        DrawText("Wrong answer. Try again.", 120, 250, 24, RED);
    }

    EndDrawing();
}

int main() {
    InitWindow(screenWidth, screenHeight, "DUCSU Memory Match");
    bgImage = LoadTexture("/mnt/c/Users/User/Pictures/Camera Roll/depts image/DUCSU.png");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        logic_draw_memory_challenge();
    }

    UnloadTexture(bgImage);
    for (int i = 0; i < 12; i++) UnloadTexture(snackIcons[i]);
    CloseWindow();
    return 0;
}

// Modularized Memory Match Game (DUCSU Game)
#include "raylib.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "global.h"
#include <cstdio>

#define MAX_PAIRS 8
#define MAX_CARDS (MAX_PAIRS * 2)
#define CARD_WIDTH 180
#define CARD_HEIGHT 120
#define PADDING 30
#define ROWS 4
#define COLUMNS 4
#define FLIP_TIME 0.3f

// Card Struct
typedef struct {
    Rectangle rect;
    char label[32];
    bool revealed;
    bool matched;
    float flipTimer;
    bool flipping;
} Card;

Card cards[MAX_CARDS];
int selectedIndices[2] = {-1, -1};
int matchedCount = 0;
float gameTime = 40.0f;
bool gameOver = false;
Rectangle restartButton = {350, 560, 150, 30};
Font font;
Color cardColor = (Color){200, 230, 255, 255};

const char *pairs[MAX_PAIRS][2] = {
    {"Raima", "irony"}, {"Rapunzel", "disney"},
    {"csedu", "death"}, {"Rana sir", "GOAT"},
    {"DU", "dream"}, {"Raisa", "cat"},
    {"Salwa", "high"}, {"Ahan", "guy"}
};

void shuffle_labels(char labels[MAX_CARDS][32]) {
    for (int i = MAX_CARDS - 1; i > 0; i--) {
        int j = GetRandomValue(0, i);
        char temp[32];
        strcpy(temp, labels[i]);
        strcpy(labels[i], labels[j]);
        strcpy(labels[j], temp);
    }
}

void init_memory_match() {
    InitAudioDevice();
    srand(time(NULL));

    font = LoadFont("resources/Roboto-Bold.ttf");
    if (!font.texture.id) font = GetFontDefault();

    char labels[MAX_CARDS][32];
    for (int i = 0; i < MAX_PAIRS; i++) {
        strcpy(labels[2 * i], pairs[i][0]);
        strcpy(labels[2 * i + 1], pairs[i][1]);
    }
    shuffle_labels(labels);

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

    selectedIndices[0] = selectedIndices[1] = -1;
    matchedCount = 0;
    gameTime = 40.0f;
    gameOver = false;
    ducsu_game = false;
}

void logic_memory_match() {
    float delta = GetFrameTime();
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
            bool matched = false;
            for (int i = 0; i < MAX_PAIRS; i++) {
                if ((strcmp(pairs[i][0], cards[i1].label) == 0 && strcmp(pairs[i][1], cards[i2].label) == 0) ||
                    (strcmp(pairs[i][1], cards[i1].label) == 0 && strcmp(pairs[i][0], cards[i2].label) == 0)) {
                    matched = true;
                    break;
                }
            }
            if (matched) {
                cards[i1].matched = true;
                cards[i2].matched = true;
                matchedCount++;
            } else {
                cards[i1].flipping = true;
                cards[i2].flipping = true;
            }
            selectedIndices[0] = selectedIndices[1] = -1;
        }
    }

    if (matchedCount == MAX_PAIRS) {
        gameOver = true;
        ducsu_game = true;
    }
}

void draw_memory_match(Rectangle viewRect) {
    BeginScissorMode(viewRect.x, viewRect.y, viewRect.width, viewRect.height);
    DrawRectangleRec(viewRect, RAYWHITE);

    for (int i = 0; i < MAX_CARDS; i++) {
        Rectangle r = cards[i].rect;
        r.x += viewRect.x;
        r.y += viewRect.y;

        Color borderColor = BLACK;
        DrawRectangleRec(r, cards[i].revealed || cards[i].matched ? cardColor : DARKGRAY);
        DrawRectangleLinesEx(r, 2, borderColor);

        if (cards[i].revealed || cards[i].matched) {
            Vector2 textSize = MeasureTextEx(font, cards[i].label, 24, 2);
            DrawTextEx(font, cards[i].label,
                       (Vector2){r.x + (CARD_WIDTH - textSize.x)/2,
                                 r.y + (CARD_HEIGHT - textSize.y)/2},
                       24, 2, BLACK);
        }
    }

    char timerText[64];
    snprintf(timerText, sizeof(timerText), "Time Left: %.1f", gameTime);
    DrawTextEx(font, timerText, (Vector2){viewRect.x + 680, viewRect.y + 20}, 22, 2, RED);

    if (gameOver) {
        const char* resultText = (matchedCount == MAX_PAIRS) ? "YOU WIN LESGOO!" : "WHARRA MEMORY HAHA";
        Vector2 size = MeasureTextEx(font, resultText, 38, 2);
        DrawTextEx(font, resultText, (Vector2){viewRect.x + (viewRect.width - size.x)/2, viewRect.y + viewRect.height - 100, 38, 2, BLUE);

        Rectangle btn = restartButton;
        btn.x += viewRect.x;
        btn.y += viewRect.y;
        DrawRectangleRec(btn, LIGHTGRAY);
        DrawTextEx(font, "RESTART", (Vector2){btn.x + 20, btn.y + 5}, 22, 2, BLACK);
    }

    EndScissorMode();
}

void unload_memory_match() {
    UnloadFont(font);
}

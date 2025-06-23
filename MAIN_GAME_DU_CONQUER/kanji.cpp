#include "raylib.h"
#include <vector>
#include <cmath>
#include "global.h"
#include "kanji.h"

// Stroke definition
struct Stroke {
    Vector2 start;
    Vector2 end;
    bool drawn = false;
    bool correct = false;
    std::vector<Vector2> trail;
};

const int windowWidth = 400;
const int windowHeight = 300;
float timer = 25.0f;
bool showRules = true;
bool drawing = false;
bool gameOver_kanji = false;
int currentCharacter = 0;
int currentStroke = 0;
Rectangle helpButton;
std::vector<std::vector<Stroke>> characters;
Vector2 mouse;
int centerX;
int centerY;

float Distance(Vector2 a, Vector2 b) {
    return sqrtf((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

void init_kanji() {
    centerX = (screenWidth - windowWidth) / 2;
    centerY = (screenHeight - windowHeight) / 2;
    helpButton = { screenWidth - 40, 10, 30, 30 };

    // 3 characters: 木, 王, 小 (all with straight-line strokes)
    characters = {
        // 木 (Tree)
        {
            {{0, 0}, {0, 170}},         // Vertical trunk (extends to diagonal base)
            {{-60, 30}, {60, 30}},         // Horizontal bar (slightly above center)
            {{0, 30}, {-60, 170}},         // Left diagonal (/\ shape)
            {{0, 30}, {60, 170}}          // Right diagonal
        },

        // 王 (King)
        {
            {{-50, 0}, {50, 0}},         // Top horizontal
            {{-40, 80}, {40, 80}},         // Middle horizontal
            {{-60, 160}, {40, 160}},         // Bottom horizontal
            {{0, 0}, {0, 160}}          // Vertical center line
        },

        // 小 (Small) – simplified 3 strokes
        {
            {{0, 0}, {0, 140}},           // Center vertical
            {{-40, 60}, {-10, 140}},      // Left diagonal
            {{40, 60}, {10, 140}}         // Right diagonal
        }
    };

    timer = 25.0f;
    showRules = true;
    drawing = false;
    gameOver_kanji = false;
    currentCharacter = 0;
    currentStroke = 0;
}

void logic_kanji() {
    float delta = GetFrameTime();

    if (!japanesse_game && !gameOver_kanji) {
        timer -= delta;
        if (timer <= 0.0f) gameOver_kanji = true;
    }

    mouse = GetMousePosition();

    // Handle drawing logic
    if (!gameOver_kanji && !japanesse_game && currentCharacter < characters.size()) {
        Stroke &s = characters[currentCharacter][currentStroke];

        if (!drawing && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (Distance(mouse, (Vector2){s.start.x + centerX, s.start.y + centerY}) < 15.0f) {
                drawing = true;
                s.trail.clear();
                s.trail.push_back(mouse);
            }
        }

        if (drawing && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            s.trail.push_back(mouse);
        }

        if (drawing && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            drawing = false;

            if (Distance(mouse, (Vector2){s.end.x + centerX, s.end.y + centerY}) < 15.0f) {
                s.drawn = true;
                s.correct = true;
                currentStroke++;
                if (currentStroke >= characters[currentCharacter].size()) {
                    currentCharacter++;
                    currentStroke = 0;
                    if (currentCharacter >= 3) {
                        japanesse_game = true;
                    }
                }
            } else {
                gameOver_kanji = true;
            }
        }
    }

    // Restart
    if (IsKeyPressed(KEY_R)) {
        for (auto &charStrokes : characters) {
            for (auto &s : charStrokes) {
                s.drawn = false;
                s.correct = false;
                s.trail.clear();
            }
        }
        currentCharacter = 0;
        currentStroke = 0;
        drawing = false;
        timer = 180.0f;
        gameOver_kanji = false;
        japanesse_game = false;
    }
}

void draw_kanji() {

    // Draw current character strokes
    if (currentCharacter < characters.size()) {
        auto &strokes = characters[currentCharacter];
        for (int i = 0; i < strokes.size(); i++) {
            Stroke &s = strokes[i];
            Color col = (s.drawn && s.correct) ? MAGENTA : Fade(GRAY, 0.3f);
            // Draw kanji character strokes (only magenta outlines)
            DrawLineEx((Vector2){s.start.x + centerX, s.start.y + centerY},
                       (Vector2){s.end.x + centerX, s.end.y + centerY},
                       10.0f, col);
            DrawCircleV((Vector2){s.start.x + centerX, s.start.y + centerY}, 8, col);
        }

        // Draw current stroke trail
        if (currentStroke < strokes.size()) {
            for (size_t i = 1; i < strokes[currentStroke].trail.size(); i++) {
                DrawLineEx((Vector2){strokes[currentStroke].trail[i - 1].x, strokes[currentStroke].trail[i - 1].y},
                           (Vector2){strokes[currentStroke].trail[i].x, strokes[currentStroke].trail[i].y},
                           8.0f, MAGENTA);
            }
        }
    }

    // Cursor
    DrawCircleV(mouse, 6, DARKBLUE);

    DrawText("Draw Outline of these japanesse characters", screenWidth/2 - MeasureText("Draw Outline of these japanesse characters",32)/2, centerY - 110*scale, 32, MAGENTA);
    
    // Messages
    if (japanesse_game) {
        DrawText("You mastered all the kanji!", 220, 500, 24, DARKGREEN);
    } else if (gameOver_kanji) {
        DrawText("Oops! Wrong stroke or time ran out.", 200, 500, 24, RED);
        DrawText("Press [R] to retry.", 300-1, 530-1, 20, RED);
    } else {
        DrawText(TextFormat("Time Left: %.1f", timer), centerX - 60, centerY - 70, 28, WHITE);
    }
}

void unload_kanji() {
    
}
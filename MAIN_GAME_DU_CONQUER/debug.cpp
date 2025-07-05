#include "debug.h"
#include <vector>
#include <string>
#include <cmath>
#include <ctime>
#include "global.h"

enum GamePhase {
    LOOPY_INTRO,
    LOOPY_ROUND,
    LOOPY_FAILURE,
    WHACK_TUTORIAL,
    WHACK_INIT, WHACK_PLAY, WHACK_WIN, WHACK_FAIL,
    FAILURE
};

struct Mole {
    Vector2 center;
    float radius;
    bool isNull;
    float timer;
    bool active;
    float popScale;
    float shakeOffset;
    float shakeTimer;
};

const int SSW = 900;
const int SSH = 600;
const int maxLoopyRounds = 3;

static GamePhase phase;
static bool debug_complete = false;

static Texture2D loopyTex;
static Texture2D nully;
static Texture2D nullyZombie;

static float timeLeft;
static float bounceOffset;
static bool bouncingUp;

static int loopPrintCount;
static float glowTime;
static std::vector<std::string> outputLines;
static int targetValue;
static int targetCount;
static int streamCounter;
static int currentLoopyRound;
static Rectangle breakButton;

static float whackTimer;
static int score, misses, nullClicks;
static std::vector<Mole> moles;

static bool breakSuccess;

void init_debug() {
    loopyTex = LoadTexture("resources/loopy_sad.png");
    nully = LoadTexture("resources/nully.png");
    nullyZombie = LoadTexture("resources/nully_zombie.png");

    phase = LOOPY_INTRO;
    timeLeft = 60.0f;
    bounceOffset = 0.0f;
    bouncingUp = true;

    loopPrintCount = 0;
    glowTime = 0.0f;
    outputLines.clear();
    targetValue = GetRandomValue(1, 9);
    targetCount = GetRandomValue(5, 15);
    streamCounter = 0;
    currentLoopyRound = 0;
    breakButton = { SSW / 2 - 60, 480, 120, 40 };

    whackTimer = 30.0f;
    score = misses = nullClicks = 0;

    moles.resize(6);
    float moleRadius = 60.0f;
    for (int i = 0; i < 6; i++) {
        float x = 180 + (i % 3) * 250;
        float y = 220 + (i / 3) * 200;
        moles[i] = { {x, y}, moleRadius, false, 0, false, 0, 0, 0 };
    }

    breakSuccess = false;

    debug_complete = false;
}

void logic_debug() {
    float delta = GetFrameTime();

    if (IsKeyPressed(KEY_R) && (phase == LOOPY_FAILURE || phase == WHACK_FAIL || phase == FAILURE)) {
        init_debug();
        return;
    }

    if (phase != WHACK_PLAY) timeLeft -= delta;
    if (timeLeft <= 0 && phase < WHACK_PLAY) phase = FAILURE;

    if (phase < WHACK_PLAY) {
        bounceOffset += (bouncingUp ? -1 : 1) * 40.0f * delta;
        if (bounceOffset <= -10) bouncingUp = false;
        if (bounceOffset >= 0) bouncingUp = true;
    }

    switch (phase) {
        case LOOPY_INTRO:
            if (IsKeyPressed(KEY_ENTER)) phase = LOOPY_ROUND;
            break;

        case LOOPY_ROUND:
            glowTime += delta;
            streamCounter++;
            if (streamCounter % 25 == 0 && loopPrintCount < targetCount + 30) {
                const int maxCharsPerLine = 60;
                if (outputLines.empty()) outputLines.push_back("");
                if ((int)outputLines.back().length() < maxCharsPerLine)
                    outputLines.back() += TextFormat("%d", targetValue);
                else if ((int)outputLines.size() < 25)
                    outputLines.push_back(TextFormat("%d", targetValue));
                loopPrintCount++;
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();
                if (CheckCollisionPointRec(mouse, breakButton)) {
                    if (loopPrintCount >= targetCount && loopPrintCount < targetCount + 5) {
                        breakSuccess = true;
                        currentLoopyRound++;
                        if (currentLoopyRound >= maxLoopyRounds)
                            phase = WHACK_TUTORIAL;
                        else {
                            targetValue = GetRandomValue(1, 9);
                            targetCount = GetRandomValue(5, 10);
                            loopPrintCount = 0;
                            outputLines.clear();
                            streamCounter = 0;
                            breakSuccess = false;
                        }
                    } else {
                        phase = LOOPY_FAILURE;
                    }
                }
            }
            break;

        case WHACK_TUTORIAL:
            if (IsKeyPressed(KEY_SPACE)) {
                whackTimer = 30.0f;
                score = misses = nullClicks = 0;
                for (auto &m : moles) {
                    m.active = false;
                    m.timer = 0;
                    m.popScale = 0;
                    m.shakeTimer = 0;
                }
                phase = WHACK_PLAY;
            }
            break;

        case WHACK_PLAY:
            whackTimer -= delta;
            if (whackTimer <= 0) phase = WHACK_FAIL;

            for (auto &mole : moles) {
                if (!mole.active && GetRandomValue(0, 100) < 2) {
                    mole.isNull = GetRandomValue(0, 1);
                    mole.active = true;
                    mole.timer = 1.5f;
                    mole.popScale = 0;
                }

                if (mole.active) {
                    mole.timer -= delta;
                    if (mole.popScale < 1.0f) mole.popScale += delta * 5.0f;
                    if (mole.timer <= 0) {
                        mole.active = false;
                        if (mole.isNull) {
                            misses++;
                            if (misses >= 5) phase = WHACK_FAIL;
                        }
                    }
                }

                if (mole.shakeTimer > 0) {
                    mole.shakeTimer -= delta;
                    mole.shakeOffset = sinf(GetTime() * 50) * 5;
                } else {
                    mole.shakeOffset = 0;
                }
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                Vector2 mouse = GetMousePosition();
                for (auto &mole : moles) {
                    if (mole.active && CheckCollisionPointCircle(mouse, mole.center, mole.radius)) {
                        if (mole.isNull) {
                            score++;
                            if (score >= 5) {
                                phase = WHACK_WIN;
                                debug_complete = true;
                            }
                        } else {
                            nullClicks++;
                            mole.shakeTimer = 0.3f;
                            if (nullClicks >= 3) phase = WHACK_FAIL;
                        }
                        mole.active = false;
                        break;
                    }
                }
            }
            break;

        case LOOPY_FAILURE:
            break;

        case FAILURE:
            break;

        default:
            break;
    }
}

void draw_debug() {
    DrawText("Loopy + Whack-a-Null", 20, 20, 30, DARKGRAY);
    DrawText(TextFormat("Timer: %.1f", (phase < WHACK_PLAY ? timeLeft : whackTimer)), 20, 60, 20, RED);

    if (phase < WHACK_TUTORIAL) {
        DrawTexture(loopyTex, SSW / 2 - loopyTex.width / 2, 100 + bounceOffset, WHITE);
    }

    switch (phase) {
        case LOOPY_INTRO:
            DrawText("Hi, I'm Loopy!", 300, 300, 40, LIME);
            DrawText("I can't stop spinning...", 260, 350, 30, GREEN);
            DrawText("Press ENTER to help me break the loop.", 200, 400, 20, YELLOW);
            break;

        case LOOPY_ROUND: {
            Rectangle terminal = {100, 380, 600, 80};
            DrawText(TextFormat("for(int i=0; ;i++) cout<<\"%%d\"; if (i==%d) break;", targetCount - 1),
                     terminal.x, terminal.y - 40, 22, DARKGRAY);
            DrawRectangleRounded(terminal, 0.1f, 16, DARKGRAY);
            float alpha = 128 + 127 * sinf(glowTime * 3);
            DrawRectangleLinesEx(terminal, 4, Color{0, 255, 0, (unsigned char)alpha});
            for (int i = 0; i < (int)outputLines.size(); i++) {
                DrawText(outputLines[i].c_str(), terminal.x + 10, terminal.y + 10 + i * 24, 24, LIME);
            }
            DrawRectangleRec(breakButton, LIGHTGRAY);
            DrawText("BREAK", breakButton.x + 25, breakButton.y + 10, 24, RED);
            if (breakSuccess) {
                DrawText("OK!", breakButton.x + 30, breakButton.y + 50, 20, GREEN);
            }
        } break;

        case WHACK_TUTORIAL:
            DrawText("This is Nully", 120, 100, 30, GREEN);
            DrawTexture(nully, 100, 150, WHITE);
            DrawText("This is Zombie Nully (bug)", 500, 100, 30, RED);
            DrawTexture(nullyZombie, 500, 150, WHITE);
            DrawText("Press SPACE to begin", 300, 500, 24, YELLOW);
            break;

        case WHACK_PLAY:
            DrawText(TextFormat("Score: %d", score), 20, 90, 20, GREEN);
            DrawText(TextFormat("Misses: %d", misses), 20, 110, 20, ORANGE);
            DrawText(TextFormat("Wrong Clicks: %d / 3", nullClicks), 20, 130, 20, MAROON);
            for (auto &mole : moles) {
                DrawCircleV(mole.center, mole.radius, LIGHTGRAY);
                if (mole.active) {
                    Texture2D tex = mole.isNull ? nullyZombie : nully;
                    float scale = mole.popScale;
                    float offset = mole.shakeOffset;
                    DrawTextureEx(tex, {mole.center.x - tex.width * scale / 2 + offset, mole.center.y - tex.height * scale + 70}, 0.0f, scale, WHITE);
                }
            }
            break;
        case WHACK_WIN:
            cse_game = true;
            break;
        case WHACK_FAIL:
            DrawText(TextFormat("Score: %d", score), 20, 90, 20, GREEN);
            DrawText(TextFormat("Misses: %d", misses), 20, 110, 20, ORANGE);
            DrawText(TextFormat("Wrong Clicks: %d / 3", nullClicks), 20, 130, 20, MAROON);
            for (auto &mole : moles) {
                DrawCircleV(mole.center, mole.radius, LIGHTGRAY);
                if (mole.active) {
                    Texture2D tex = mole.isNull ? nullyZombie : nully;
                    float scale = mole.popScale;
                    float offset = mole.shakeOffset;
                    DrawTextureEx(tex, {mole.center.x - tex.width * scale / 2 + offset, mole.center.y - tex.height * scale + 70}, 0.0f, scale, WHITE);
                }
            }
            if (phase == WHACK_WIN)
                DrawText("You stabilized memory! Good job!", 200, 520, 24, GREEN);
            else if (phase == WHACK_FAIL) {
                DrawText("Memory corrupted! Game Over!", 200, 520, 24, RED);
                DrawText("Press 'R' to Restart", 200, 550, 20, GRAY);
            }
            break;

        case LOOPY_FAILURE:
            DrawText("Wrong timing! Loopy is stuck forever...", 100, 420, 24, MAROON);
            DrawText("Press 'R' to Restart", 100, 460, 20, GRAY);
            break;

        case FAILURE:
            DrawText("Too late! Loopy corrupted your code.", 100, 420, 24, MAROON);
            DrawText("Press 'R' to Restart", 100, 460, 20, GRAY);
            break;

        default:
            break;
    }
}

void unload_debug() {
    UnloadTexture(loopyTex);
    UnloadTexture(nully);
    UnloadTexture(nullyZombie);
}

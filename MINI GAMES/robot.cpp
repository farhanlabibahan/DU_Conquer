#include "raylib.h"
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <string>

struct Component {
    Texture2D texture;
    Rectangle bounds;
    bool dragging = false;
    Vector2 offset = {0, 0};
    int id;
};

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int START_Y = 100;
const int SLOT_Y = 300;
const int SLOT_WIDTH = 140;
const int SLOT_HEIGHT = 140;
const int COMPONENT_SIZE = 120;
const int SLOT_X_OFFSET = 160;

std::vector<int> GenerateAnswer() {
    std::vector<int> order = {0, 1, 2, 3};
    std::shuffle(order.begin(), order.end(), std::default_random_engine((unsigned)time(0)));
    return order;
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Collect the Tools");
    SetTargetFPS(60);

    std::vector<Texture2D> componentTextures = {
        LoadTexture("motionsensor.png"),
        LoadTexture("motor.png"),
        LoadTexture("wire.png"),
        LoadTexture("iron.png")
    };

    std::vector<Component> components;
    std::vector<Vector2> originalPos;

    for (int i = 0; i < 4; i++) {
        Component c;
        c.texture = componentTextures[i];
        c.bounds = {
            (float)(100 + i * SLOT_X_OFFSET),
            (float)START_Y,
            (float)COMPONENT_SIZE,
            (float)COMPONENT_SIZE
        };
        c.id = i;
        components.push_back(c);
        originalPos.push_back({ c.bounds.x, c.bounds.y });
    }

    std::vector<int> correctOrder = GenerateAnswer();
    int slots[4] = {-1, -1, -1, -1};
    bool submitted = false;
    int correctCount = 0;

    // 🕒 Timer + reset limit
    float timer = 60.0f;
    bool timeUp = false;
    int resetCount = 5;

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        // Decrease timer if game is active
        if (!submitted && !timeUp) {
            timer -= delta;
            if (timer <= 0.0f) {
                timer = 0.0f;
                timeUp = true;
            }
        }

        Vector2 mouse = GetMousePosition();

        for (auto &c : components) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, c.bounds)) {
                c.dragging = true;
                c.offset = { mouse.x - c.bounds.x, mouse.y - c.bounds.y };
            }
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) c.dragging = false;
            if (c.dragging) {
                c.bounds.x = mouse.x - c.offset.x;
                c.bounds.y = mouse.y - c.offset.y;
            }
        }

        // ✅ Submit if time not up
        if (IsKeyPressed(KEY_SPACE) && !submitted && !timeUp) {
            submitted = true;
            correctCount = 0;

            for (int i = 0; i < 4; i++) {
                slots[i] = -1;
                for (int j = 0; j < 4; j++) {
                    Rectangle slot = {
                        (float)(100 + i * SLOT_X_OFFSET),
                        (float)SLOT_Y,
                        (float)SLOT_WIDTH,
                        (float)SLOT_HEIGHT
                    };
                    if (CheckCollisionRecs(slot, components[j].bounds)) {
                        slots[i] = components[j].id;
                        components[j].bounds.x = slot.x + (SLOT_WIDTH - COMPONENT_SIZE) / 2;
                        components[j].bounds.y = slot.y + (SLOT_HEIGHT - COMPONENT_SIZE) / 2;
                        break;
                    }
                }
                if (slots[i] == correctOrder[i]) correctCount++;
            }
            resetCount--;
        }
        

        // 🔁 Limited Resets
        if (IsKeyPressed(KEY_R) && resetCount > 0 && !timeUp) {
            for (int i = 0; i < 4; i++) {
                components[i].bounds.x = originalPos[i].x;
                components[i].bounds.y = originalPos[i].y;
            }
            submitted = false;
            correctCount = 0;
           // resetCount--;
        }

        // ==== Drawing ====
        BeginDrawing();
        ClearBackground(BLACK);

        DrawText("Drag the components to the slots below!", 20, 20, 20, MAROON);
        DrawText("Press SPACE to submit. Press R to reset.", 20, 50, 20, BLUE);

        // ⏱️ Draw timer
        std::string timeStr = "Time: " + std::to_string((int)timer) + "s";
        DrawText(timeStr.c_str(), 650, 20, 20, YELLOW);

        // 🔁 Draw remaining resets
        std::string resetStr = "Tries left: " + std::to_string(resetCount);
        DrawText(resetStr.c_str(), 620, 50, 20, ORANGE);

        // Draw slots
        for (int i = 0; i < 4; i++) {
            DrawRectangleLines(
                100 + i * SLOT_X_OFFSET,
                SLOT_Y,
                SLOT_WIDTH,
                SLOT_HEIGHT,
                GRAY
            );
        }

        // Draw components
        for (auto &c : components) {
            DrawTextureEx(
                c.texture,
                { c.bounds.x, c.bounds.y },
                0.0f,
                COMPONENT_SIZE / (float)c.texture.width,
                WHITE
            );
        }

        // Show result
        if (submitted) {
            std::string msg = "Correct: " + std::to_string(correctCount) + "/4";
            DrawText(msg.c_str(), 20, 500, 30, GREEN);

            std::string percent = std::to_string(correctCount * 25) + "% correct!";
            DrawText(percent.c_str(), 20, 540, 30, BLUE);
        } else if (timeUp) {
            DrawText("Time's Up!", 20, 500, 40, RED);
        }

        EndDrawing();
    }

    for (auto &t : componentTextures) UnloadTexture(t);
    CloseWindow();
    return 0;
}

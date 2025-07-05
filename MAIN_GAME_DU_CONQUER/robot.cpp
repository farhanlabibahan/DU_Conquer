// robot.cpp (modular version for integration)
#include <raylib.h>
#include "robot.h"
#include "global.h"
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>
#include <string>

// Component structure
struct Component {
    Texture2D texture;
    Rectangle bounds;
    bool dragging = false;
    Vector2 offset = {0, 0};
    int id;
};

// Constants
 const int SCREEN_WIDTH = 800;
 const int SCREEN_HEIGHT = 600;
 const int START_Y = 100;
 const int SLOT_Y = 300;
 const int SLOT_WIDTH = 140;
 const int SLOT_HEIGHT = 140;
 const int COMPONENT_SIZE = 120;
 const int SLOT_X_OFFSET = 160;

// Game state variables
static std::vector<Texture2D> componentTextures;
static std::vector<Component> components;
static std::vector<Vector2> originalPos;
static std::vector<int> correctOrder;
static int slots[4];
static bool submitted = false;
static int correctCount = 0;
// static float timer = 60.0f;
// static bool timeUp = false;
static int resetCount = 5;

// Random answer generator
static std::vector<int> GenerateAnswer() {
    std::vector<int> order = {0, 1, 2, 3};
    std::shuffle(order.begin(), order.end(), std::default_random_engine((unsigned)time(0)));
    return order;
}

// Init game (no InitWindow here!)
void init_robot() {
    SetTargetFPS(60);

    componentTextures = {
        LoadTexture("resources/motionsensor.png"),
        LoadTexture("resources/motor.png"),
        LoadTexture("resources/wire.png"),
        LoadTexture("resources/iron.png")
    };

    components.clear();
    originalPos.clear();

    for (int i = 0; i < 4; i++) {
        Component c;
        c.texture = componentTextures[i];
        c.bounds = {(float)(100 + i * SLOT_X_OFFSET), (float)START_Y, (float)COMPONENT_SIZE, (float)COMPONENT_SIZE};
        c.id = i;
        components.push_back(c);
        originalPos.push_back({c.bounds.x, c.bounds.y});
    }

    correctOrder = GenerateAnswer();
    for (int &s : slots) s = -1;
    submitted = false;
    correctCount = 0;
    // timer = 60.0f;
    // timeUp = false;
}

// Game logic
void logic_robot() {
   
    // float delta = GetFrameTime();
    // if (!submitted && !timeUp) {
    //     timer -= delta;
    //     if (timer <= 0.0f) {
    //         timer = 0.0f;
    //         timeUp = true;
    //     }
    // }

    Vector2 mouse = GetMousePosition();
    for (auto &c : components) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, c.bounds)) {
            c.dragging = true;
            c.offset = {mouse.x - c.bounds.x, mouse.y - c.bounds.y};
        }
        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) c.dragging = false;
        if (c.dragging) {
            c.bounds.x = mouse.x - c.offset.x;
            c.bounds.y = mouse.y - c.offset.y;
        }
    }

    // Submit solution
    if (IsKeyPressed(KEY_SPACE) && !submitted) {
        submitted = true;
        correctCount = 0;

        for (int i = 0; i < 4; i++) {
            slots[i] = -1;
            for (int j = 0; j < 4; j++) {
                Rectangle slot = {(float)(100 + i * SLOT_X_OFFSET), (float)SLOT_Y, (float)SLOT_WIDTH, (float)SLOT_HEIGHT};
                if (CheckCollisionRecs(slot, components[j].bounds)) {
                    slots[i] = components[j].id;
                    components[j].bounds.x = slot.x + (SLOT_WIDTH - COMPONENT_SIZE) / 2;
                    components[j].bounds.y = slot.y + (SLOT_HEIGHT - COMPONENT_SIZE) / 2;
                    break;
                }
            }
            if (slots[i] == correctOrder[i]) correctCount++;
        }
      bool win= (correctCount==4);
        // resetCount--; // Removed decrement of resetCount
    }
    
    // Reset try
    if (IsKeyPressed(KEY_R)) {
        for (int i = 0; i < 4; i++) {
            components[i].bounds.x = originalPos[i].x;
            components[i].bounds.y = originalPos[i].y;
        }
        submitted = false;
        correctCount = 0;
    }
}

// Drawing 
void draw_robot() {
    // ClearBackground(BLACK);

    DrawText("Drag the components to the slots below!", 20, 20, 20, MAROON);
    DrawText("Press SPACE to submit. Press R to reset.", 20, 50, 20, BLUE);

    // DrawText(("Time: " + std::to_string((int)timer) + "s").c_str(), 650, 20, 20, YELLOW);
    // DrawText(("Tries left: " + std::to_string(resetCount)).c_str(), 620, 50, 20, ORANGE);

    for (int i = 0; i < 4; i++) {
        DrawRectangleLines(100 + i * SLOT_X_OFFSET, SLOT_Y, SLOT_WIDTH, SLOT_HEIGHT, GRAY);
    }

    for (auto &c : components) {
        DrawTextureEx(c.texture, {c.bounds.x, c.bounds.y}, 0.0f, COMPONENT_SIZE / (float)c.texture.width, WHITE);
    }

    if (submitted) {
        DrawText(("Correct: " + std::to_string(correctCount) + "/4").c_str(), 20, 500, 30, GREEN);
        if(correctCount == 4) robotics_game = true;
        DrawText((std::to_string(correctCount * 25) + "% correct!").c_str(), 20, 540, 30, BLUE);
    } 
    // else if (timeUp) {
    //     DrawText("Time's Up!", 20, 500, 40, RED);
    // }
}

// Unload resources 
void unload_robot() {
    for (auto &t : componentTextures) UnloadTexture(t);
    componentTextures.clear();
}
#include "gear.h"
#include "raylib.h"
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "global.h"
#include "raymath.h"

#define GRID_SIZE 20
#define MAX_GEARS 8
#define TOUCH_TOLERANCE 5.0f  // Tight distance tolerance

static std::vector<Gear> gears;
static int draggingIndex = -1;
static Vector2 offset = { 0, 0 };
static bool liftFixed = false;

const float DEFAULT_GEAR_SPEED = 60.0f;
const float FIXED_GEAR_SCALE = 0.18f;

static Texture2D gearTexture;

static bool AreTouching(Gear a, Gear b) {
    float radiusA = (gearTexture.width * a.scale) / 2.0f;
    float radiusB = (gearTexture.width * b.scale) / 2.0f;

    float dx = a.position.x - b.position.x;
    float dy = a.position.y - b.position.y;
    float distance = sqrtf(dx * dx + dy * dy);

    float expected = radiusA + radiusB;

    // Return true only if they are nearly touching
    return (distance >= expected - TOUCH_TOLERANCE) &&
           (distance <= expected + TOUCH_TOLERANCE);
}

void init_gear() {
    gearTexture = LoadTexture("resources/gear.png");
    gears.clear();
    srand((unsigned int)time(NULL));

    for (int i = 0; i < MAX_GEARS; i++) {
        Gear g;
        g.position = {
            (float)GetRandomValue(100, GetScreenWidth() - 100),
            (float)GetRandomValue(100, GetScreenHeight() - 100)
        };
        g.startPosition = g.position;
        g.rotation = 0;
        g.startRotation = 0;
        g.fixed = (i == 0);
        g.angularVelocity = (i == 0) ? DEFAULT_GEAR_SPEED : 0.0f;
        g.selected = false;
        g.connected = false;
        g.correctDirection = false;
        g.scale = GetRandomValue(10, 40) / 100.0f;
        gears.push_back(g);
    }
}

void unload_gear() {
    UnloadTexture(gearTexture);
    gears.clear();
}

void draw_gear() {
    for (const Gear& gear : gears) {
        float scale = gear.scale;
        float texWidth = gearTexture.width * scale;
        float texHeight = gearTexture.height * scale;

        Rectangle src = { 0.0f, 0.0f, (float)gearTexture.width, (float)gearTexture.height };
        Rectangle dest = { gear.position.x, gear.position.y, texWidth, texHeight };
        Vector2 origin = { texWidth / 2.0f, texHeight / 2.0f };

        DrawTexturePro(gearTexture, src, dest, origin, gear.rotation, WHITE);

        // Optional debug visual
        // DrawCircleLines(gear.position.x, gear.position.y, texWidth / 2.0f, RED);
    }
}

void logic_gear() {
    float deltaTime = GetFrameTime();
    Vector2 mouse = GetMousePosition();

    if (IsKeyPressed(KEY_R)) {
        for (int i = 0; i < (int)gears.size(); i++) {
            gears[i].position = gears[i].startPosition;
            gears[i].rotation = gears[i].startRotation;
            gears[i].angularVelocity = (i == 0) ? DEFAULT_GEAR_SPEED : 0.0f;
            gears[i].fixed = (i == 0);
        }
    }

    // Dragging logic
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = 0; i < (int)gears.size(); i++) {
            float radius = (gearTexture.width * gears[i].scale) / 2.0f;
            if (!gears[i].fixed && fabs(gears[i].angularVelocity) < 1e-3f && CheckCollisionPointCircle(mouse, gears[i].position, radius)) {
                draggingIndex = i;
                offset = Vector2Subtract(mouse, gears[i].position);
                gears[i].selected = true;
                break;
            }
        }
    } else if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if (draggingIndex != -1) {
            gears[draggingIndex].selected = false;
            // Removed snapping for full manual alignment
            // gears[draggingIndex].position = SnapToGrid(gears[draggingIndex].position, GRID_SIZE);
            draggingIndex = -1;
        }
    }

    if (draggingIndex != -1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        gears[draggingIndex].position = Vector2Subtract(mouse, offset);
    }

    // Reset connection states
    for (auto& gear : gears) {
        gear.connected = false;
        gear.correctDirection = false;
        if (!gear.fixed) gear.angularVelocity = 0.0f;
    }

    std::vector<bool> visited(gears.size(), false);
    std::vector<int> parents(gears.size(), -1);

    // Rotation propagation
    for (int i = 0; i < (int)gears.size(); i++) {
        if (gears[i].fixed && fabs(gears[i].angularVelocity) > 0.0f) {
            visited[i] = true;
            gears[i].connected = true;

            std::vector<int> queue = { i };
            while (!queue.empty()) {
                int current = queue.back();
                queue.pop_back();

                float rCurrent = (gearTexture.width * gears[current].scale) / 2.0f;

                for (int j = 0; j < (int)gears.size(); j++) {
                    if (visited[j]) continue;
                    if (AreTouching(gears[current], gears[j])) {
                        visited[j] = true;
                        gears[j].connected = true;
                        parents[j] = current;

                        float rJ = (gearTexture.width * gears[j].scale) / 2.0f;

                        gears[j].angularVelocity = -gears[current].angularVelocity * (rCurrent / rJ);
                        gears[j].fixed = true;
                        queue.push_back(j);
                    }
                }
            }
        }
    }

    // Rotate all connected gears
    for (auto& gear : gears) {
        gear.rotation += gear.angularVelocity * deltaTime;
        if (gear.rotation >= 360.0f) gear.rotation -= 360.0f;
        if (gear.rotation < 0.0f) gear.rotation += 360.0f;
    }

    // Check if all gears are connected and spinning (not near zero angular velocity)
    liftFixed = true;
    for (const auto& gear : gears) {
        if (!gear.connected || fabs(gear.angularVelocity) < 1e-2f) {
            liftFixed = false;
            break;
        }
    }
    if (liftFixed) {
        gear_game = true;
    }
}

std::vector<Gear>& get_gears() {
    return gears;
}

bool is_lift_ready() {
    return liftFixed;
}
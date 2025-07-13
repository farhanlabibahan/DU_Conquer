#include "raylib.h"

struct Gear {
    Vector2 position;
    Vector2 startPosition;
    float rotation;
    float startRotation;
    bool fixed;
    float angularVelocity;
    bool selected;
    bool connected;
    bool correctDirection;
    float scale;
};

void init_gear();
void logic_gear();
void draw_gear();
void unload_gear();
#include "raylib.h"
#include "character.h"

Texture2D character;
Vector2 position = { -20, 410 };
Rectangle frameRec;
int currentFrame = 0;
int framesCounter = 0;
int framesSpeed = 11;
bool facingRight = true;
bool moving = false;

void init_character() {
    character = LoadTexture("LEFT_SS.png");

    position = { -20, 410 };

    frameRec = {
        0.0f, 0.0f,
        (float)character.width / 4,
        (float)character.height / 2
    };
}

void walk_character_dept() {
    moving = false;

    if (IsKeyDown(KEY_A)) {
        facingRight = false;
        position.x -= 2.0f;
        moving = true;
    }
    else if (IsKeyDown(KEY_D)) {
        facingRight = true;
        position.x += 2.0f;
        moving = true;
    }

    if (moving) {
        framesCounter++;
        if (framesCounter >= (60 / framesSpeed)) {
            framesCounter = 0;
            currentFrame++;
            if (currentFrame > 7) currentFrame = 0;
        }
    } else {
        currentFrame = 0;
    }
}

void draw_char_dept() {
    ClearBackground(RAYWHITE);

    int frameRow = currentFrame / 4;
    int frameCol = currentFrame % 4;

    Rectangle source = {
        frameCol * frameRec.width,
        frameRow * frameRec.height,
        frameRec.width,
        frameRec.height
    };

    if (facingRight) {
        source.width = -frameRec.width;
        source.x += frameRec.width;
    }

    Rectangle dest = {
        position.x,
        position.y,
        frameRec.width,
        frameRec.height
    };

    Vector2 origin = { 0, 0 };

    DrawTexturePro(character, source, dest, origin, 0.0f, WHITE);
}

void unload_character() {
    UnloadTexture(character);
}

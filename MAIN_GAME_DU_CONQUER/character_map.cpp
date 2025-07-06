#include "raylib.h"
#include <iostream>
#include "character_map.h"
#include "global.h"
#include <math.h>

using namespace std;

Texture2D character_mapl, character_mapr, character_mapu, character_mapd;

Rectangle frameRecmap,  src, dest;

int currentFramemap = 0;
int frameRow, frameCol;
int framesCountermap = 0;

int framesSpeedmap = 11;

bool facingRightmap = true;
bool facingLeftmap = false;
bool facingUp = false;
bool facingDown = false;

bool movingmap= false;

float speed = 3.0f;
float theta = 26.2f * DEG2RAD;
float cosTheta = cosf(theta);
float sinTheta = sinf(theta);

void init_character_map() {
    character_mapl = LoadTexture("resources/sprite_left.png");
    character_mapr = LoadTexture("resources/sprite_right.png");
    character_mapu = LoadTexture("resources/back.png");
    character_mapd = LoadTexture("resources/ss_up2.png");



    frameRecmap= {
        0.0f, 0.0f,
        (float)character_mapd.width / 5,
        (float)character_mapd.height / 2
    };
}

Vector2 walk_character_map() {
   
    movingmap = false;
    Vector2 offsetmap = { 0, 0 };

    // Left
    if (IsKeyDown(KEY_A)) {
        facingRightmap = false;
        facingLeftmap = true;
        facingUp = false;
        facingDown = false;

        offsetmap.x -= speed * cosTheta;
        offsetmap.y -= speed * sinTheta;
        movingmap = true;
    }
    // Right
    else if (IsKeyDown(KEY_D)) {
        facingRightmap = true;
        facingLeftmap = false;
        facingUp = false;
        facingDown = false;

        offsetmap.x += speed * cosTheta;
        offsetmap.y += speed * sinTheta;
        movingmap = true;
    }
    // Down
    else if (IsKeyDown(KEY_S)) {
        facingDown = true;
        facingUp = false;
        facingLeftmap = false;
        facingRightmap = false;

        offsetmap.x -= speed * cosTheta;
        offsetmap.y += speed * sinTheta;
        movingmap = true;
    }
    // Up
    else if (IsKeyDown(KEY_W)) {
        facingUp = true;
        facingDown = false;
        facingLeftmap = false;
        facingRightmap = false;

        offsetmap.x += speed * cosTheta;
        offsetmap.y -= speed * sinTheta;
        movingmap = true;
    }

    // Animate horizontal movement
    if (movingmap) {
        framesCountermap++;
        if (framesCountermap >= (60 / framesSpeedmap)) {
            framesCountermap = 0;
            currentFramemap++;
            if (currentFramemap > 9) currentFramemap = 0;
        }
    }
    else {
        currentFramemap = 0;
    }

    // Animate vertical movement
    

    return offsetmap;
}

void draw_char_map(Vector2 pos) {

    if (movingmap) {
        frameRow = currentFramemap / 5;
        frameCol = currentFramemap % 5;

        src = {
            frameCol * frameRecmap.width,
            frameRow* frameRecmap.height,
            frameRecmap.width,
            frameRecmap.height
        };
    }
    
    else {
        // Idle frame fallback
        if (facingLeftmap || facingRightmap) {
            src = {
                0,
                0,
                frameRecmap.width,
                frameRecmap.height
            };
        }
        else if (facingDown) {
            src = {
                0,
                0,
                frameRecmap.width,
                frameRecmap.height
            };
        }
        else {
            src = {
                0,
                0,
                frameRecmap.width,
                frameRecmap.height
            };
        }
    }

    float targetHeight = 42.0f;
    float scale = targetHeight / src.height;
    Rectangle dest = {
        pos.x,
        pos.y,
        src.width * scale,
        src.height * scale
    };

    Vector2 origin = { dest.width / 2, dest.height / 2 };


    if (facingLeftmap)
        DrawTexturePro(character_mapl, src, dest, origin, 0.0f, WHITE);
    else if (facingRightmap)
        DrawTexturePro(character_mapr, src, dest, origin, 0.0f, WHITE);
    else if (facingDown)
        DrawTexturePro(character_mapd, src, dest, origin, 0.0f, WHITE);
    else if (facingUp)
        DrawTexturePro(character_mapu, src, dest, origin, 0.0f, WHITE);
}

void unload_character_map() {
    UnloadTexture(character_mapu);
    UnloadTexture(character_mapd);
    UnloadTexture(character_mapl);
    UnloadTexture(character_mapr);
}
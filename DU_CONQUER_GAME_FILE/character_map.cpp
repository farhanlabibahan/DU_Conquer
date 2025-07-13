#include "raylib.h"
#include <iostream>
#include "character_map.h"
#include "global.h"
#include <math.h>

using namespace std;

Texture2D character_mapl, character_mapr, character_mapu, character_mapd;

Rectangle frameRecmaph, frameRecmapv, frameRecmapd, src, dest;
int currentFramemaph = 0;
int currentFramemapv = 0;
int frameRowh, frameColh, frameRowv, frameColv;
int framesCountermaph = 0;
int framesCountermapv = 0;
int framesSpeedmap = 11;

bool facingRightmap = true;
bool facingLeftmap = false;
bool facingUp = false;
bool facingDown = false;
bool movingmaph = false;
bool movingmapv = false;

float speed = 3.0f;
float theta = 26.2f * DEG2RAD;
float cosTheta = cosf(theta);
float sinTheta = sinf(theta);

void init_character_map() {
    character_mapl = LoadTexture("resources/sprite_left.png");
    character_mapr = LoadTexture("resources/sprite_right.png");
    character_mapu = LoadTexture("resources/back.png");
    character_mapd = LoadTexture("resources/ss_up2.png");

    frameRecmaph = {
        0.0f, 0.0f,
        (float)character_mapl.width / 5,
        (float)character_mapl.height / 2
    };

    frameRecmapv = {
        0.0f, 0.0f,
        (float)character_mapu.width / 5,
        (float)character_mapu.height / 2
    };

    frameRecmapd = {
        0.0f, 0.0f,
        (float)character_mapd.width / 5,
        (float)character_mapd.height / 2
    };
}

Vector2 walk_character_map() {
    movingmaph = false;
    movingmapv = false;
    Vector2 offsetmap = { 0, 0 };

    // Left
    if (IsKeyDown(KEY_A)) {
        facingRightmap = false;
        facingLeftmap = true;
        facingUp = false;
        facingDown = false;

        offsetmap.x -= speed * cosTheta;
        offsetmap.y -= speed * sinTheta;
        movingmaph = true;
    }
    // Right
    else if (IsKeyDown(KEY_D)) {
        facingRightmap = true;
        facingLeftmap = false;
        facingUp = false;
        facingDown = false;

        offsetmap.x += speed * cosTheta;
        offsetmap.y += speed * sinTheta;
        movingmaph = true;
    }
    // Down
    else if (IsKeyDown(KEY_S)) {
        facingDown = true;
        facingUp = false;
        facingLeftmap = false;
        facingRightmap = false;

        offsetmap.x -= speed * cosTheta;
        offsetmap.y += speed * sinTheta;
        movingmapv = true;
    }
    // Up
    else if (IsKeyDown(KEY_W)) {
        facingUp = true;
        facingDown = false;
        facingLeftmap = false;
        facingRightmap = false;

        offsetmap.x += speed * cosTheta;
        offsetmap.y -= speed * sinTheta;
        movingmapv = true;
    }

    // Animate horizontal movement
    if (movingmaph) {
        framesCountermaph++;
        if (framesCountermaph >= (60 / framesSpeedmap)) {
            framesCountermaph = 0;
            currentFramemaph++;
            if (currentFramemaph > 9) currentFramemaph = 0;
        }
    }
    else {
        currentFramemaph = 0;
    }

    // Animate vertical movement
    if (movingmapv) {
        framesCountermapv++;
        if (framesCountermapv >= (60 / framesSpeedmap)) {
            framesCountermapv = 0;
            currentFramemapv++;
            if (currentFramemapv > 9) currentFramemapv = 0;
        }
    }
    else {
        currentFramemapv = 0;
    }

    return offsetmap;
}

void draw_char_map(Vector2 pos) {

    if (movingmaph) {
        frameRowh = currentFramemaph / 5;
        frameColh = currentFramemaph % 5;

        src = {
            frameColh * frameRecmaph.width,
            frameRowh * frameRecmaph.height,
            frameRecmaph.width,
            frameRecmaph.height
        };
    }
    else if (movingmapv) {
        frameRowv = currentFramemapv / 5;
        frameColv = currentFramemapv % 5;

        src = {
            frameColv * (facingDown ? frameRecmapd.width : frameRecmapv.width),
            frameRowv * (facingDown ? frameRecmapd.height : frameRecmapv.height),
            (facingDown ? frameRecmapd.width : frameRecmapv.width),
            (facingDown ? frameRecmapd.height : frameRecmapv.height)
        };
    }
    else {
        // Idle frame fallback
        if (facingLeftmap || facingRightmap) {
            src = {
                0,
                0,
                frameRecmaph.width,
                frameRecmaph.height
            };
        }
        else if (facingDown) {
            src = {
                0,
                0,
                frameRecmapd.width,
                frameRecmapd.height
            };
        }
        else {
            src = {
                0,
                0,
                frameRecmapv.width,
                frameRecmapv.height
            };
        }
    }

    float targetHeight = 64.0f;
    float scale = targetHeight / src.height;
    Rectangle dest = {
        pos.x,
        pos.y,
        src.width * scale,
        src.height * scale
    };

    Vector2 origin = { 0, 0 };

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
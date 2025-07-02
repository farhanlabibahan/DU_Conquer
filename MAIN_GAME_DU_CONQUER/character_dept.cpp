#include "raylib.h"
#include<iostream>
using namespace std;


Texture2D character_deptl,character_deptr,character_dept;
// Vector2 character_dept_pos = { -20, 410 };
Rectangle frameRec;
int currentFrame = 0;
int framesCounter = 0;
int framesSpeed = 11;
bool facingRight = true;
bool moving = false;

void init_character_dept() {
    character_deptl= LoadTexture("resources/sprite_left.png");
     character_deptr = LoadTexture("resources/sprite_right.png");
    
    // character_dept_pos = { -20, 410 };

    frameRec = {
        0.0f, 0.0f,
        (float)character_deptl.width / 5,
        (float)character_deptr.height / 2
    };
}

Vector2 walk_character_dept() {
    moving = false;
    Vector2 offset = {0,0};

    if (IsKeyDown(KEY_A)) {
        facingRight = false;
        // character_dept_pos.x -= 2.0f;
        offset.x = -5;
        moving = true;
    }
    else if (IsKeyDown(KEY_D)) {
        facingRight = true;
        // character_dept_pos.x += 2.0f;
        offset.x = 5;
        moving = true;
    }

    if (moving) {
        framesCounter++;
        if (framesCounter >= (60 / framesSpeed)) {
            framesCounter = 0;
            currentFrame++;
            if (currentFrame > 9) currentFrame = 0;
        }
    } else {
        currentFrame = 0;
    }

    return offset;
}

void draw_char_dept(Vector2 poss,float scale) {
    int frameRow = currentFrame / 5;
    int frameCol = currentFrame % 5;

    Rectangle source = {
        frameCol * frameRec.width,
        frameRow * frameRec.height,
        frameRec.width,
        frameRec.height
    };

    // if (facingRight) {
    //     source.width = -frameRec.width;  // Flip horizontally
    //     source.x += frameRec.width;      // Adjust origin for flipping
    // }

    Rectangle dest = {
        poss.x
        , poss.y,
        frameRec.width * scale*2.5f,
        frameRec.height * scale*2.5f
    };

    Vector2 origin = { 0, 0 };
    character_dept=facingRight?character_deptr:character_deptl;
    cout<<"position in character dept"<<poss.x<<endl;
 cout<<"position in character dept"<<poss.y<<endl;
    DrawTexturePro(character_dept, source, dest, origin, 0.0f, WHITE);
}

void unload_character_dept() {
    UnloadTexture(character_deptl);
    UnloadTexture(character_deptr);
}

#ifndef CHARACTER_H
#define CHARACTER_H

#include "raylib.h"

// Declare functions
void init_character();
void walk_character_dept();
void draw_char_dept();
void unload_character();

// Declare extern globals (only declared here, defined in one .cpp)
extern Texture2D characterl;
extern Texture2D characterr;
extern Vector2 position;
extern bool facingRight;

#endif

#ifndef CHARACTER_DEPT_H
#define CHARACTER_DEPT_H

#include "raylib.h"

// Declare functions
void init_character_dept();
Vector2 walk_character_dept();
void draw_char_dept(float possx,float possy,float scale);
void unload_character_dept();

extern bool moving;


#endif

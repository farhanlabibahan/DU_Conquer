#ifndef CHARACTER_MAP_H
#define CHARACTER_MAP_H

#include "raylib.h"

// Declare functions
void init_character_map();
Vector2 walk_character_map();
void draw_char_map(Vector2 poss);
void unload_character_map();

extern bool movingmap;


#endif

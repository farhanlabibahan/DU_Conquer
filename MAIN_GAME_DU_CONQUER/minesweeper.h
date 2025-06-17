// minesweeper.h
#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include "raylib.h"


// Core functions
void init_minesweeper();                 // Initialize game, assets, and window
void logic_minesweeper();  // Update game logic based on mouse
void draw_minesweeper();   // Render the game
void unload_minesweeper();              // Unload all textures and close window

#endif

#ifndef LIFT_H
#define LIFT_H

#include "raylib.h"

class liftButton {
public:
    int floor_num;
    bool isPressed = false;
    Rectangle buttonRect;

    // Fix: declare constructor with float x and y
    liftButton(int num, float x, float y);

    void DrawButton();
    bool isLiftButtonPressed();
};


// Public lift functions
void init_lift();
void unload_lift();
void logic_draw_lift();

#endif

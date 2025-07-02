#include "lift.h"

Texture2D bg;
int offsetX = 150, offsetY = 50;

class liftButton{
    public:
    int floor_num;
    bool isPressed = false;
    Rectangle buttonRect;

    liftButton(int num,int x,int y) : floor_num(num) {
        buttonRect = { (float)x, (float)y, 30, 30 };
    }

    void DrawButton()
    {
        Color btn_color;
        if(isPressed) btn_color = RED;
        else btn_color = WHITE;

        DrawText(TextFormat("%d",floor_num),buttonRect.x,buttonRect.y,15,btn_color);
        
    }

    bool isLiftButtonPressed()
    {
        Vector2 mouse_poss = GetMousePosition();

        if (CheckCollisionPointRec(mouse_poss, buttonRect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
        {
            isPressed = true;
            return true;
        }

        return false;
    }
};

void init_lift()
{
    bg = LoadTexture("resources/lift.png");
}

void unload_lift()
{
    UnloadTexture(bg);
}

void logic_draw_lift()
{
    DrawRectangle(offsetX*scale,offsetY*scale,100,300,RED);
    liftButton first_floor(1,offsetX,offsetY);
    first_floor.isLiftButtonPressed();
    first_floor.DrawButton();
}
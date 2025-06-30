#include "raylib.h"
#include <iostream>
#include "menu.h"
#include "map.h"
using namespace std;

int main()
{
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "DU Conquer");
    ToggleFullscreen();
    int screenHeight = GetMonitorHeight(0);
    int screenWidth = GetMonitorWidth(0);

    while (!isGameClosed) 
    {
        BeginDrawing();
        ClearBackground(BLACK);

        init_menu();
        logic_draw_menu();
        init_map();
        logic_draw_map();

        DrawText("DU Conquer", screenWidth/2, screenHeight/2, 50, MAGENTA);
        EndDrawing();
    }

    CloseWindow();
}
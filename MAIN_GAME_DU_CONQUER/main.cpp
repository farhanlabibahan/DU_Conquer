#include "raylib.h"
#include "global.h"
#include "menu.h"
#include "map.h"
#include "eee.h"
#include "loading.h"
#include <iostream>

using namespace std;

int main() {
    

    InitWindow(GetScreenWidth(), GetScreenHeight(), "DU_Conquer");
    InitAudioDevice();
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    ToggleFullscreen();
    SetTargetFPS(144);

    layer prev_state_of_game = state_of_game;

    init_map();
    init_menu();
    init_eee();

    // cout<<"Initial state: "<<state_of_game<<endl;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("DU_CONQUER", screenWidth / 2, screenHeight / 2, 50, MAGENTA);

        // transition
        if(state_of_game!=prev_state_of_game)
        {
            time_loading_start = GetTime();
            prev_state_of_game = state_of_game;
        }

        if(state_of_game==LAYER_MENU) logic_draw_menu();
        else if(state_of_game==LAYER_MAP) logic_draw_map();
        else if(state_of_game==LAYER_EEE) logic_draw_eee();
        else if(state_of_game==LAYER_EXIT) break;
        
        

        cout<<"Game State: "<<state_of_game<<endl;
        EndDrawing();
    }

    unload_map();
    unload_menu();
    unload_eee();
    CloseAudioDevice();
    CloseWindow();

    
    return 0;
}
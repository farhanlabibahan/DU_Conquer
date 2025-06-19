#include "raylib.h"
#include "global.h"
#include "menu.h"
#include "map.h"
#include "loading.h"
#include "genetics.h"
#include <iostream>

int main() {
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "DU_Conquer");
    ToggleFullscreen();
    InitAudioDevice();

    init_assets();

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    SetTargetFPS(144);

    layer prev_state_of_game = state_of_game;

    init_map();
    init_menu();
    init_genetics();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (state_of_game != prev_state_of_game) {
            time_loading_start = GetTime();
            prev_state_of_game = state_of_game;
        }

        if (state_of_game == LAYER_MENU) logic_draw_genetics();

        EndDrawing();
    }

    unload_map();
    unload_menu();
    unload_assets();
    unload_genetics();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

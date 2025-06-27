#include "raylib.h"
#include "global.h"
#include "menu.h"
#include "map.h"
#include "eee.h"
#include "nuclear.h"
#include "loading.h"
#include "genetics.h"
#include "japanesse.h"
#include "microbiology.h"
#include "eee_first.h"
#include <iostream>


int main() {
    InitWindow(GetMonitorWidth(0), GetMonitorHeight(0), "DU_Conquer");
    ToggleFullscreen();
    InitAudioDevice();

    // Initialize global assets
    init_assets();

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    SetTargetFPS(144);

    layer prev_state_of_game = state_of_game;

    init_map();
    init_menu();
    init_eee();
    init_genetics();
    init_nuclear();
    init_microbiology();
    init_japanesse();
    init_eee_first();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("DU_CONQUER", screenWidth / 2, screenHeight / 2, 50, MAGENTA);

        if (state_of_game != prev_state_of_game) {
            time_loading_start = GetTime();
            prev_state_of_game = state_of_game;
        }

        if (state_of_game == LAYER_MENU) logic_draw_menu();
        else if (state_of_game == LAYER_MAP) logic_draw_map();
        else if (state_of_game == LAYER_EEE) logic_draw_eee();
        else if (state_of_game == LAYER_NUCLEAR) logic_draw_nuclear();
        else if (state_of_game == LAYER_GENETICS) logic_draw_genetics();
        else if (state_of_game == LAYER_MICROBIOLOGY) logic_draw_microbiology();
        else if (state_of_game == LAYER_JAPANESSE) logic_draw_japanesse();
        else if (state_of_game == LAYER_EEE_FIRST) logic_draw_eee_first();
        else if (state_of_game == LAYER_EXIT) break;

        DrawFPS(10,20);
        EndDrawing();
    }

    unload_map();
    unload_menu();
    unload_eee();
    unload_genetics();
    unload_nuclear();
    unload_microbiology();
    unload_assets();
    unload_japanesse();
    unload_eee_first();

    CloseAudioDevice();
    CloseWindow();

    return 0;
}

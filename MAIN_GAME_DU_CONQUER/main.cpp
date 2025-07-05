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
#include "character_dept.h"
#include "character_map.h"
#include "vc_bunglaw.h"
#include "lift.h"
#include "cctv.h"
#include "maze.h"
#include "iba.h"
#include "bank.h"
#include "business.h"
#include "ducsu.h"
#include "law.h"
#include "robotics.h"
#include "cse.h"
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
    init_eee();
    init_genetics();
    init_nuclear();
    init_microbiology();
    init_japanesse();
    init_eee_first();
    init_character_dept();
    init_vc_bunglaw();
    init_lift();
    init_character_map();
    init_cctv();
    init_maze();
    init_bank();
    init_ducsu();
    init_iba();
    init_law();
    init_robotics();
    init_cse();

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
        else if (state_of_game == LAYER_VC_BUNGLAW) logic_draw_vc_bunglaw();
        else if (state_of_game == LAYER_LIFT) logic_draw_lift();
        else if (state_of_game == LAYER_CCTV)logic_draw_cctv();
        else if (state_of_game == LAYER_MAZE)logic_draw_maze();
        else if (state_of_game == LAYER_BANK) logic_draw_bank();
        else if (state_of_game == LAYER_IBA) logic_draw_iba();
        else if (state_of_game == LAYER_DUCSU)logic_draw_ducsu();
        else if (state_of_game == LAYER_LAW)logic_draw_law();
        else if (state_of_game == LAYER_ROBOTICS) logic_draw_robotics();
        else if (state_of_game == LAYER_CSE) logic_draw_cse();
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
    unload_character_dept();
    unload_vc_bunglaw();
    unload_lift();
    unload_character_map();
    unload_cctv();
    unload_maze();
    unload_bank();
    unload_iba();
    unload_ducsu();
    unload_law();
    unload_robotics();
    unload_cse();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

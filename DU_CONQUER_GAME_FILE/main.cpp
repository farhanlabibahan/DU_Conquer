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
#include "philosophy.h"
#include "tictactoe.h"
#include "intro.h"
#include "outro.h"
#include <iostream>


int main() {
    InitWindow(GetScreenWidth(), GetScreenHeight(), "DU_Conquer");
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
    init_philosophy();
    init_tictactoe();

    IntroPlayer::InitIntroPlayer();
    OutroPlayer::InitOutroPlayer();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        if (state_of_game != prev_state_of_game) {
            time_loading_start = GetTime();
            prev_state_of_game = state_of_game;
        }


        

        
        else if (state_of_game == LAYER_INTRO) {
            if (IntroPlayer::UpdateAndDrawIntro() || IsKeyPressed(KEY_S)) {
                IntroPlayer::UnloadIntroPlayer();
                state_of_game = LAYER_MAP;
            }
        }
        else if (state_of_game == LAYER_OUTRO) {    
            if (OutroPlayer::UpdateAndDrawOutro() || IsKeyPressed(KEY_S)) {
                OutroPlayer::UnloadOutroPlayer();
                state_of_game = LAYER_EXIT;
            }
        }
        else if (state_of_game == LAYER_MENU) logic_draw_menu();
        else if (state_of_game == LAYER_MAP) 
        {
                    if (eee_game_1 && eee_game_2 && nuclear_game && ducsu_game && genetics_game &&
            microbiology_game && japanesse_game && shooting_game && lift_game &&
            cctv_game && maze_game && bank_game && iba_game && law_game &&
            gear_game && robotics_game && cse_game && philosophy_game) 
            {
                state_of_game = LAYER_OUTRO;
            }
            logic_draw_map();
        }
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
        else if (state_of_game == LAYER_PHILOSOPHY)logic_draw_philosophy();
        else if (state_of_game == LAYER_EXIT) break;
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
    unload_philosophy();
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

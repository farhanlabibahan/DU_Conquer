#ifndef GLOBAL_H
#define GLOBAL_H

#include "raylib.h"
#include "character_dept.h"

extern double x_co_ordinate, y_co_ordinate;
extern double scale;
extern int screenWidth;
extern int screenHeight;

typedef enum {
    LAYER_MENU,
    LAYER_MAP,
    LAYER_EEE_FIRST,
    LAYER_EXIT,
    LAYER_EEE,
    LAYER_NUCLEAR,
    LAYER_GENETICS,
    LAYER_MICROBIOLOGY,
    LAYER_JAPANESSE,
    LAYER_ICTCELL,
    LAYER_VC_BUNGLAW,
    LAYER_LIFT
} layer;

extern layer state_of_game;

// dept wise game list
extern bool eee_game_2;
extern bool eee_game_1;
extern bool nuclear_game;
extern bool ducsu_game;
extern bool genetics_game;
extern bool microbiology_game;
extern bool japanesse_game;
extern bool shooting_game;
extern bool lift_game;

//loading
extern double time_loading_start;

// Game assets
extern Sound conquered_sound, pop_up_sound, click_sound, error_sound;
extern Music bgm_eee, walk_music,bgm_nuclear,bgm_genetics,bgm_microbiology,bgm_japanesse,bgm_eee_first,bgm_vc_bunglaw;
extern Texture2D character;

// Asset management
void init_assets();
void unload_assets();

#endif

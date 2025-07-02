#include "global.h"
#include "character_dept.h"
#include "raylib.h"
// #include "raylib.h"

// Global variable definitions
double x_co_ordinate = 4523, y_co_ordinate = 2873;
double scale = 0;
int screenWidth = GetScreenWidth();
int screenHeight = GetScreenHeight();
layer state_of_game = LAYER_MENU;

//dept wise games
bool eee_game_2 = false;
bool eee_game_1 = false;
bool nuclear_game = false;
bool ducsu_game = false;
bool genetics_game = false;
bool microbiology_game = false;
bool japanesse_game = false;
bool shooting_game = false;
bool lift_game = false;


//loading
double time_loading_start = 0.0;

// Game asset definitions
Sound conquered_sound;
Sound pop_up_sound;
Sound click_sound;
Sound error_sound;
Music walk_music;
Texture2D character;

// dept wise bgm
Music bgm_eee,bgm_nuclear,bgm_microbiology,bgm_genetics,bgm_japanesse,bgm_eee_first,bgm_vc_bunglaw,bgm_lift;

// Load all game assets
void init_assets() {
    conquered_sound = LoadSound("resources/conquered.mp3");
    pop_up_sound = LoadSound("resources/pop_up.mp3");
    click_sound = LoadSound("resources/click.wav");
    error_sound = LoadSound("resources/error.mp3");

    bgm_eee = LoadMusicStream("resources/bgm_eee.mp3");
    bgm_nuclear = bgm_eee;
    bgm_microbiology = bgm_eee;
    bgm_genetics = bgm_eee;
    bgm_japanesse = LoadMusicStream("resources/japanese_bgm.mp3");
    bgm_eee_first = bgm_eee;
    walk_music = LoadMusicStream("resources/walk.mp3");
    bgm_vc_bunglaw = bgm_eee;
    bgm_lift = bgm_eee;

    character = LoadTexture("resources/character.png");
}

// Unload all game assets
void unload_assets() {
    UnloadSound(conquered_sound);
    UnloadSound(pop_up_sound);
    UnloadSound(click_sound);
    UnloadSound(error_sound);

    UnloadMusicStream(bgm_eee);
    UnloadMusicStream(walk_music);

    UnloadTexture(character);
}



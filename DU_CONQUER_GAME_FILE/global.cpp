#include "global.h"
#include "character_dept.h"
#include "raylib.h"

// Global variable definitions
double x_co_ordinate = 1991.8, y_co_ordinate = 1904.8;
double scale = 0;
int screenWidth = GetScreenWidth();
int screenHeight = GetScreenHeight();
layer state_of_game = LAYER_MENU;

//dept wise games
bool eee_game_2 = 0;
bool eee_game_1 = 0;
bool nuclear_game = false;
bool ducsu_game = 0;
bool genetics_game = 0;
bool microbiology_game = 0;
bool japanesse_game = 0;
bool shooting_game = 0;
bool lift_game = 0;
bool cctv_game =0;
bool maze_game = 0;
bool bank_game = 0;
bool iba_game =0;
bool law_game = 0;
bool gear_game = 0;
bool robotics_game = 0;
bool cse_game = 0;
bool philosophy_game=0;

//sabotage things
bool lift_sabotage = false;


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
Music bgm_eee,bgm_nuclear,bgm_microbiology,bgm_genetics,bgm_japanesse,bgm_eee_first,bgm_vc_bunglaw,bgm_lift,bgm_cctv,bgm_bank,bgm_iba,bgm_ducsu,bgm_cse,bgm_robotics,bgm_law,bgm_philosophy;

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
    bgm_cctv = bgm_eee;
    bgm_bank = bgm_eee;
    bgm_iba =bgm_eee;
    bgm_ducsu = bgm_eee;
    bgm_lift = bgm_eee;
    bgm_law = bgm_eee;
    bgm_robotics = bgm_eee;
    bgm_cse = bgm_eee;
    bgm_philosophy=bgm_eee;
    character = LoadTexture("resources/character.png");
}

void unload_assets() {
    UnloadSound(conquered_sound);
    UnloadSound(pop_up_sound);
    UnloadSound(click_sound);
    UnloadSound(error_sound);
    UnloadMusicStream(bgm_eee);
    UnloadMusicStream(walk_music);
    UnloadTexture(character);
}



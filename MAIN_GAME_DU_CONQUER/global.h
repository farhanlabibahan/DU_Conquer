#ifndef GLOBAL_H
#define GLOBAL_H

extern double x_co_ordinate,y_co_ordinate;
extern double scale;
typedef enum
{
    LAYER_MENU,
    LAYER_MAP,
    LAYER_EXIT,
    LAYER_EEE
} layer;
extern layer state_of_game;
extern int screenWidth;
extern int screenHeight;

// dept wise games
extern bool eee_game_2;

// loading
extern double time_loading_start;

#endif // GLOBAL_H

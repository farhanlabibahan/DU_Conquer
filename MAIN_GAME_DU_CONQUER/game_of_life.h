#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include "raylib.h"


class GridSimulation {
public:
    void init_cells();
    void draw_cells();
    int count_neighbors(int i, int j);
    void simulate();
};

void init_game_of_life();
void logic_game_of_life(GridSimulation &grids);
void draw_game_of_life(GridSimulation &grids);
void unload_game_of_life();

#endif
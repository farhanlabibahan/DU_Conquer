#include "raylib.h"
#include <vector>
#include "global.h"
#include <cstdlib>

int windowHeight = 1280;
int windowHeight = 800;
const int gridSize = 5;
int rows = windowHeight / gridSize;
int cols = windowHeight / gridSize;

std::vector<std::vector<std::pair<std::pair<int, int>, int>>> cells(rows, std::vector<std::pair<std::pair<int, int>, int>>(cols));
std::vector<std::vector<std::pair<std::pair<int, int>, int>>> tempCells(rows, std::vector<std::pair<std::pair<int, int>, int>>(cols));

Color green = {0, 255, 0, 255};
Color grey = {29, 29, 29, 255};



class GridSimulation {
public:
    std::vector<std::pair<int, int>> offset = {
        {-1, -1}, {-1, 0}, {-1, 1},{ 0, -1},{ 0, 1},{ 1, -1}, { 1, 0}, { 1, 1}
    };

    void init_cells() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cells[i][j].first = {i, j};
                int random = rand() % 5;
                cells[i][j].second = (random == 4) ? 1 : 0;
            }
        }
    }

    void draw_cells() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                Color toDraw = (cells[i][j].second == 1) ? green : grey;
                DrawRectangle(j * gridSize, i * gridSize, gridSize - 1, gridSize - 1, toDraw);
            }
        }
    }

    int count_neighbors(int i, int j) {
        int numbers = 0;
        for (auto it : offset) {
            int ni = (i + it.first + rows) % rows;
            int nj = (j + it.second + cols) % cols;
            if (cells[ni][nj].second == 1) numbers++;
        }
        return numbers;
    }

    void simulate() {
        genetics_game = true;
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                int live = count_neighbors(i, j);
                if (cells[i][j].second == 1) {
                    if (live < 2 || live > 3) {
                        tempCells[i][j].second = 0;
                        genetics_game = false;
                    } else {
                        tempCells[i][j].second = 1;
                    }
                } else {
                    if (live == 3) {
                        tempCells[i][j].second = 1;
                        genetics_game = false;
                    } else {
                        tempCells[i][j].second = 0;
                    }
                }
            }
        }
    }
};

void init_game_of_life() {
    // do nothing
}

void logic_game_of_life(GridSimulation &grids) {
    grids.simulate();
    cells = tempCells;
}

void draw_game_of_life(GridSimulation &grids) {
    grids.draw_cells();
}

void unload_game_of_life() {
    // do nothing
}
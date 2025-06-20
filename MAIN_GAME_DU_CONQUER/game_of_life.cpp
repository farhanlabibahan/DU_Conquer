//---------------- game_of_life.h ----------------
#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include "raylib.h"
#include "global.h"

class GridSimulation {
public:
    void init_cells();
    void draw_cells(int offsetX = 0, int offsetY = 0);
    int count_neighbors(int i, int j);
    void simulate();
    int get_plus_count();
    int get_circle_count();
};

void init_game_of_life();
void logic_game_of_life(GridSimulation &grids);
void draw_game_of_life(GridSimulation &grids);
void unload_game_of_life();

#endif


//---------------- game_of_life.cpp ----------------
#include "game_of_life.h"
#include "global.h"
#include <vector>
#include <cstdlib>

const int gridSize = 5;
int windowWidth = 900;
int windowHeight = 700;
int rows = windowHeight / gridSize;
int cols = windowWidth / gridSize;

std::vector<std::vector<std::pair<std::pair<int, int>, int>>> cells(rows, std::vector<std::pair<std::pair<int, int>, int>>(cols));
std::vector<std::vector<std::pair<std::pair<int, int>, int>>> tempCells(rows, std::vector<std::pair<std::pair<int, int>, int>>(cols));

Color green = {0, 255, 0, 255};
Color grey = {29, 29, 29, 255};

float sim_timer = 0.0f;
bool ask_question = false;
bool question_answered = false;
bool question_popup = false;
const float SIM_DURATION = 120.0f; // 2 minutes

void GridSimulation::init_cells() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cells[i][j].first = {i, j};
            int random = rand() % 5;
            cells[i][j].second = (random == 4) ? 1 : 0;
        }
    }
}

void GridSimulation::draw_cells(int offsetX, int offsetY) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Color toDraw = (cells[i][j].second == 1) ? green : grey;
            DrawRectangle(offsetX + j * gridSize, offsetY + i * gridSize, gridSize - 1, gridSize - 1, toDraw);
        }
    }
}

int GridSimulation::count_neighbors(int i, int j) {
    int numbers = 0;
    std::vector<std::pair<int, int>> offset = {
        {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}
    };
    for (auto it : offset) {
        int ni = (i + it.first + rows) % rows;
        int nj = (j + it.second + cols) % cols;
        if (cells[ni][nj].second == 1) numbers++;
    }
    return numbers;
}

int GridSimulation::get_plus_count() {
    int count = 0;
    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            if (cells[i][j].second == 1 &&
                cells[i-1][j].second == 1 &&
                cells[i+1][j].second == 1 &&
                cells[i][j-1].second == 1 &&
                cells[i][j+1].second == 1) {
                count++;
            }
        }
    }
    return count;
}

int GridSimulation::get_circle_count() {
    int count = 0;
    for (int i = 1; i < rows - 1; i++) {
        for (int j = 1; j < cols - 1; j++) {
            if (cells[i-1][j].second == 1 &&
                cells[i+1][j].second == 1 &&
                cells[i][j-1].second == 1 &&
                cells[i][j+1].second == 1 &&
                cells[i-1][j-1].second == 1 &&
                cells[i-1][j+1].second == 1 &&
                cells[i+1][j-1].second == 1 &&
                cells[i+1][j+1].second == 1 &&
                cells[i][j].second == 0) {
                count++;
            }
        }
    }
    return count;
}

void GridSimulation::simulate() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int live = count_neighbors(i, j);
            if (cells[i][j].second == 1) {
                tempCells[i][j].second = (live == 2 || live == 3) ? 1 : 0;
            } else {
                tempCells[i][j].second = (live == 3) ? 1 : 0;
            }
        }
    }
    cells = tempCells;
}

void init_game_of_life() {
    sim_timer = 0.0f;
    ask_question = false;
    question_answered = false;
    question_popup = false;
}

void logic_game_of_life(GridSimulation &grids) {
    if (!ask_question) {
        sim_timer += GetFrameTime();
        if (sim_timer >= SIM_DURATION) {
            ask_question = true;
            question_popup = true;
        } else {
            grids.simulate();
        }
    } else if (!question_answered && IsKeyPressed(KEY_A) || IsKeyPressed(KEY_B)) {
        genetics_game = true;
        question_answered = true;
        question_popup = false;
    }
}

void draw_game_of_life(GridSimulation &grids) {
    grids.draw_cells(screenWidth / 2 - (cols * gridSize / 2), screenHeight / 2 - (rows * gridSize / 2));

    if (question_popup) {
        DrawRectangle(screenWidth / 2 - 200, screenHeight / 2 - 100, 400, 200, Fade(BLACK, 0.8f));
        DrawRectangleLines(screenWidth / 2 - 200, screenHeight / 2 - 100, 400, 200, WHITE);

        // DrawText("GENETICS QUESTION:", screenWidth / 2 - 180, screenHeight / 2 - 80, 20, RAYWHITE);
        DrawText("After observing the bacterial colony \nit seems one mutation is more prevalent.\nWhich structure represents the dominant mutation?", screenWidth / 2 - 180, screenHeight / 2 - 80, 15, RAYWHITE);
        DrawText("A. Plus-shaped clusters", screenWidth / 2 - 180, screenHeight / 2, 17, GREEN);
        DrawText("B. Circle-shaped clusters", screenWidth / 2 - 180, screenHeight / 2 + 30, 17, GREEN);
        DrawText("Press A or B", screenWidth / 2 - 180, screenHeight / 2 + 70, 20, YELLOW);
    }
}

void unload_game_of_life() {
    // Cleanup if needed
}

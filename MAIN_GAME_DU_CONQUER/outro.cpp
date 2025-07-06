// outro.cpp
#include "raylib.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

static std::vector<Texture2D> outroFrames;
static const int totalOutroFrames = 100;
static int currentOutroFrame = 0;
static float outroFrameTime = 1.0f / 60.0f;  // 60 FPS
static float outroTimer = 0.0f;

static bool outroLoaded = false;
static bool outroFinished = false;

void LoadOutroFrames() {
    for (int i = 1; i <= totalOutroFrames; i++) {
        std::ostringstream filename;
        filename << "frames/outro/frame_" << std::setfill('0') << std::setw(4) << i << ".png";

        Image img = LoadImage(filename.str().c_str());
        outroFrames.push_back(LoadTextureFromImage(img));
        UnloadImage(img);
    }
    outroLoaded = true;
}

void UnloadOutroFrames() {
    for (auto& f : outroFrames) {
        UnloadTexture(f);
    }
    outroFrames.clear();
    outroLoaded = false;
}

bool logic_draw_outro() {
    if (!outroLoaded) LoadOutroFrames();

    if (!outroFinished) {
        outroTimer += GetFrameTime();
        if (outroTimer >= outroFrameTime) {
            currentOutroFrame++;
            outroTimer = 0.0f;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        if (currentOutroFrame < totalOutroFrames) {
            DrawTexture(outroFrames[currentOutroFrame], 0, 0, WHITE);
        }
        EndDrawing();

        if (currentOutroFrame >= totalOutroFrames) {
            outroFinished = true;
            UnloadOutroFrames();
        }
    }

    return outroFinished;
}

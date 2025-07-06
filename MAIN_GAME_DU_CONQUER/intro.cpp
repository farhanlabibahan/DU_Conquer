// intro.cpp
#include "raylib.h"
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

static std::vector<Texture2D> introFrames;
static const int totalIntroFrames = 150;
static int currentIntroFrame = 0;
static float introFrameTime = 1.0f / 60.0f;  // 60 FPS
static float introTimer = 0.0f;

static bool introLoaded = false;
static bool introFinished = false;

void LoadIntroFrames() {
    for (int i = 1; i <= totalIntroFrames; i++) {
        std::ostringstream filename;
        filename << "frames/intro/frame_" << std::setfill('0') << std::setw(4) << i << ".png";

        Image img = LoadImage(filename.str().c_str());
        introFrames.push_back(LoadTextureFromImage(img));
        UnloadImage(img);
    }
    introLoaded = true;
}

void UnloadIntroFrames() {
    for (auto& f : introFrames) {
        UnloadTexture(f);
    }
    introFrames.clear();
    introLoaded = false;
}

bool logic_draw_intro() {
    if (!introLoaded) LoadIntroFrames();

    if (!introFinished) {
        introTimer += GetFrameTime();
        if (introTimer >= introFrameTime) {
            currentIntroFrame++;
            introTimer = 0.0f;
        }

        BeginDrawing();
        ClearBackground(BLACK);
        if (currentIntroFrame < totalIntroFrames) {
            DrawTexture(introFrames[currentIntroFrame], 0, 0, WHITE);
        }
        EndDrawing();

        if (currentIntroFrame >= totalIntroFrames) {
            introFinished = true;
            UnloadIntroFrames();
        }
    }

    return introFinished;
}

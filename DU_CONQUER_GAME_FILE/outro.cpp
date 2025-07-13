#include "raylib.h"
#include <string>
#include <sstream>
#include <iostream>
#include "outro.h"
#include "global.h"
#include <iomanip>

namespace OutroPlayer {

static const int totalOutroFrames = 511;           // total frames: 0 to 873
static const float outroDurationSeconds = 47.0f;   // total intro duration in seconds
static const int targetFPS = 60;                    // target FPS of your game

// Calculate how many game frames to show each video frame
static const float framesPerVideoFrame = (outroDurationSeconds * targetFPS) / totalOutroFrames;

static int currentOutroFrame = 0;
static float frameCounter = 0.0f;
static bool outroFinished = false;

static Texture2D currentFrameTexture = {0};
static bool frameLoaded = false;

// Paths - update these if needed
static const std::string outroFrameDir = "resources/outroframes/";   // your frames folder
static const std::string outroAudioPath = "resources/outro_audio.ogg";

static Music outroMusic = {0};

void LoadFrame(int frameIndex) {
    if (frameLoaded) UnloadTexture(currentFrameTexture);

    // Frame files named: frame00001.png, frame00003.png, etc.
    int actualFrameNumber = 1 + frameIndex * 2;
    std::ostringstream filename;
    filename << outroFrameDir << "frame" << std::setfill('0') << std::setw(5) << actualFrameNumber << ".png";

    Image img = LoadImage(filename.str().c_str());
    if (img.data != nullptr) {
        currentFrameTexture = LoadTextureFromImage(img);
        UnloadImage(img);
        frameLoaded = true;
    } else {
        frameLoaded = false;
        TraceLog(LOG_WARNING, "Failed to load frame: %s", filename.str().c_str());
    }
}

bool logic_draw_outro() {
    if (outroFinished) return true;

    std::cout<<"OUTRO\n";
    float musicTime = GetMusicTimePlayed(outroMusic);
    int frameToShow = (int)((musicTime / outroDurationSeconds) * totalOutroFrames);
    if (frameToShow >= totalOutroFrames) frameToShow = totalOutroFrames - 1;

    if (frameToShow != currentOutroFrame) {
        currentOutroFrame = frameToShow;
        LoadFrame(currentOutroFrame);
    }

    UpdateMusicStream(outroMusic);

    if (frameLoaded) {
        // DrawTexture(currentFrameTexture, 0, 0, WHITE);
        DrawTextureEx(currentFrameTexture,(Vector2){0,0},0.0f,(float)screenWidth/currentFrameTexture.width,WHITE);
    } else {
        DrawText("Loading outro...", 40, 40, 30, RAYWHITE);
    }

    if (musicTime >= outroDurationSeconds) {
       if (musicTime >= outroDurationSeconds) {
    outroFinished = true;
    return true;
}
    }

    return false;
}

void InitOutroPlayer() {
   
    outroMusic = LoadMusicStream(outroAudioPath.c_str());
    PlayMusicStream(outroMusic);
    LoadFrame(0);  // load first frame
}

void UnloadOutroPlayer() {
    StopMusicStream(outroMusic);
    UnloadMusicStream(outroMusic);
    if (frameLoaded) UnloadTexture(currentFrameTexture);
   
}

bool UpdateAndDrawOutro() {
    return logic_draw_outro();
}

} 
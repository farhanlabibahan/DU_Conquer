#include "raylib.h"
#include <string>
#include <sstream>
#include "intro.h"
#include "global.h"
#include <iomanip>

namespace IntroPlayer {

static const int totalIntroFrames = 874;           // total frames: 0 to 873
static const float introDurationSeconds = 77.0f;   // total intro duration in seconds
static const int targetFPS = 59;                    // target FPS of your game

// Calculate how many game frames to show each video frame
static const float framesPerVideoFrame = (introDurationSeconds * targetFPS) / totalIntroFrames;

static int currentIntroFrame = 0;
static float frameCounter = 0.0f;
static bool introFinished = false;

static Texture2D currentFrameTexture = {0};
static bool frameLoaded = false;

// Paths - update these if needed
static const std::string frameDir = "resources/introframes/";   // your frames folder
static const std::string audioPath = "resources/intro_audio.mp3";

static Music introMusic = {0};

void LoadFrame(int frameIndex) {
    if (frameLoaded) UnloadTexture(currentFrameTexture);

    // Frame files named: frame00001.png, frame00003.png, etc.
    int actualFrameNumber = 1 + frameIndex * 2;
    std::ostringstream filename;
    filename << frameDir << "frame" << std::setfill('0') << std::setw(5) << actualFrameNumber << ".png";

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

bool logic_draw_intro() {
    if (introFinished) return true;

    float musicTime = GetMusicTimePlayed(introMusic);
    int frameToShow = (int)((musicTime / introDurationSeconds) * totalIntroFrames);
    if (frameToShow >= totalIntroFrames) frameToShow = totalIntroFrames - 1;

    if (frameToShow != currentIntroFrame) {
        currentIntroFrame = frameToShow;
        LoadFrame(currentIntroFrame);
    }

    UpdateMusicStream(introMusic);

    if (frameLoaded) {
        // DrawTextureEx(currentFrameTexture, 0, 0, WHITE);
        DrawTextureEx(currentFrameTexture,(Vector2){0,0},0.0f,(float)screenWidth/currentFrameTexture.width,WHITE);
    } else {
        DrawText("Loading intro...", 40, 40, 30, RAYWHITE);
    }

    if (musicTime >= introDurationSeconds) {
       if (musicTime >= introDurationSeconds) {
    introFinished = true;
    return true;
}
    }

    return false;
}

void InitIntroPlayer() {
   
    introMusic = LoadMusicStream(audioPath.c_str());
    PlayMusicStream(introMusic);
    LoadFrame(0);  // load first frame
}

void UnloadIntroPlayer() {
    StopMusicStream(introMusic);
    UnloadMusicStream(introMusic);
    if (frameLoaded) UnloadTexture(currentFrameTexture);
   
}

bool UpdateAndDrawIntro() {
    return logic_draw_intro();
}

} 
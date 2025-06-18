#include "raylib.h"
#include <bits/stdc++.h>
#include "menu.h"
using namespace std;

Color red = {255, 0, 0, 255};
Color blackTransparent = {0, 0, 0, 128};
Vector2 cloudPos = {0, 0};

const int MAX_HOVER_SOUNDS = 4;
Sound hoverSounds[MAX_HOVER_SOUNDS];
int currentHoverSound = 0;
bool isGameClosed = false;
bool menuOpen = true;
bool menuUnloaded = false; 

void InitHoverSounds() {
    Sound baseSound = LoadSound("resources/click.mp3");
    for (int i = 0; i < MAX_HOVER_SOUNDS; i++) {
        hoverSounds[i] = baseSound;
    }
}

void PlayHoverSound() {
    PlaySound(hoverSounds[currentHoverSound]);
    currentHoverSound = (currentHoverSound + 1) % MAX_HOVER_SOUNDS;
}

Music bgmMenu;
bool wasHoveredNew = false, wasHoveredHigh = false, wasHoveredHelp = false, wasHoveredMap = false, wasHoveredSaved = false, wasHoveredExit = false;

Texture2D main_menu;
Texture2D cloud;
float scale;
int buttonWidth, buttonHeight, spacing, startY, startX;

bool showMap = false;
bool showHelp = false;
Texture2D mapTexture;
string helpText = "Use arrow keys to move.\nPress SPACE to shoot.\nCollect power-ups and avoid enemies.";

class Button {
public:
    Rectangle bounds;
    const char* label;
    Color textColorNormal;
    Color textColorHover;

    Button() = default;

    Button(float x, float y, float width, float height, const char* text)
        : bounds{ x, y, width, height },
          label(text),
          textColorNormal(WHITE),
          textColorHover(MAGENTA) {}

    bool IsHovered() const {
        return CheckCollisionPointRec(GetMousePosition(), bounds);
    }

    bool IsClicked() const {
        return IsHovered() && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

    void DrawButton(int fontSize, Color bgColor) const {
        DrawRectangleRec(bounds, bgColor);
        Color currentTextColor = IsHovered() ? textColorHover : textColorNormal;
        int textWidth = MeasureText(label, fontSize);
        DrawText(label,
                 bounds.x + (bounds.width - textWidth) / 2,
                 bounds.y + (bounds.height - fontSize) / 2,
                 fontSize,
                 currentTextColor);
    }
};

Button btnNew, btnHigh, btnHelp, btnMap, btnSaved, btnExit;

const char* clickedMessage;
bool high_score_b;

void init_menu() {
    InitAudioDevice();
    InitHoverSounds();
    bgmMenu = LoadMusicStream("resources/bgm_menu.mp3");
    SetMusicVolume(bgmMenu, 0.2f);
    PlayMusicStream(bgmMenu);

    const int screenWidth = GetMonitorWidth(0);
    const int screenHeight = GetMonitorHeight(0);

    main_menu = LoadTexture("resources/main_menu.png");
    cloud = LoadTexture("resources/cloud.png");
    scale = (float)GetMonitorHeight(0) / main_menu.height;

    buttonWidth = 250 * scale;
    buttonHeight = 40 * scale;
    spacing = 40 * scale;
    startY = GetMonitorHeight(0) - 150;
    startX = GetMonitorWidth(0) - 330;

    btnNew = Button(startX, startY - spacing * 6, buttonWidth, buttonHeight, "New Game");
    btnHigh = Button(startX, startY - spacing * 5, buttonWidth, buttonHeight, "High Score");
    btnHelp = Button(startX, startY - spacing * 4, buttonWidth, buttonHeight, "Help");
    btnMap = Button(startX, startY - spacing * 3, buttonWidth, buttonHeight, "Map");
    btnSaved = Button(startX, startY - spacing * 2, buttonWidth, buttonHeight, "Saved Games");
    btnExit = Button(startX, startY - spacing * 1, buttonWidth, buttonHeight, "Exit");

    clickedMessage = nullptr;
    high_score_b = false;

    mapTexture = LoadTexture("resources/map.png");

    SetTargetFPS(60);
}

void unload_menu() {
    UnloadTexture(main_menu);
    UnloadTexture(mapTexture);
    for (int i = 0; i < MAX_HOVER_SOUNDS; i++) {
        UnloadSound(hoverSounds[i]);
    }
    UnloadMusicStream(bgmMenu);
    CloseAudioDevice();
    CloseWindow();
}

void logic_draw_menu() {
    const int screenWidth = GetMonitorWidth(0);
    const int screenHeight = GetMonitorHeight(0);

    while (menuOpen) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            menuOpen = false;
            break;
        }

        if (clickedMessage && strcmp(clickedMessage, "New Game clicked!") == 0) break;

        // Same drawing and logic code as before...
        // (Omitted here for brevity, but keep your cloud animation, map/help popup, buttons, sounds, etc.)

        UpdateMusicStream(bgmMenu);
        EndDrawing();
    }

    // 🔒 Unload only once
    if (!menuUnloaded) {
        unload_menu();
        menuUnloaded = true;
    }
}

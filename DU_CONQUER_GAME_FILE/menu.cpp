#include "raylib.h"
#include "global.h"
#include <bits/stdc++.h>
using namespace std;

Color red = {255, 0, 0, 255};
Color blackTransparent = {0, 0, 0, 128};
Vector2 cloudPos = {0, 0};

Sound hoverSound;

void InitHoverSounds() {
    hoverSound = LoadSound("resources/click.wav");
}

void PlayHoverSound() {
    PlaySound(hoverSound);
}

Music bgmMenu;
bool wasHoveredNew = false, wasHoveredHigh = false, wasHoveredHelp = false;
bool wasHoveredMap = false, wasHoveredAuthor = false, wasHoveredExit = false;

Texture2D main_menu;
Texture2D cloud;
Texture2D mapTexture;
Texture2D authorPics[4];

int buttonWidth, buttonHeight, spacing, startY, startX;

bool showMap = false;
bool showHelp = false;
bool showAuthor = false;

string helpText = "Use arrow keys to move.\nPress SPACE to shoot.\nCollect power-ups and avoid enemies.";

class Button {
public:
    Rectangle bounds;
    const char *label;
    Color textColorNormal;
    Color textColorHover;

    Button() = default;

    Button(float x, float y, float width, float height, const char *text)
        : bounds{x, y, width, height}, label(text),
          textColorNormal(WHITE), textColorHover(MAGENTA) {}

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

void DrawAuthorAvatar(Texture2D texture, Vector2 position, float size) {
    DrawTexturePro(
        texture,
        (Rectangle){ 0, 0, (float)texture.width, (float)texture.height },
        (Rectangle){ position.x, position.y, size, size },
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );
}

Button btnNew, btnHigh, btnHelp, btnMap, btnAuthor, btnExit;

const char *clickedMessage;
bool high_score_b;

void init_menu() {
    InitHoverSounds();
    bgmMenu = LoadMusicStream("resources/bgm_menu.mp3");
    SetMusicVolume(bgmMenu, 0.2f);
    PlayMusicStream(bgmMenu);

    main_menu = LoadTexture("resources/main_menu.png");
    cloud = LoadTexture("resources/cloud.png");
    mapTexture = LoadTexture("resources/map.png");

    authorPics[0] = LoadTexture("resources/ahan.png");
    authorPics[1] = LoadTexture("resources/salwa.png");
    authorPics[2] = LoadTexture("resources/raima.png");
    authorPics[3] = LoadTexture("resources/raisa.png");

    scale = (double)screenHeight / main_menu.height;

    buttonWidth = 250 * scale;
    buttonHeight = 40 * scale;
    spacing = 40 * scale;
    startY = screenHeight - 150;
    startX = screenWidth - 330;

    btnNew    = Button(startX, startY - spacing * 6, buttonWidth, buttonHeight, "New Game");
    btnHigh   = Button(startX, startY - spacing * 5, buttonWidth, buttonHeight, "High Score");
    btnHelp   = Button(startX, startY - spacing * 4, buttonWidth, buttonHeight, "Help");
    btnMap    = Button(startX, startY - spacing * 3, buttonWidth, buttonHeight, "Map");
    btnAuthor = Button(startX, startY - spacing * 2, buttonWidth, buttonHeight, "Author");
    btnExit   = Button(startX, startY - spacing * 1, buttonWidth, buttonHeight, "Exit");

    clickedMessage = nullptr;
    high_score_b = false;

    SetTargetFPS(60);
}

void logic_draw_menu() {
    scale = (double)screenHeight / main_menu.height;

    if (clickedMessage && strcmp(clickedMessage, "New Game clicked!") == 0) {
        state_of_game = LAYER_INTRO;
        clickedMessage = nullptr;
    }

    if (btnNew.IsHovered() && !wasHoveredNew) PlayHoverSound();
    wasHoveredNew = btnNew.IsHovered();
    if (btnNew.IsClicked()) {
        PlaySound(hoverSound);
        clickedMessage = "New Game clicked!";
    }

    if (btnHigh.IsHovered() && !wasHoveredHigh) PlayHoverSound();
    wasHoveredHigh = btnHigh.IsHovered();
    if (btnHigh.IsClicked()) {
        PlaySound(hoverSound);
        clickedMessage = "High Scores";
        high_score_b = true;
    }

    if (btnHelp.IsHovered() && !wasHoveredHelp) PlayHoverSound();
    wasHoveredHelp = btnHelp.IsHovered();
    if (btnHelp.IsClicked()) {
        PlaySound(hoverSound);
        showHelp = true;
    }

    if (btnMap.IsHovered() && !wasHoveredMap) PlayHoverSound();
    wasHoveredMap = btnMap.IsHovered();
    if (btnMap.IsClicked()) {
        PlaySound(hoverSound);
        showMap = true;
    }

    if (btnAuthor.IsHovered() && !wasHoveredAuthor) PlayHoverSound();
    wasHoveredAuthor = btnAuthor.IsHovered();
    if (btnAuthor.IsClicked()) {
        PlaySound(hoverSound);
        showAuthor = true;
    }

    if (btnExit.IsHovered() && !wasHoveredExit) PlayHoverSound();
    wasHoveredExit = btnExit.IsHovered();
    if (btnExit.IsClicked()) {
        PlaySound(hoverSound);
        state_of_game = LAYER_EXIT;
    }

    static float cloudSpeed = 20.0f;
    cloudPos.y = -120 * scale;
    static bool movingRight = true;
    if (movingRight) {
        cloudPos.x += cloudSpeed * GetFrameTime();
        if (cloudPos.x >= GetScreenWidth() - cloud.width * scale) movingRight = false;
    } else {
        cloudPos.x -= cloudSpeed * GetFrameTime();
        if (cloudPos.x <= 0) movingRight = true;
    }

    static float bgOffsetX = 0, bgRotation = 0;
    static bool bgMovingRight = true;
    float bgMaxOffset = 10 * scale;
    float bgMoveSpeed = 7.5f * GetFrameTime();
    float bgRotationSpeed = 0.45f * GetFrameTime();

    if (bgMovingRight) {
        bgOffsetX += bgMoveSpeed;
        bgRotation += bgRotationSpeed;
        if (bgOffsetX >= bgMaxOffset) bgMovingRight = false;
    } else {
        bgOffsetX -= bgMoveSpeed;
        bgRotation -= bgRotationSpeed;
        if (bgOffsetX <= -bgMaxOffset) bgMovingRight = true;
    }

    DrawTexturePro(main_menu,
        (Rectangle){0, 0, (float)main_menu.width, (float)main_menu.height},
        (Rectangle){bgOffsetX, 0, (float)screenWidth, (float)screenHeight},
        (Vector2){0, 0}, bgRotation, WHITE);

    DrawTextureEx(cloud, cloudPos, 0.0f, 1.1f * scale, MAGENTA);
    DrawText("DU Conquer", startX - 15, startY - spacing * 8, (int)(55 * scale), MAGENTA);

    int fontSize = (int)(27 * scale);
    btnNew.DrawButton(fontSize, BLANK);
    btnHigh.DrawButton(fontSize, BLANK);
    btnHelp.DrawButton(fontSize, BLANK);
    btnMap.DrawButton(fontSize, BLANK);
    btnAuthor.DrawButton(fontSize, BLANK);
    btnExit.DrawButton(fontSize, BLANK);
        if (showHelp)
    {
        int helpBoxWidth = (int)(600 * scale);
        int helpBoxHeight = (int)(300 * scale);
        int helpBoxX = (GetScreenWidth() - helpBoxWidth) / 2;
        int helpBoxY = (GetScreenHeight() - helpBoxHeight) / 2;
        DrawRectangle(helpBoxX, helpBoxY, helpBoxWidth, helpBoxHeight, Fade(BLACK, 0.8f));
        DrawText(helpText.c_str(), helpBoxX + 20, helpBoxY + 20, (int)(22 * scale), RAYWHITE);

        Rectangle closeBtn = {helpBoxX + helpBoxWidth - 35.0f, helpBoxY + 5.0f, 30.0f, 30.0f};
        DrawRectangleRec(closeBtn, RED);
        DrawText("X", (int)(closeBtn.x + 8), (int)(closeBtn.y + 5), 20, WHITE);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(GetMousePosition(), closeBtn))
        {
            PlayHoverSound();
            showHelp = false;
        }
    }
    if (showMap)
    {
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));
        DrawText("Dhaka University Map", 40, 40, (int)(40 * scale), WHITE);

        // Static variables for oscillation and rotation
        static float mapOffsetX = 0;
        static float mapRotation = 0;
        static bool movingRight = true;
        float maxOffset = 25 * scale;
        float moveSpeed = 25 * GetFrameTime();
        float rotationSpeed = 0.5f * GetFrameTime();

        if (movingRight)
        {
            mapOffsetX += moveSpeed;
            mapRotation += rotationSpeed;
            if (mapOffsetX >= maxOffset)
                movingRight = false;
        }
        else
        {
            mapOffsetX -= moveSpeed;
            mapRotation -= rotationSpeed;
            if (mapOffsetX <= -maxOffset)
                movingRight = true;
        }

        DrawTexturePro(mapTexture,
                       (Rectangle){0, 0, (float)mapTexture.width, (float)mapTexture.height},
                       (Rectangle){mapOffsetX, 0, (float)GetScreenWidth(), (float)GetScreenHeight()},
                       (Vector2){0, 0},
                       mapRotation,
                       WHITE);

        Rectangle closeBtn = {GetScreenWidth() - 45.0f, 15.0f, 30.0f, 30.0f};
        DrawRectangleRec(closeBtn, RED);
        DrawText("X", (int)(closeBtn.x + 8), (int)(closeBtn.y + 5), 20, WHITE);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(GetMousePosition(), closeBtn))
        {
            PlayHoverSound();
            showMap = false;
        }
    }

    if (showAuthor) {
        int boxW = 750 * scale;
        int boxH = 400 * scale;
        int x = (screenWidth - boxW) / 2;
        int y = (screenHeight - boxH) / 2;

        DrawRectangle(x, y, boxW, boxH, Fade(BLACK, 0.9f));
        DrawText("team @procrastintaors", x + 20, y + 20, 30, MAGENTA);

        const char* names[] = {"Farhan Labib", "Salwa Baki", "Raima Hridika", "Raisa Tabassum"};
        const char* githubs[] = {"farhanlabibahan", "emmeryxanos", "raihri", "joe4ffs"};

        for (int i = 0; i < 4; i++) {
            int lineY = y + 70 + i * 80;
            DrawAuthorAvatar(authorPics[i], (Vector2){(float)(x + 20), (float)(lineY)}, 50 * scale);
            DrawText(names[i], x + 90, lineY + 5, 22, WHITE);
            DrawText(TextFormat("@%s", githubs[i]), x + 90, lineY + 30, 20, LIGHTGRAY);
        }

        Rectangle closeBtn = {x + boxW - 35.0f, y + 5.0f, 30.0f, 30.0f};
        DrawRectangleRec(closeBtn, RED);
        DrawText("X", (int)(closeBtn.x + 8), (int)(closeBtn.y + 5), 20, WHITE);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
            CheckCollisionPointRec(GetMousePosition(), closeBtn)) {
            PlayHoverSound();
            showAuthor = false;
        }
    }

    UpdateMusicStream(bgmMenu);
}

void unload_menu() {
    if (main_menu.id) UnloadTexture(main_menu);
    if (cloud.id) UnloadTexture(cloud);
    if (mapTexture.id) UnloadTexture(mapTexture);
    for (int i = 0; i < 4; i++) {
        if (authorPics[i].id) UnloadTexture(authorPics[i]);
    }
    if (hoverSound.frameCount) UnloadSound(hoverSound);
    if (bgmMenu.frameCount) UnloadMusicStream(bgmMenu);
    CloseAudioDevice();
}

void MAIN_MENU() {
    init_menu();
    logic_draw_menu();
    unload_menu();
}
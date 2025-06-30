#include "raylib.h"

#define MAX_FRAME_SPEED 15
#define MIN_FRAME_SPEED 1

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [texture] example - 2-row 8-frame sprite anim");

    Texture2D scarfy = LoadTexture("LEFT_SS.png");  // 2-row × 4-column sprite sheet

    Vector2 position = { 400.0f, 380.0f };  // y adjusted for foot-based placement

    Rectangle frameRec = { 0.0f, 0.0f, (float)scarfy.width / 4, (float)scarfy.height / 2 };
    int currentFrame = 0;

    int framesCounter = 0;
    int framesSpeed = 8;

    bool moving = false;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // UPDATE --------------------------------------------------
        moving = false;

        if (IsKeyDown(KEY_D))
        {
            position.x += 2.0f;
            moving = true;
        }

        if (IsKeyDown(KEY_A))
        {
            position.x -= 2.0f;
            moving = true;
        }

        if (moving)
        {
            framesCounter++;
            if (framesCounter >= (60 / framesSpeed))
            {
                framesCounter = 0;
                currentFrame++;
                if (currentFrame > 7) currentFrame = 0;  // 8 frames total
            }
        }
        else
        {
            currentFrame = 0;
        }

        // Calculate row & column
        int frameRow = currentFrame / 4;
        int frameCol = currentFrame % 4;

        frameRec.x = frameCol * frameRec.width;
        frameRec.y = frameRow * frameRec.height;

        // DRAW ----------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);

        Vector2 drawPos = {
            position.x - frameRec.width / 2,
            position.y - frameRec.height  // align feet to position
        };

        DrawTextureRec(scarfy, frameRec, drawPos, WHITE);

        EndDrawing();
    }

    UnloadTexture(scarfy);
    CloseWindow();

    return 0;
}

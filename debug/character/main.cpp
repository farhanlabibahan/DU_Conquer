#include "raylib.h"
#include "character.h"

int main() {
    InitWindow(800, 600, "Character Walk");
    SetTargetFPS(60);

    init_character();

    while (!WindowShouldClose()) {
        walk_character_dept();
        ClearBackground(RAYWHITE);
        BeginDrawing();
        draw_char_dept();
        EndDrawing();
    }

    unload_character();
    CloseWindow();
    return 0;
}

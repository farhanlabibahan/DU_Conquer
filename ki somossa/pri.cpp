#include "raylib.h"
#include "tileson.hpp"
#include <filesystem>
#include <string>
namespace fs = std::filesystem;

int main() {
    const int screenWidth = 1200;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Isometric Map Viewer");
    SetTargetFPS(60);

    // Load tile texture
    Texture2D tileset = LoadTexture("tileson.png");

    // Parse map.json
    tson::Tileson parser;
    auto map = parser.parse(fs::path("map.json"));
    if (map->getStatus() != tson::ParseStatus::OK) {
        TraceLog(LOG_ERROR, "Failed to load map: %s", map->getStatusMessage().c_str());
        CloseWindow();
        return 1;
    }

    // Tile dimensions
    const int tileWidth = 64;
    const int tileHeight = 32;

    // Character setup
    Vector2 characterPos = {400, 300};
    float characterSpeed = 5.0f;
    int currentTileId = -1;
    std::string tileInfoText = "Standing on tile: None";

    // Initialize camera
    Camera2D camera = { 0 };
    camera.target = characterPos;
    camera.offset = { screenWidth/2.0f, screenHeight/2.0f };
    camera.zoom = 1.0f;

    // Game loop
    while (!WindowShouldClose()) {
        // Character movement
        if (IsKeyDown(KEY_RIGHT)) characterPos.x += characterSpeed;
        if (IsKeyDown(KEY_LEFT)) characterPos.x -= characterSpeed;
        if (IsKeyDown(KEY_DOWN)) characterPos.y += characterSpeed;
        if (IsKeyDown(KEY_UP)) characterPos.y -= characterSpeed;

        // Update camera to follow character
        camera.target = characterPos;

        // Handle zoom
        camera.zoom += GetMouseWheelMove() * 0.1f;
        if (camera.zoom < 0.2f) camera.zoom = 0.2f;
        if (camera.zoom > 3.0f) camera.zoom = 3.0f;

        // Convert character position to tile coordinates
        float isoX = (characterPos.x / tileWidth) + (characterPos.y / tileHeight);
        float isoY = (characterPos.y / tileHeight) - (characterPos.x / tileWidth);
        int tileX = static_cast<int>(isoX);
        int tileY = static_cast<int>(isoY);

        // Find the tile ID at this position
        currentTileId = -1;
        for (auto& layer : map->getLayers()) {
            if (layer.getType() != tson::LayerType::TileLayer) continue;
            
            if (layer.getTileData().count({tileX, tileY}) > 0) {
                tson::Tile* tile = layer.getTileData().at({tileX, tileY});
                if (tile) {
                    currentTileId = tile->getId();
                    break;
                }
            }
        }

        // Update tile info text
        if (currentTileId != -1) {
            tileInfoText = "Standing on tile: " + std::to_string(currentTileId);
        } else {
            tileInfoText = "Standing on tile: None";
        }

        // Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(camera);

        // Draw map tiles
        for (auto& layer : map->getLayers()) {
            if (layer.getType() != tson::LayerType::TileLayer) continue;

            for (const auto& item : layer.getTileData()) {
                auto pos = item.first;
                tson::Tile* tile = item.second;
                if (!tile) continue;

                const tson::Tileset* ts = tile->getTileset();
                if (!ts) continue;

                tson::Rect rect = const_cast<tson::Tileset*>(ts)->getTile(tile->getId())->getDrawingRect();
                Rectangle src = {
                    (float)rect.x,
                    (float)rect.y,
                    (float)rect.width,
                    (float)rect.height
                };

                int x = std::get<0>(pos);
                int y = std::get<1>(pos);

                Rectangle dest = {
                    static_cast<float>(x * tileWidth),
                    static_cast<float>(y * tileHeight),
                    static_cast<float>(tileWidth),
                    static_cast<float>(tileHeight)
                };

                DrawTexturePro(tileset, src, dest, {0, 0}, 0.0f, WHITE);
            }
        }

        // Draw character
        DrawCircleV(characterPos, 10, RED);

        EndMode2D();

        // Draw debug info
        DrawText(TextFormat("Character Position: (%.1f, %.1f)", characterPos.x, characterPos.y), 10, 10, 20, DARKGRAY);
        DrawText(tileInfoText.c_str(), 10, 40, 20, DARKGRAY);
        DrawText(TextFormat("Tile Coordinates: [%d, %d]", tileX, tileY), 10, 70, 20, DARKGRAY);
        DrawText(TextFormat("Camera Zoom: %.2f", camera.zoom), 10, 100, 20, DARKGRAY);
        DrawFPS(10, 130);

        EndDrawing();
    }

    UnloadTexture(tileset);
    CloseWindow();
    return 0;
}

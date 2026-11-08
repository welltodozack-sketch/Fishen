#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <fstream>
#include <vector>

// Dependency library
#include "raylib.h"

// External files
#include "loadLevelData.cpp"

struct Player {
    bool isTouching;
    int size;
    float x;
    float y;
    float rotation;
    float jumpPower;
    float velocity;
    Texture2D cube;
    Rectangle hitbox;
};

struct World {
    float x;
    float y;
    float groundMovement;
    float speed;
    Texture2D ground, backGround;
    Rectangle groundBox;
};

// Draw background and foreground textures
void drawOtherTextures(World& WORLD, std::unordered_map<std::string, Texture2D>& Textures) {
    WORLD.groundMovement -= WORLD.speed;

    if (WORLD.groundMovement <= -WORLD.ground.width) WORLD.groundMovement = 0;

    for (auto& object : Level) {
        DrawTextureEx(object.texture, Vector2({object.x, object.y}), 0.0f, 1.0f, WHITE);
    }

    DrawTextureEx(WORLD.ground, Vector2({WORLD.groundMovement, WORLD.y}), 0.0f, 1.0f, WHITE);
    DrawTextureEx(WORLD.ground, Vector2({WORLD.ground.width + WORLD.groundMovement, WORLD.y}), 0.0f, 1.0f, WHITE);
}

// Draw player icon on screen
void drawPlayer(Player& PLAYER) {
    DrawTexturePro(PLAYER.cube, 
        {0, 0, static_cast<float>(PLAYER.size), static_cast<float>(PLAYER.size)}, 
        {PLAYER.x, PLAYER.y, static_cast<float>(PLAYER.size), static_cast<float>(PLAYER.size)},
        Vector2({PLAYER.size / 2.0f, PLAYER.size / 2.0f}),
        PLAYER.rotation,
        WHITE);
}

// Optional: Draws hitboxes for each object in the world
void drawHitboxes(Player& PLAYER, World& WORLD) {
    DrawRectangleLinesEx(PLAYER.hitbox, 4.0f, RED); // PLAYER HITBOXES
    DrawRectangleLinesEx(WORLD.groundBox, 4.0f, GREEN); // GROUND HITBOXES

    for (auto& object : Level) {
        DrawRectangleLinesEx(object.hitbox, 4.0f, BLUE);
    }
}


// Draw Everything on screen in order (Order is VERY IMPORTANT!)
void drawOnScreen(std::unordered_map<std::string, Texture2D>& Textures, Player& PLAYER, World& WORLD) {
    drawOtherTextures(WORLD, Textures); // Background and foreground
    drawPlayer(PLAYER); // Draw player icon
    // drawHitboxes(PLAYER, WORLD); // Draw all world hitboxes
};
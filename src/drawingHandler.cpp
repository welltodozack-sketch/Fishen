#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <fstream>

// Dependency library
#include "raylib.h"

struct Player {
    bool isTouching;
    int size;
    float x, y, rotation, jumpPower, velocity;
    Texture2D cube;
    Rectangle hitbox;
};

struct World {
    float x, y, groundMovement, speed;
    Texture2D ground, backGround;
    Rectangle groundBox;
};


// Draw background and foreground textures
void drawBackgroundAndForeground(World& WORLD) {
    WORLD.groundMovement -= WORLD.speed;

    if (WORLD.groundMovement <= -WORLD.ground.width) WORLD.groundMovement = 0;

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
}
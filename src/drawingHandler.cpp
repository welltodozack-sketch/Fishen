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
    Rectangle innerBox;
};

struct World {
    float x;
    float y;
    float groundMovement;
    float backgroundMovement;
    float speed;
    Texture2D ground; 
    Texture2D backGround;
    Rectangle groundBox;
};

// Draw player icon on screen
void drawPlayer(Player& PLAYER) {
    DrawTexturePro(PLAYER.cube, 
        {0, 0, static_cast<float>(PLAYER.size), static_cast<float>(PLAYER.size)}, 
        {PLAYER.x, PLAYER.y, static_cast<float>(PLAYER.size), static_cast<float>(PLAYER.size)},
        Vector2({PLAYER.size / 2.0f, PLAYER.size / 2.0f}),
        PLAYER.rotation,
        WHITE);
}

// Draw background and foreground textures
void drawTextures(World& WORLD, Player& PLAYER, std::unordered_map<std::string, Texture2D>& Textures) {
    WORLD.groundMovement -= WORLD.speed;
    WORLD.backgroundMovement -= WORLD.speed / 3.0f;

    if (WORLD.groundMovement <= -WORLD.ground.width) WORLD.groundMovement = 0;
    if (WORLD.backgroundMovement <= -WORLD.backGround.width) WORLD.backgroundMovement = 0;

    DrawTextureEx(WORLD.backGround, Vector2({WORLD.backgroundMovement, -100}), 0.0f, 1.0f, WHITE);
    DrawTextureEx(WORLD.backGround, Vector2({WORLD.backGround.width + WORLD.backgroundMovement, -100}), 0.0f, 1.0f, WHITE);

    drawPlayer(PLAYER); // Draw player icon
    for (auto& object : Level) {
        if (object.x <= GetScreenWidth() + object.texture.width && object.x >= 0 - object.texture.width) {
            DrawTextureEx(object.texture, Vector2({object.x, object.y}), 0.0f, 1.0f, WHITE);
        }
    }

    DrawTextureEx(WORLD.ground, Vector2({WORLD.groundMovement, WORLD.y}), 0.0f, 1.0f, WHITE);
    DrawTextureEx(WORLD.ground, Vector2({WORLD.ground.width + WORLD.groundMovement, WORLD.y}), 0.0f, 1.0f, WHITE);

}

// Optional: Draws hitboxes for each object in the world
void drawHitboxes(Player& PLAYER, World& WORLD) {
    DrawRectangleLinesEx(PLAYER.hitbox, 4.0f, RED); // PLAYER HITBOXES
    DrawRectangleLinesEx(PLAYER.innerBox, 4.0f, BLUE); // PLAYER HITBOXES
    // DrawRectangleLinesEx(WORLD.groundBox, 4.0f, GREEN); // GROUND HITBOXES

    for (auto& object : Level) {
        DrawRectangleLinesEx(object.hitbox, 4.0f, BLUE);
    }
}


// Draw Everything on screen in order (Order is VERY IMPORTANT!)
void drawOnScreen(std::unordered_map<std::string, Texture2D>& Textures, Player& PLAYER, World& WORLD) {
    drawTextures(WORLD, PLAYER, Textures); // Draw textures on screen
    // drawHitboxes(PLAYER, WORLD); // Draw all world hitboxes
};
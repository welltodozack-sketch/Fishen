#include "Fishen.hpp"
#include "raylib.h"
#include <chrono>
#include <thread>
#include <unistd.h>

// Initialize all struct objects with default values
void initStructs(Player& PLAYER, World& WORLD, std::unordered_map<std::string, Texture2D>& Textures) {
    // Player
    PLAYER.isTouching = false;

    PLAYER.size = 100;
    PLAYER.x = GetScreenWidth() / 3.0f;
    PLAYER.y = GetScreenHeight() - PLAYER.size;
    PLAYER.rotation = 0.0f;
     
    switch (FRAMERATE) {
        case 60:
            PLAYER.jumpPower = 30;
            break;

        case 120:
            PLAYER.jumpPower = 15;
            break;

        default:
            CloseWindow();
    }

    PLAYER.velocity = 0.0f;

    PLAYER.cube = Textures.at("icon1");
    PLAYER.hitbox = {PLAYER.x - (PLAYER.size / 2.0f), PLAYER.y - (PLAYER.size / 2.0f), static_cast<float>(PLAYER.size), static_cast<float>(PLAYER.size)};
    PLAYER.innerBox = {PLAYER.x - (PLAYER.size / 2.0f), PLAYER.y, PLAYER.size / 3.0f, PLAYER.size / 3.0f};

    // Background and foreground
    WORLD.ground = Textures.at("ground1");
    WORLD.backGround = Textures.at("background1");
    WORLD.x = 0.0f;
    WORLD.y = static_cast<float>(GetScreenHeight() - WORLD.ground.height);
    WORLD.groundMovement = 0;
    WORLD.backgroundMovement = 0;

    switch (FRAMERATE) {
        case 60:
            WORLD.speed = 10.0f;
            break;

        case 120:
            WORLD.speed = 5.0f;
            break;

        default:
            CloseWindow();
    }
    
    WORLD.groundBox = {WORLD.x, WORLD.y, static_cast<float>(WORLD.ground.width), static_cast<float>(WORLD.ground.height)};
}

void onDeath(World& WORLD, Player& PLAYER, bool isInputTriggered) {
    Level = levelResetState;
    WORLD.groundMovement = 0;
    WORLD.backgroundMovement = 0;

    PLAYER.x = GetScreenWidth() / 3.0f;
    PLAYER.y = GetScreenHeight() - PLAYER.size;
    PLAYER.rotation = 0.0f;

    if (isInputTriggered) {
        return;
    }
    else std::this_thread::sleep_for(std::chrono::milliseconds(750));
}

// Handle user inputs
void handleInputs(Player& PLAYER) {
    if (IsKeyDown(KEY_UP) && PLAYER.isTouching ||
        IsKeyDown(KEY_W) && PLAYER.isTouching ||
        IsKeyDown(KEY_SPACE) && PLAYER.isTouching ||
        IsMouseButtonDown(MOUSE_LEFT_BUTTON) && PLAYER.isTouching) 
        {
        PLAYER.isTouching = false;
        PLAYER.velocity = PLAYER.jumpPower;
        }
}

// Handle player rotation animation
void handlePlayerRotation(Player& PLAYER) {

    if (PLAYER.rotation >= 315 || PLAYER.rotation <= 45) PLAYER.rotation = 0.0f;
    else if (PLAYER.rotation >= 45 && PLAYER.rotation <= 135) PLAYER.rotation = 90.0f;
    else if (PLAYER.rotation >= 135 && PLAYER.rotation <= 225) PLAYER.rotation = 180.0f;
    else if (PLAYER.rotation >= 225 && PLAYER.rotation <= 315) PLAYER.rotation = 270.0f;

}

// Update all stored level objects position
void updateGlobalObjectPosition(World& WORLD) {
    for (auto& object : Level) {
        object.x -= WORLD.speed;
        object.hitbox.x = object.x;
    }
}

// Update player state
void updatePlayer(Player& PLAYER) {

    // Update player velocity
    if (!PLAYER.isTouching) {
        
        switch (FRAMERATE) {
            case 60:
                PLAYER.velocity -= 2.0f;
                PLAYER.rotation += 6.25f;
                break;

            case 120:
                PLAYER.velocity -= 0.5f;
                PLAYER.rotation += 3.075f;
                break;
            
            default:
                CloseWindow();
        }

        if (PLAYER.rotation >= 360) PLAYER.rotation = 0.0f;
    }
    else {
        handlePlayerRotation(PLAYER);
    }

    PLAYER.y -= PLAYER.velocity;
    
    PLAYER.hitbox.y = PLAYER.y - (PLAYER.size / 2.0f);
    PLAYER.hitbox.x = PLAYER.x - (PLAYER.size / 2.0f);

    PLAYER.innerBox.y = PLAYER.y - (PLAYER.size / 2.0f) + (PLAYER.size / 3.0f);
    PLAYER.innerBox.x = PLAYER.x - (PLAYER.size / 2.0f) + (PLAYER.size / 3.0f);
}


// handle collision checks

// handle collision checks
void checkCollisions(Player& PLAYER, World& WORLD) {

    bool collided = false;

    // Capture velocity before collision detection zeroes it
    const float vel = PLAYER.velocity;

    if (CheckCollisionRecs(PLAYER.hitbox, WORLD.groundBox)) {
        collided = true;
        PLAYER.y = WORLD.groundBox.y - (PLAYER.size / 2.0f) + 0.5f;
        PLAYER.hitbox.y = PLAYER.y - (PLAYER.size / 2.0f);
        PLAYER.innerBox.y = PLAYER.y - (PLAYER.size / 2.0f) + (PLAYER.size / 3.0f);
        PLAYER.velocity = 0.0f;
    }

    for (auto& object : Level) {
        if (!CheckCollisionRecs(PLAYER.hitbox, object.hitbox)) continue; // Iterate through objects

        // Get bottom of player hitbox and previous position
        float bottomOfPlayer = PLAYER.hitbox.y + PLAYER.hitbox.height;
        float previousBottom = bottomOfPlayer + vel;

        // Determine if the player is falling down && if previous y is higher than the top of the block
        bool landing = (vel <= 0.0f) && (previousBottom <= object.hitbox.y + 1.0f);

        // If the player is above the block
        if (object.type == 0 && landing) {
            collided = true;
            PLAYER.y = object.hitbox.y - (PLAYER.size / 2.0f) + 0.5f;
            PLAYER.hitbox.y = PLAYER.y - (PLAYER.size / 2.0f);
            PLAYER.innerBox.y = PLAYER.y - (PLAYER.size / 2.0f) + (PLAYER.size / 3.0f);
            PLAYER.velocity = 0.0f;
        }

        // Else check for collision with the inner box (if the player is bellow the top of the block)
        else if (CheckCollisionRecs(PLAYER.innerBox, object.hitbox)) {
            onDeath(WORLD, PLAYER, false);
            return; // stop iterating - reset level
        }
    }

    PLAYER.isTouching = collided;
}

/*
void checkCollisions(struct Player& PLAYER, struct World& WORLD) {

    bool collided = false;

    if (CheckCollisionRecs(PLAYER.hitbox, WORLD.groundBox)) {
            collided = true;
            PLAYER.y = WORLD.groundBox.y - PLAYER.size / 2.0f + 0.5f;
            PLAYER.hitbox.y = PLAYER.y - PLAYER.size / 2.0f;
            PLAYER.innerBox.y = PLAYER.y - PLAYER.size / 2.0f + PLAYER.size / 3.0f;
            PLAYER.velocity = 0.0f;   
    }
    
    for (auto& object : Level) {
        if (CheckCollisionRecs(PLAYER.hitbox, object.hitbox)) {

            if (PLAYER.y > object.hitbox.y + (object.hitbox.height / 5.0f) && object.type == 0) {
                if (CheckCollisionRecs(PLAYER.innerBox, object.hitbox)) {
                    onDeath(WORLD);
                }
            }
            else {
                collided = true;
                PLAYER.y = object.hitbox.y - PLAYER.size / 2.0f + 0.5f;
                PLAYER.hitbox.y = PLAYER.y - PLAYER.size / 2.0f;
                PLAYER.innerBox.y = PLAYER.y - PLAYER.size / 2.0f + PLAYER.size / 3.0f;
                PLAYER.velocity = 0.0f;
            }
        }
    }

    PLAYER.isTouching = collided;

}

*/

// MAIN
int main(void)
{

    Player PLAYER;
    World WORLD;

    std::string path = "../resources/textures";
    std::string levelPath = "../resources/levels/level1.txt";
    std::unordered_map<std::string, Texture2D> Textures;

    InitWindow(WIDTH, HEIGHT, WINDOW_TITLE);
    SetTargetFPS(FRAMERATE);
    
    // Load game data and initialize
    loadTextureData(path, Textures);
    loadLevelFromFile(levelPath, Textures);
    initStructs(PLAYER, WORLD, Textures);

    // Game loop
    while (!WindowShouldClose())
    {

        if (IsKeyPressed(KEY_R)) {
            onDeath(WORLD, PLAYER, true);
        }

        // Background checks
        handleInputs(PLAYER);
        updateGlobalObjectPosition(WORLD);
        updatePlayer(PLAYER);
        checkCollisions(PLAYER, WORLD);

        // Update frames
        BeginDrawing();
            ClearBackground(BLACK);
            drawOnScreen(Textures, PLAYER, WORLD);
        EndDrawing();
    }

    unloadTextureData(Textures);
    CloseWindow();

    return 0;
}
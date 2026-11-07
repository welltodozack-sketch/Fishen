#include "Fishen.hpp"

// Initialize all struct objects with default values
void initStructs(Player& PLAYER, World& WORLD, std::unordered_map<std::string, Texture2D>& Textures) {
    // Player
    PLAYER.isTouching = true;

    PLAYER.size = 90;
    PLAYER.x = GetScreenWidth() / 3.0f;
    PLAYER.y = GetScreenHeight() - PLAYER.size;
    PLAYER.rotation = 0.0f;
     
    switch (FRAMERATE) {
        case 60:
            PLAYER.jumpPower = 25;
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

    // Background and foreground
    WORLD.ground = Textures.at("ground1");
    WORLD.x = 0.0f;
    WORLD.y = static_cast<float>(GetScreenHeight() - WORLD.ground.height);
    WORLD.groundMovement = 0;

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

// Update player state
void updatePlayer(Player& PLAYER) {

    // Update player velocity
    if (!PLAYER.isTouching) {
        
        switch (FRAMERATE) {
            case 60:
                PLAYER.velocity -= 1.5f;
                PLAYER.rotation += 5.5f;
                break;

            case 120:
                PLAYER.velocity -= 0.5f;
                PLAYER.rotation += 3.0f;
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
}

void checkCollisions(struct Player& PLAYER, struct World& WORLD) {
    if (CheckCollisionRecs(PLAYER.hitbox, WORLD.groundBox)) {
        PLAYER.isTouching = true;
        PLAYER.y = WORLD.groundBox.y - (PLAYER.size / 2.0f);
        PLAYER.hitbox.y = PLAYER.y - (PLAYER.size / 2.0f);
        PLAYER.velocity = 0.0f;
    }
}


// MAIN
int main(void)
{

    Player PLAYER;
    World WORLD;

    std::string path = "../resources/textures";
    std::unordered_map<std::string, Texture2D> Textures;

    InitWindow(WIDTH, HEIGHT, WINDOW_TITLE);
    loadTextureData(path, Textures);
    initStructs(PLAYER, WORLD, Textures);
    SetTargetFPS(FRAMERATE);

    while (!WindowShouldClose())
    {
        // Background checks
        handleInputs(PLAYER);
        updatePlayer(PLAYER);
        checkCollisions(PLAYER, WORLD);

        // Update frames
        BeginDrawing();
            ClearBackground(BLACK);
            drawBackgroundAndForeground(WORLD);
            drawPlayer(PLAYER);
            // drawHitboxes(PLAYER, WORLD);
        EndDrawing();
    }

    unloadTextureData(Textures);
    CloseWindow();

    return 0;
}
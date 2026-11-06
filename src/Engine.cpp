#include "Fishen.hpp"

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

void initStructs(Player& PLAYER, World& WORLD) {
    // Player
    PLAYER.isTouching = true;

    PLAYER.size = 90;
    PLAYER.x = GetScreenWidth() / 1.75;
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

    PLAYER.cube = LoadTexture("../resources/textures/icon1.png");
    PLAYER.hitbox = {PLAYER.x - (PLAYER.size / 2.0f), PLAYER.y - (PLAYER.size / 2.0f), static_cast<float>(PLAYER.size), static_cast<float>(PLAYER.size)};

    // Background and foreground
    WORLD.ground = LoadTexture("../resources/textures/ground1.png");
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

void checkCollisions(Player& PLAYER, World& WORLD) {
    if (CheckCollisionRecs(PLAYER.hitbox, WORLD.groundBox)) {
        PLAYER.isTouching = true;
        PLAYER.y = WORLD.groundBox.y - (PLAYER.size / 2.0f);
        PLAYER.hitbox.y = PLAYER.y - (PLAYER.size / 2.0f);
        PLAYER.velocity = 0.0f;
    }
}

void handlePlayerRotation(Player& PLAYER) {

    if (PLAYER.rotation >= 315 || PLAYER.rotation <= 45) PLAYER.rotation = 0.0f;
    else if (PLAYER.rotation >= 45 && PLAYER.rotation <= 135) PLAYER.rotation = 90.0f;
    else if (PLAYER.rotation >= 135 && PLAYER.rotation <= 225) PLAYER.rotation = 180.0f;
    else if (PLAYER.rotation >= 225 && PLAYER.rotation <= 315) PLAYER.rotation = 270.0f;

}

void updatePlayer(Player& PLAYER) {
    PLAYER.x = static_cast<int>(GetScreenWidth() - GetScreenWidth() + 200);

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

void drawBackgroundAndForeground(World& WORLD) {
    WORLD.groundMovement -= WORLD.speed;

    if (WORLD.groundMovement <= -WORLD.ground.width) WORLD.groundMovement = 0;

    DrawTextureEx(WORLD.ground, Vector2({WORLD.groundMovement, WORLD.y}), 0.0f, 1.0f, WHITE);
    DrawTextureEx(WORLD.ground, Vector2({WORLD.ground.width + WORLD.groundMovement, WORLD.y}), 0.0f, 1.0f, WHITE);
}

void drawPlayer(Player& PLAYER) {
    DrawTexturePro(PLAYER.cube, 
        {0, 0, static_cast<float>(PLAYER.size), static_cast<float>(PLAYER.size)}, 
        {PLAYER.x, PLAYER.y, static_cast<float>(PLAYER.size), static_cast<float>(PLAYER.size)},
        Vector2({PLAYER.size / 2.0f, PLAYER.size / 2.0f}),
        PLAYER.rotation,
        WHITE);
}

void drawHitboxes(Player& PLAYER, World& WORLD) {
    DrawRectangleLinesEx(PLAYER.hitbox, 4.0f, RED); // PLAYER HITBOXES
    DrawRectangleLinesEx(WORLD.groundBox, 4.0f, RED); // GROUND HITBOXES
}

int main(void)
{

    Player PLAYER;
    World WORLD;

    InitWindow(WIDTH, HEIGHT, WINDOW_TITLE);
    initStructs(PLAYER, WORLD);
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

    CloseWindow();
    UnloadTexture(PLAYER.cube);
    UnloadTexture(WORLD.ground);

    return 0;
}
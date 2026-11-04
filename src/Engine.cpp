#include "Fishen.hpp"
#include "raylib.h"

struct Player {
    bool isTouching;
    int size;
    float x, y, jumpPower, velocity;
    Texture2D cube;
};

void initStructs(Player& PLAYER) {
    // Player
    PLAYER.isTouching = true;
    PLAYER.size = 100;
    PLAYER.x = GetScreenWidth() / 2.25;
    PLAYER.y = GetScreenHeight() - PLAYER.size;
    PLAYER.jumpPower = static_cast<float>(FRAMERATE) / 3.2;
    PLAYER.velocity = 0.0f;
    PLAYER.cube = LoadTexture("../resources/textures/icon1.png");
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

void checkCollisions(Player& PLAYER) {
    if (PLAYER.y > GetScreenHeight() - PLAYER.size) {
        PLAYER.isTouching = true;
        PLAYER.velocity = 0.0f;
        PLAYER.y = GetScreenWidth() - PLAYER.size;
    }
}

void updatePlayer(Player& PLAYER) {
    PLAYER.x = static_cast<int>(GetScreenWidth() - GetScreenWidth() + 200);

    // Update player velocity
    if (!PLAYER.isTouching) {
        PLAYER.velocity -= 1.1;
    }
    else {
        PLAYER.y = GetScreenHeight() - PLAYER.size;
    }

    PLAYER.y -= PLAYER.velocity;
}

void drawPlayer(Player& PLAYER) {
    DrawTexture(PLAYER.cube, PLAYER.x, PLAYER.y, WHITE);
}

int main(void)
{

    Player PLAYER;

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(WIDTH, HEIGHT, WINDOW_TITLE);
    initStructs(PLAYER);
    SetTargetFPS(FRAMERATE);

    while (!WindowShouldClose())
    {

        // Background checks
        handleInputs(PLAYER);
        checkCollisions(PLAYER);
        updatePlayer(PLAYER);

        // Update frames
        BeginDrawing();
            ClearBackground(BLACK);
            drawPlayer(PLAYER);
        EndDrawing();
    }

    CloseWindow();
    UnloadTexture(PLAYER.cube);

    return 0;
}
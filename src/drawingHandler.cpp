// System libraries
#include <unordered_map>

// Dependency library
#include "raylib.h"

// External files
#include "loadLevelData.cpp"

struct Player {
    bool isTouching;
    bool isPlaying;
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

struct Menu {
    bool isActive;
    Rectangle continueButton;
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
void drawHitboxes(Player& PLAYER, World& WORLD, Menu& MENU) {
    DrawRectangleLinesEx(PLAYER.hitbox, 4.0f, RED); // PLAYER HITBOXES
    DrawRectangleLinesEx(PLAYER.innerBox, 4.0f, BLUE); // PLAYER HITBOXES
    // DrawRectangleLinesEx(WORLD.groundBox, 4.0f, GREEN); // GROUND HITBOXES

    for (auto& object : Level) {

        switch (object.type) {
        case 0:
            DrawRectangleLinesEx(object.hitbox, 4.0f, BLUE);
            break;
        
            case 1:
            DrawRectangleLinesEx(object.hitbox, 4.0f, RED);
            break;
        
        default:
            break;
        }

        // if (object.type == 0) {
        //     DrawRectangleLinesEx(object.hitbox, 4.0f, BLUE);
        // }
        // else if (object.type == 1) {
        //     DrawRectangleLinesEx(object.hitbox, 4.0f, BLUE);
        // }
    }
}

void drawPauseMenu(std::unordered_map<std::string, Texture2D>& Textures, Menu& MENU) {
    static Texture2D pauseMenuTexture = Textures.at("pauseMenu");
    static Texture2D continueButtonTexture = Textures.at("continueButton");
    static Texture2D continueButtonTextureHover = Textures.at("continueButtonHover");
    static Texture2D continueButtonTextureInput = Textures.at("continueButtonInput");
    Vector2 mousePos = GetMousePosition();

    // Pause menu texture
    DrawTextureEx(pauseMenuTexture, 
        Vector2({0, 0}), 
        0.0f,
        1.0f,
        WHITE);

    
    if (!CheckCollisionPointRec(mousePos, MENU.continueButton)) {
        // continue button
        DrawTextureEx(continueButtonTexture, 
            (Vector2){GetScreenWidth()/2.0f - (continueButtonTexture.width/2.0f), GetScreenHeight()/2.0f - (continueButtonTexture.width/2.5f)}, 
            0.0f,
            1.0f,
            WHITE);
    }
    else if (CheckCollisionPointRec(mousePos, MENU.continueButton)) {

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
           // continue button
         DrawTextureEx(continueButtonTextureInput, 
            (Vector2){GetScreenWidth()/2.0f - (continueButtonTexture.width/2.0f), GetScreenHeight()/2.0f - (continueButtonTexture.width/2.5f)}, 
            0.0f,
            1.0f,
            WHITE);          
        }
        else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            MENU.isActive = false;
        }
        else {
        // continue button
        DrawTextureEx(continueButtonTextureHover, 
            (Vector2){GetScreenWidth()/2.0f - (continueButtonTexture.width/2.0f), GetScreenHeight()/2.0f - (continueButtonTexture.width/2.5f)}, 
            0.0f,
            1.0f,
            WHITE);
        }
    }
    
}

void drawMainMenu() {
    
}

// Draw Everything on screen in order (Order is VERY IMPORTANT!)
void drawOnScreen(std::unordered_map<std::string, Texture2D>& Textures, Player& PLAYER, World& WORLD, Menu& MENU) {
    drawTextures(WORLD, PLAYER, Textures); // Draw textures on screen
    // drawHitboxes(PLAYER, WORLD, MENU); // Draw all world hitboxes
    if (MENU.isActive) drawPauseMenu(Textures, MENU);
};
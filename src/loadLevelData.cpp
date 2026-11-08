#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <fstream>
#include <vector>

// Dependency library
#include "raylib.h"

struct levelObject {
    Texture2D texture;
    float x;
    float y;
    Rectangle hitbox;
};
std::vector<levelObject> Level;
std::vector<levelObject> levelResetState;

void assignHitboxToObject(std::unordered_map<std::string, Texture2D>& Textures, std::string& object, float& x, float& y, Rectangle& hitbox, Texture2D& textureData) {
    
    if (object == "block1") {
        hitbox = {x,
            y,
            static_cast<float>(textureData.width),
            static_cast<float>(textureData.height)
        };
    }
}


void loadLevelFromFile(const std::string& path, std::unordered_map<std::string, Texture2D>& Textures) {
    if (std::filesystem::exists(path)) {
        std::string object;
        float x;
        float y;
        Rectangle hitbox;

        std::fstream readFromFile;
        readFromFile.open(path);

        while(readFromFile >> object >> x >> y) {
            Texture2D textureData = Textures.at(object); 
            y = static_cast<float>(GetScreenHeight()) - Textures.at("ground1").height - textureData.height - y;
            assignHitboxToObject(Textures, object, x, y, hitbox, textureData);
            Level.push_back({Textures.at(object), x, y, hitbox});

            object = "";
            x = 0;
            y = 0;
            hitbox = {};
        }

        readFromFile.close();
        levelResetState = Level;
    }
}
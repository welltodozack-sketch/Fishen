#include <cctype>
#include <codecvt>
#include <cstring>
#include <forward_list>
#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <fstream>
#include <vector>

// Dependency library
#include "raylib.h"

struct levelObject {
    Texture2D texture;
    int type;
    float x;
    float y;
    Rectangle hitbox;
};
std::vector<levelObject> Level;
std::vector<levelObject> levelResetState;


void assignTypeToObject(std::string& object, int& type) {
    std::string forEachCharacter;

    for (int i = 0; i < strlen(object.c_str()); ++i) {
        if (!std::isdigit(object[i])) {
            forEachCharacter += object[i];
        }
        else {
            break;
        } 
    }

    if (forEachCharacter == "block") {
        type = 0;
    }
}

void assignHitboxToObject(std::unordered_map<std::string, Texture2D>& Textures, int& type, float& x, float& y, Rectangle& hitbox, Texture2D& textureData) {
    
    if (type == 0) {
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
        int type;
        float x;
        float y;
        Rectangle hitbox;

        std::fstream readFromFile;
        readFromFile.open(path);

        while(readFromFile >> object >> x >> y) {
            Texture2D textureData = Textures.at(object); 
            y = static_cast<float>(GetScreenHeight()) - Textures.at("ground1").height - textureData.height - y;
            // Assign specific values
            assignTypeToObject(object, type);
            assignHitboxToObject(Textures, type, x, y, hitbox, textureData);
            // Push complete level data back
            Level.push_back({Textures.at(object), type, x, y, hitbox});

            object = "";
            x = 0;
            y = 0;
            hitbox = {};
        }

        readFromFile.close();
        levelResetState = Level;
    }
}
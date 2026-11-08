#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <fstream>

// Dependency library
#include "raylib.h"

void loadTextureData(std::string path, std::unordered_map<std::string, Texture2D>& Textures) {
    if (std::filesystem::exists(path)) {
        std::ifstream Data;
        Data.open(path);

        for (auto& file : std::filesystem::directory_iterator(path)) {
            if (file.path().extension() == ".png") {
                std::string Key = file.path().stem().string();
                Textures[Key] = LoadTexture(file.path().c_str());
            }
        }

        Data.close();
    }
}

void unloadTextureData(std::unordered_map<std::string, Texture2D>& Textures) {
    for (auto& [Key, texture] : Textures) {
        UnloadTexture(texture);
    }
}
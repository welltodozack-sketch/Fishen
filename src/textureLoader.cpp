// System libraries
#include <filesystem>
#include <unordered_map>
#include <fstream>

// Dependency library
#include "raylib.h"

void loadTextureData(const std::string& path, std::unordered_map<std::string, Texture2D>& Textures, std::unordered_map<std::string, Sound>& Audio) {
    std::string texturesPath = path + "/textures";
    std::string audioPath = path + "/audio";

    if (std::filesystem::exists(texturesPath)) {
        std::ifstream Data;
        Data.open(texturesPath);

        for (auto& file : std::filesystem::directory_iterator(texturesPath)) {
            if (file.path().extension() == ".png") {
                std::string Key = file.path().stem().string();
                Textures[Key] = LoadTexture(file.path().string().c_str());
            }
        }

        Data.close();
    }

    if (std::filesystem::exists(audioPath)) {
        std::ifstream Data;
        Data.open(audioPath);

        for (auto& file : std::filesystem::directory_iterator(audioPath)) {
            if (file.path().extension() == ".mp3") {
                std::string Key = file.path().stem().string();
                Audio[Key] = LoadSound(file.path().string().c_str());
            }
        }

        Data.close();
    }
}

void unloadTextureData(std::unordered_map<std::string, Texture2D>& Textures, std::unordered_map<std::string, Sound>& Audio) {
    for (auto& [Key, texture] : Textures) {
        UnloadTexture(texture);
    }
    
    for (auto& [Key, audioFile] : Audio) {
        UnloadSound(audioFile);
    }
}
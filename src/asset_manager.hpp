#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <string>
#include <unordered_map>

class AssetManager
{
private:
  std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> textures;

public:
  AssetManager(SDL_Renderer *renderer);
  std::shared_ptr<SDL_Texture> get_texture(std::string name);
};
#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <string>
#include <unordered_map>

class Assets {
  private:
    // Renderer is cleaned up automatically by SDL, so it doesn't need to be a smart pointer.
    SDL_Renderer *renderer;
    std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> textures;

  public:
    Assets(SDL_Renderer *renderer);
    void load_png_texture(std::string name, std::string path);
    std::shared_ptr<SDL_Texture> get_texture(std::string name) const;
};
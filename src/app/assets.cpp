#include "assets.hpp"
#include "../util/sdl_util.hpp"
#include <SDL3/SDL.h>
#include <format>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

Assets::Assets(SDL_Renderer *renderer) : renderer{renderer} {};

void Assets::load_png_texture(std::string name, std::string path) {
  auto png_surface = std::unique_ptr<SDL_Surface, SDLDeleter>(SDL_LoadPNG(path.c_str()));
  if (!png_surface.get()) {
    throw std::runtime_error(std::format("Failed to create PNG surface '{}': {}", "", SDL_GetError()));
  }

  auto png_texture =
      std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, png_surface.get()), SDLDeleter());
  if (!png_texture.get()) {
    throw std::runtime_error(std::format("Failed to create PNG texture from surface '{}': {}", "", SDL_GetError()));
  }

  SDL_SetTextureScaleMode(png_texture.get(), SDL_SCALEMODE_PIXELART);

  textures.insert({name, png_texture});
}

std::shared_ptr<SDL_Texture> Assets::get_texture(std::string name) {
  return textures.at(name);
}

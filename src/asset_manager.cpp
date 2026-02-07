#include "asset_manager.hpp"
#include "sdl_util.hpp"
#include <SDL3/SDL.h>
#include <format>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

AssetManager::AssetManager(SDL_Renderer *renderer)
{

  auto png_surface = std::unique_ptr<SDL_Surface, SDLDeleter>(SDL_LoadPNG("./assets/space_invaders.png"));
  if (!png_surface.get())
  {
    throw std::runtime_error(std::format("Failed to create PNG surface: {}", SDL_GetError()));
  }

  auto png_texture =
      std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, png_surface.get()), SDLDeleter());
  if (!png_texture.get())
  {
    throw std::runtime_error(std::format("Failed to create PNG texture from surface: {}", SDL_GetError()));
  }

  SDL_SetTextureScaleMode(png_texture.get(), SDL_SCALEMODE_PIXELART);

  textures.insert({"example", png_texture});
};

std::shared_ptr<SDL_Texture> AssetManager::get_texture(std::string name)
{
  return textures.at(name);
}
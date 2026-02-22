#include "assets.hpp"
#include "sdl_util.hpp"
#include <SDL3/SDL.h>
#include <format>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>

Assets::Assets(SDL_Renderer *renderer, std::shared_ptr<MIX_Mixer> mixer)
    : renderer{renderer},
      mixer{mixer} {};

void Assets::load_png_texture(std::string name, std::string path) {
  auto png_surface = std::unique_ptr<SDL_Surface, SDLDeleter>(SDL_LoadPNG(path.c_str()));
  if (!png_surface.get()) {
    throw std::runtime_error(std::format("Failed to create PNG surface '{}': {}", path, SDL_GetError()));
  }

  auto png_texture =
      std::shared_ptr<SDL_Texture>(SDL_CreateTextureFromSurface(renderer, png_surface.get()), SDLDeleter());
  if (!png_texture.get()) {
    throw std::runtime_error(std::format("Failed to create PNG texture from surface '{}': {}", path, SDL_GetError()));
  }

  SDL_SetTextureScaleMode(png_texture.get(), SDL_SCALEMODE_PIXELART);

  textures.insert({name, png_texture});
}

std::shared_ptr<SDL_Texture> Assets::get_texture(std::string name) const {
  return textures.at(name);
}

void Assets::load_audio(std::string name, std::string path) {
  auto audio_track = std::shared_ptr<MIX_Track>(MIX_CreateTrack(mixer.get()), SDLDeleter());
  if (!audio_track.get()) {
    // TODO this could just return and the game would still work (albeit without sound)
    throw std::runtime_error(std::format("Couldn't create bgm track: {}", SDL_GetError()));
  }

  auto audio = std::shared_ptr<MIX_Audio>(MIX_LoadAudio(mixer.get(), path.c_str(), true), SDLDeleter());
  if (!audio.get()) {
    throw std::runtime_error(std::format("Failed to load audio '{}': {}", path, SDL_GetError()));
  }

  MIX_SetTrackAudio(audio_track.get(), audio.get());

  audio_tracks.insert({name, audio_track});
}

void Assets::play_audio(std::string audio_name) const {
  if (audio_tracks.contains(audio_name)) {
    MIX_PlayTrack(audio_tracks.at(audio_name).get(), 0);
  }
}

void Assets::stop_audio(std::string audio_name) const {
  if (audio_tracks.contains(audio_name)) {
    MIX_StopTrack(audio_tracks.at(audio_name).get(), 0);
  }
}
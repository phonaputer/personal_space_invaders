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
      mixer{mixer} {
  auto sfx_track_p = MIX_CreateTrack(mixer.get());
  if (!sfx_track_p) {
    throw std::runtime_error(std::format("Couldn't create sfx track: {}", SDL_GetError()));
  }
  sfx_track = std::unique_ptr<MIX_Track, SDLDeleter>(sfx_track_p, SDLDeleter());

  auto bgm_track_p = MIX_CreateTrack(mixer.get());
  if (!bgm_track_p) {
    throw std::runtime_error(std::format("Couldn't create bgm track: {}", SDL_GetError()));
  }
  bgm_track = std::unique_ptr<MIX_Track, SDLDeleter>(bgm_track_p, SDLDeleter());
};

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
  auto audio = std::shared_ptr<MIX_Audio>(MIX_LoadAudio(mixer.get(), path.c_str(), true), SDLDeleter());
  if (!audio.get()) {
    throw std::runtime_error(std::format("Failed to load audio '{}': {}", path, SDL_GetError()));
  }

  audios.insert({name, audio});
}

void Assets::play_sfx(std::string audio_name) {
  if (audios.contains(audio_name)) {
    MIX_SetTrackAudio(sfx_track.get(), audios.at(audio_name).get());
    MIX_PlayTrack(sfx_track.get(), 0);
  }
}

void Assets::play_bgm(std::string audio_name) {
  if (audios.contains(audio_name)) {
    MIX_SetTrackAudio(bgm_track.get(), audios.at(audio_name).get());
    MIX_PlayTrack(bgm_track.get(), 0);
  }
}
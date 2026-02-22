#pragma once

#include "sdl_util.hpp"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <memory>
#include <string>
#include <unordered_map>

class Assets {
  private:
    // Renderer is cleaned up automatically by SDL, so it doesn't need to be a smart pointer.
    SDL_Renderer *renderer;
    std::unordered_map<std::string, std::shared_ptr<SDL_Texture>> textures;

    std::shared_ptr<MIX_Mixer> mixer;
    std::unordered_map<std::string, std::shared_ptr<MIX_Track>> audio_tracks;

  public:
    Assets(SDL_Renderer *renderer, std::shared_ptr<MIX_Mixer> mixer);

    void load_png_texture(std::string name, std::string path);
    std::shared_ptr<SDL_Texture> get_texture(std::string name) const;

    void load_audio(std::string name, std::string path);
    void play_audio(std::string audio_name) const;
    void stop_audio(std::string audio_name) const;
};
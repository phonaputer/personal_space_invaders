#pragma once

#include "asset_manager.hpp"
#include "draw_util.hpp"
#include <SDL3/SDL.h>
#include <memory>

class Application {
  private:
    // Window and Renderer aren't unique ptrs since SDL cleans them up automatically on aplication exit.
    SDL_Window *window;
    SDL_Renderer *renderer;

    std::unique_ptr<AssetManager> asset_manager;
    std::unique_ptr<Animation> animation;
    Uint64 previous_now_ms;
    Uint64 unprocessed_ms;

  public:
    Application();
    void update();
    void draw();
};
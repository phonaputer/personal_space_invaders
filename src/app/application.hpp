#pragma once

#include "../util/draw_util.hpp"
#include "assets.hpp"
#include "scene.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <optional>

struct SceneHarness {
    std::unique_ptr<Scene> scene;
    std::shared_ptr<Assets> assets;
    std::shared_ptr<Entities> entities;
};

class Application {
  private:
    // Window and Renderer aren't unique ptrs since SDL cleans them up automatically on aplication exit.
    SDL_Window *window;
    SDL_Renderer *renderer;

    std::optional<SceneHarness> active_scene;

    Uint64 previous_now_ms;
    Uint64 unprocessed_ms;

  public:
    Application();
    void set_scene(std::unique_ptr<Scene> scene);
    void update();
    void draw();
};
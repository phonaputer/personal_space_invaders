#pragma once

#include "assets.hpp"
#include "collision.hpp"
#include "entity.hpp"
#include "input.hpp"
#include "scene.hpp"
#include "sprites.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <optional>

struct SceneHarness {
    std::unique_ptr<Scene> scene;
    std::unique_ptr<Assets> assets;
    std::unique_ptr<Entities> entities;
    std::unique_ptr<Collidables> collidables;
};

class Engine {
  private:
    // Window and Renderer aren't unique ptrs since SDL cleans them up automatically on aplication exit.
    SDL_Window *window;
    SDL_Renderer *renderer;

    std::unique_ptr<UserInputs> user_inputs;
    std::optional<SceneHarness> active_scene;

    Uint64 previous_now_ms;
    Uint64 unprocessed_ms;

  public:
    Engine();
    void set_scene(std::unique_ptr<Scene> scene);
    void update();
    void draw();
    UserInputs &get_user_inputs();
};
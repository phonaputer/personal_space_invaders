#include "engine.hpp"
#include "assets.hpp"
#include "core.hpp"
#include "input.hpp"
#include "sprites.hpp"
#include <SDL3/SDL.h>
#include <format>
#include <memory>
#include <stdexcept>

// Roughly 60 updates per second. 1000 / 60 = 16.66 (repeating, of course).
const Uint64 MS_PER_UPDATE = 17;

Engine::Engine() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error(std::format("Failed to initialize SDL: {}", SDL_GetError()));
  }

  window = SDL_CreateWindow("Personal Space Invaders", core::WINDOW_WIDTH, core::WINDOW_HEIGHT, 0);
  if (!window) {
    throw std::runtime_error(std::format("Couldn't create window: {}", SDL_GetError()));
  }

  renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) {
    throw std::runtime_error(std::format("Couldn't create renderer: {}", SDL_GetError()));
  }

  previous_now_ms = SDL_GetTicks();
  unprocessed_ms = 0;
  user_inputs = std::make_unique<UserInputs>();
}

void Engine::update() {
  if (!active_scene.has_value()) {
    return;
  }
  auto &scene = active_scene.value();

  const auto now_ms = SDL_GetTicks();
  unprocessed_ms += now_ms - previous_now_ms;
  previous_now_ms = now_ms;

  while (unprocessed_ms > MS_PER_UPDATE) {
    scene.entities->update(
        UpdateCtx{
            .assets = *scene.assets,
            .entities = *scene.entities,
            .user_inputs = *user_inputs,
        }
    );

    unprocessed_ms -= MS_PER_UPDATE;
  }
}

void Engine::draw() {
  if (!active_scene.has_value()) {
    return;
  }

  SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);
  SDL_RenderClear(renderer);

  active_scene.value().entities->draw(renderer);

  SDL_RenderPresent(renderer);
}

void Engine::set_scene(std::unique_ptr<Scene> scene) {
  auto assets = std::make_unique<Assets>(renderer);
  auto entities = std::make_unique<Entities>();

  scene->preload_assets(PreloadAssetsCtx{.assets = *assets});
  scene->initialize(
      SceneCtx{
          .assets = *assets,
          .entities = *entities,
          .user_inputs = *user_inputs,
      }
  );

  active_scene = SceneHarness{
      .scene = std::move(scene),
      .assets = std::move(assets),
      .entities = std::move(entities),
  };
};

UserInputs &Engine::get_user_inputs() {
  return *user_inputs;
}
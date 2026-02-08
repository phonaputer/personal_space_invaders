#include "engine.hpp"
#include "assets.hpp"
#include "sprites.hpp"
#include <SDL3/SDL.h>
#include <format>
#include <memory>
#include <stdexcept>

// Roughly 60 updates per second. 1000 / 60 = 16.66 (repeating, of course).
const Uint64 MS_PER_UPDATE = 17;

const int WINDOW_WIDTH = 1100;
const int WINDOW_HEIGHT = 800;

Engine::Engine() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error(std::format("Failed to initialize SDL: {}", SDL_GetError()));
  }

  window = SDL_CreateWindow("Personal Space Invaders", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
  if (!window) {
    throw std::runtime_error(std::format("Couldn't create window: {}", SDL_GetError()));
  }

  renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) {
    throw std::runtime_error(std::format("Couldn't create renderer: {}", SDL_GetError()));
  }

  previous_now_ms = SDL_GetTicks();
  unprocessed_ms = 0;
}

void Engine::update() {
  if (!active_scene.has_value()) {
    return;
  }

  const auto now_ms = SDL_GetTicks();
  unprocessed_ms += now_ms - previous_now_ms;
  previous_now_ms = now_ms;

  while (unprocessed_ms > MS_PER_UPDATE) {
    active_scene.value().entities->update();

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
  auto assets = std::make_shared<Assets>(renderer);
  auto entities = std::make_shared<Entities>();

  scene->preload_assets(std::make_unique<PreloadAssetsCtx>(PreloadAssetsCtx{.assets = assets}));
  scene->initialize(
      std::make_unique<SceneCtx>(SceneCtx{
          .assets = assets,
          .entities = entities,
      })
  );

  active_scene = SceneHarness{
      .scene = std::move(scene),
      .assets = assets,
      .entities = entities,
  };
};

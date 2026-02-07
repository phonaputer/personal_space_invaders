#include "application.hpp"
#include "../util/draw_util.hpp"
#include "asset_manager.hpp"
#include <SDL3/SDL.h>
#include <format>
#include <memory>
#include <stdexcept>

// Roughly 60 updates per second. 1000 / 60 = 16.66 (repeating, of course).
const Uint64 MS_PER_UPDATE = 17;

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 800;

Application::Application() {
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

  asset_manager = std::make_unique<AssetManager>(renderer);

  auto spritesheet = Spritesheet(asset_manager.get()->get_texture("space_invaders"), 16, 16);

  animation = std::make_unique<Animation>(spritesheet, 17, get_frames(0, 0, 4));

  previous_now_ms = SDL_GetTicks();
  unprocessed_ms = 0;
}

void Application::update() {
  const auto now_ms = SDL_GetTicks();
  unprocessed_ms += now_ms - previous_now_ms;
  previous_now_ms = now_ms;

  while (unprocessed_ms > MS_PER_UPDATE) {
    animation->update();

    unprocessed_ms -= MS_PER_UPDATE;
  }
}

void Application::draw() {
  SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);
  SDL_RenderClear(renderer);

  animation->draw(renderer, DrawRect{10, 10, 150, 150});

  SDL_RenderPresent(renderer);
}

#include "game.hpp"
#include "assets.hpp"
#include "core.hpp"
#include "input.hpp"
#include "sdl_util.hpp"
#include "sprites.hpp"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <format>
#include <memory>
#include <stdexcept>

// Roughly 60 updates per second. 1000 / 60 = 16.66 (repeating, of course).
const Uint64 MS_PER_UPDATE = 17;

const int ACTUAL_WINDOW_WIDTH = 880;
const int ACTUAL_WINDOW_HEIGHT = 640;

Game::Game() {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    throw std::runtime_error(std::format("Failed to initialize SDL: {}", SDL_GetError()));
  }

  window = SDL_CreateWindow(
      "Personal Space Invaders",
      ACTUAL_WINDOW_WIDTH,
      ACTUAL_WINDOW_HEIGHT,
      SDL_WINDOW_HIGH_PIXEL_DENSITY | SDL_WINDOW_RESIZABLE
  );
  if (!window) {
    throw std::runtime_error(std::format("Couldn't create window: {}", SDL_GetError()));
  }

  renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) {
    throw std::runtime_error(std::format("Couldn't create renderer: {}", SDL_GetError()));
  }

  SDL_SetRenderLogicalPresentation(
      renderer, core::WINDOW_WIDTH, core::WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX
  );

  if (!MIX_Init()) {
    throw std::runtime_error(std::format("Couldn't initialize sound mixer: {}", SDL_GetError()));
  }

  auto mixerP = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
  if (!mixerP) {
    throw std::runtime_error(std::format("Couldn't initialize mixer device: {}", SDL_GetError()));
  }
  mixer = std::shared_ptr<MIX_Mixer>(mixerP, SDLDeleter());

  previous_now_ms = SDL_GetTicks();
  unprocessed_ms = 0;
  user_inputs = std::make_unique<UserInputs>();
}

void Game::update() {
  if (!active_scene.has_value()) {
    return;
  }
  auto &scene = active_scene.value();

  const auto now_ms = SDL_GetTicks();
  unprocessed_ms += now_ms - previous_now_ms;
  previous_now_ms = now_ms;

  while (unprocessed_ms > MS_PER_UPDATE) {
    user_inputs->update();

    auto maybe_new_scene = scene.scene->update(
        SceneCtx{
            .assets = *scene.assets,
            .entities = *scene.entities,
            .user_inputs = *user_inputs,
        }
    );
    if (maybe_new_scene.has_value()) {
      unprocessed_ms = 0;
      set_scene(std::move(maybe_new_scene.value()));
      return;
    }

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

void Game::draw() {
  if (!active_scene.has_value()) {
    return;
  }

  SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);
  SDL_RenderClear(renderer);

  active_scene.value().entities->draw(renderer);

  SDL_RenderPresent(renderer);
}

void Game::set_scene(std::unique_ptr<Scene> scene) {
  auto assets = std::make_unique<Assets>(renderer, mixer);
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

UserInputs &Game::get_user_inputs() {
  return *user_inputs;
}
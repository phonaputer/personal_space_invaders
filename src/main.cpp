#define SDL_MAIN_USE_CALLBACKS

#include "engine/game.hpp"
#include "game/scenes/main_menu/main_menu_scene.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <cstdlib>
#include <ctime>
#include <memory>
#include <vector>

SDL_AppResult SDL_AppInit(void **appstate, [[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  std::srand(std::time(0));

  Game *game = nullptr;
  try {
    game = new Game();
    *appstate = game;
  } catch (const std::exception &e) {
    return SDL_APP_FAILURE;
  }

  game->set_scene(std::make_unique<MainMenuScene>());

  SDL_Log("Setup complete...");

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent([[maybe_unused]] void *appstate, SDL_Event *event) {
  auto game = (Game *)appstate;

  switch (event->type) {
    case SDL_EVENT_QUIT:
      return SDL_APP_SUCCESS;

    case SDL_EVENT_KEY_DOWN:
      switch (event->key.scancode) {
        case SDL_SCANCODE_A:
          game->get_user_inputs().engage(PlayerInput::LEFT);
          break;
        case SDL_SCANCODE_D:
          game->get_user_inputs().engage(PlayerInput::RIGHT);
          break;
        case SDL_SCANCODE_SPACE:
          game->get_user_inputs().engage(PlayerInput::FIRE);
          break;
        case SDL_SCANCODE_W:
          game->get_user_inputs().engage(PlayerInput::UP);
          break;
        case SDL_SCANCODE_S:
          game->get_user_inputs().engage(PlayerInput::DOWN);
          break;
        case SDL_SCANCODE_ESCAPE:
          game->get_user_inputs().engage(PlayerInput::PAUSE);
          break;
        default:
          // do nothing
      }
      break;

    case SDL_EVENT_KEY_UP:
      switch (event->key.scancode) {
        case SDL_SCANCODE_A:
          game->get_user_inputs().disengage(PlayerInput::LEFT);
          break;
        case SDL_SCANCODE_D:
          game->get_user_inputs().disengage(PlayerInput::RIGHT);
          break;
        case SDL_SCANCODE_SPACE:
          game->get_user_inputs().disengage(PlayerInput::FIRE);
          break;
        case SDL_SCANCODE_W:
          game->get_user_inputs().disengage(PlayerInput::UP);
          break;
        case SDL_SCANCODE_S:
          game->get_user_inputs().disengage(PlayerInput::DOWN);
          break;
        case SDL_SCANCODE_ESCAPE:
          game->get_user_inputs().disengage(PlayerInput::PAUSE);
          break;
        default:
          // do nothing
      }
      break;
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  auto game = (Game *)appstate;

  game->update();
  game->draw();

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, [[maybe_unused]] SDL_AppResult result) {
  auto game = (Game *)appstate;

  delete game;

  SDL_Log("Quitting...");
}

#define SDL_MAIN_USE_CALLBACKS

#include "engine/game.hpp"
#include "game/scenes/invasion/invasion_scene.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <memory>
#include <vector>

// ----[  TODO
// X * Add bad guys
// X * Have bad guys move around
// X * Add player character
// X * Have player character move in response to WASD
// X * Have player character shoot with Space
// X * Have bullets delete bad guys
// X * Have bad guys explode upon deletion
// X * Have bad guys shoot lazers
// X * Have lazers not damage bad guys
// X * Reset board upon destroying all bad guys
// X * Track score
// X * Have bad guy lazers damage player
// X * Pause enemies while player is blowing up
// X * Have a system of "lives" for the player, displaying in the UI
// X * Game over when all lives are lost
// ? * Can reset game after game over
// X * Sound effects
// * different sound when two bullets collide
// * BGM (space invaders style where it speeds up along with the aliens)
// * increate rate of bullet firing from aliens
// * Don't forget to seed standard library rand so aliens don't shoot the same each game
// * aliens get faster when less are remaining
// * clear bullets from screen when player dies
// * Start menu
// * Escape menu
// * Throw in the mystery ship for bonus points
// * Maybe some powerups?

SDL_AppResult SDL_AppInit(void **appstate, [[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {

  Game *game = nullptr;
  try {
    game = new Game();
    *appstate = game;
  } catch (const std::exception &e) {
    return SDL_APP_FAILURE;
  }

  game->set_scene(std::make_unique<InvasionScene>());

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

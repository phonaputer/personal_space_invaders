#define SDL_MAIN_USE_CALLBACKS

#include "app/application.hpp"
#include "test_scene.hpp"
#include "util/draw_util.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <memory>
#include <vector>

// ----[  TODO
// * Add bad guys
// * Have bad guys move around
// * Add player character
// * Have player character move in response to WASD
// * Have player character shoot with Space
// * Have bullets delete bad guys
// * Have bad guys explode upon deletion
// * Sound effects
// * BGM (space invaders style where it speeds up along with the aliens)
// * Have bad guys shoot lazers
// * Have lazers damage player
// * Have a system of "lives" for the player
// * Reset board upon destroying all bad guys
// * Track score
// * Throw in the mystery ship for bonus points
// * Maybe some powerups?

SDL_AppResult SDL_AppInit(void **appstate, [[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {

  Application *application = nullptr;
  try {
    application = new Application();
    *appstate = application;
  } catch (const std::exception &e) {
    return SDL_APP_FAILURE;
  }

  application->set_scene(std::make_unique<TestScene>());

  SDL_Log("Setup complete...");

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent([[maybe_unused]] void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  auto app = (Application *)appstate;

  app->update();
  app->draw();

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, [[maybe_unused]] SDL_AppResult result) {
  auto app = (Application *)appstate;

  delete app;

  SDL_Log("Quitting...");
}

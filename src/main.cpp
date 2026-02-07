#define SDL_MAIN_USE_CALLBACKS

#include "application.hpp"
#include "draw_util.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>

SDL_AppResult SDL_AppInit(void **appstate, [[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) {
  try {
    auto application = new Application();
    *appstate = application;
  } catch (const std::exception &e) {
    return SDL_APP_FAILURE;
  }

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

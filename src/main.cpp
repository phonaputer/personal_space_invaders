#define SDL_MAIN_USE_CALLBACKS

#include "util.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>

// Roughly 60 updates per second. 1000 / 60 = 16.66 (repeating, of course).
const Uint64 MS_PER_UPDATE = 17;

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 800;

struct AppCtx
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  Uint64 previous_now_ms;
  Uint64 unprocessed_ms;
  SDL_Texture *test_texture;
  Spritesheet *test_spritesheet;
  Animation *test_animation;
};

SDL_AppResult SDL_AppInit(void **appstate, [[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
  SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;
  if (!SDL_CreateWindowAndRenderer(
          "Test window", 640, 480, SDL_WINDOW_RESIZABLE, &window, &renderer
      ))
  {
    SDL_Log("Couldn't create window & renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_SetWindowSize(window, WINDOW_WIDTH, WINDOW_HEIGHT);
  SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

  SDL_SetRenderLogicalPresentation(
      renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX
  );

  auto png_surface = SDL_LoadPNG("./assets/space_invaders.png");
  if (!png_surface)
  {
    SDL_Log("Couldn't create png: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  auto png_texture = SDL_CreateTextureFromSurface(renderer, png_surface);
  if (!png_texture)
  {
    SDL_Log("Couldn't create png texture: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  SDL_DestroySurface(png_surface);

  SDL_SetTextureScaleMode(png_texture, SDL_SCALEMODE_PIXELART);

  Spritesheet *spritesheet = new Spritesheet(png_texture, 16, 16);
  Animation *animation = new Animation(spritesheet, 14, get_frames(0, 0, 3));

  *appstate = new AppCtx{
      .window = window,
      .renderer = renderer,
      .previous_now_ms = SDL_GetTicks(),
      .unprocessed_ms = 0,
      .test_texture = png_texture,
      .test_spritesheet = spritesheet,
      .test_animation = animation,
  };

  SDL_Log("Setup complete...");

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent([[maybe_unused]] void *appstate, SDL_Event *event)
{
  if (event->type == SDL_EVENT_QUIT)
  {
    return SDL_APP_SUCCESS;
  }

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
  auto ctx = (AppCtx *)appstate;

  const auto now_ms = SDL_GetTicks();
  ctx->unprocessed_ms += now_ms - ctx->previous_now_ms;
  ctx->previous_now_ms = now_ms;

  while (ctx->unprocessed_ms > MS_PER_UPDATE)
  {
    ctx->test_animation->update();

    ctx->unprocessed_ms -= MS_PER_UPDATE;
  }

  SDL_SetRenderDrawColorFloat(ctx->renderer, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);
  SDL_RenderClear(ctx->renderer);

  ctx->test_animation->draw(ctx->renderer, DrawRect{10, 10, 250, 250});

  SDL_RenderPresent(ctx->renderer);

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, [[maybe_unused]] SDL_AppResult result)
{
  auto ctx = (AppCtx *)appstate;

  SDL_DestroyTexture(ctx->test_texture);

  delete ctx->test_animation;
  delete ctx->test_spritesheet;
  delete ctx;

  SDL_Log("Quitting...");
}

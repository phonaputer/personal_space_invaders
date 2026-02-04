#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

struct SpritesheetArgs
{
  SDL_Texture *texture;

  float src_start_x;
  float src_start_y;
  float src_frame_width;
  float src_frame_height;

  const float draw_width;
  const float draw_height;

  int frame_count;
  int ticks_per_frame;
};

class Spritesheet
{
private:
  SDL_Texture *texture;

  const float src_start_x;
  const float src_start_y;
  const float src_frame_width;
  const float src_frame_height;

  const float draw_width;
  const float draw_height;

  const int frame_count;
  const int ticks_per_frame;

  int tick_counter = 0;
  int current_frame = 0;

public:
  Spritesheet(SpritesheetArgs args)
      : texture{args.texture},
        src_start_x{args.src_start_x},
        src_start_y{args.src_start_y},
        src_frame_width{args.src_frame_width},
        src_frame_height{args.src_frame_height},
        draw_width{args.draw_width},
        draw_height{args.draw_height},
        frame_count{args.frame_count},
        ticks_per_frame{args.ticks_per_frame}
  {
  }

  void update()
  {
    tick_counter++;

    if (tick_counter >= ticks_per_frame)
    {
      current_frame++;
      tick_counter = 0;

      if (current_frame >= frame_count)
      {
        current_frame = 0;
      }
    }
  }

  void draw(SDL_Renderer *renderer, float x, float y)
  {
    auto src_rect = SDL_FRect{
        src_start_x + (current_frame * src_frame_width),
        src_start_y,
        src_frame_width,
        src_frame_height,
    };
    auto dest_rect = SDL_FRect{
        x,
        y,
        x + draw_width,
        y + draw_height,
    };

    SDL_RenderTexture(renderer, texture, &src_rect, &dest_rect);
  }
};

// Roughly 60 updates per second. 1000 / 60 = 16.66 (repeating, of course).
const Uint64 MS_PER_UPDATE = 17;

struct AppCtx
{
  SDL_Window *window;
  SDL_Renderer *renderer;
  Uint64 previous_now_ms;
  Uint64 unprocessed_ms;
  SDL_Texture *test_texture;
  Spritesheet *test_spritesheet;
};

SDL_AppResult SDL_AppInit(
    void **appstate, [[maybe_unused]] int argc, [[maybe_unused]] char *argv[]
)
{
  SDL_SetAppMetadata(
      "Example Renderer Clear", "1.0", "com.example.renderer-clear"
  );

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

  SDL_SetRenderLogicalPresentation(
      renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX
  );

  auto png_surface = SDL_LoadPNG("./assets/knight.png");
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

  Spritesheet *spritesheet = new Spritesheet(
      SpritesheetArgs{
          .texture = png_texture,
          .src_start_x = 0,
          .src_start_y = 0,
          .src_frame_width = 32,
          .src_frame_height = 32,
          .draw_width = 128,
          .draw_height = 128,
          .frame_count = 4,
          .ticks_per_frame = 7,
      }
  );

  *appstate = new AppCtx{
      .window = window,
      .renderer = renderer,
      .previous_now_ms = SDL_GetTicks(),
      .unprocessed_ms = 0,
      .test_texture = png_texture,
      .test_spritesheet = spritesheet,
  };

  SDL_Log("Setup complete...");

  return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent([[maybe_unused]] void *appstate, SDL_Event *event)
{
  if (event->type == SDL_EVENT_QUIT)
  {
    return SDL_APP_SUCCESS;
  }

  return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
  auto ctx = (AppCtx *)appstate;

  const auto now_ms = SDL_GetTicks();
  ctx->unprocessed_ms += now_ms - ctx->previous_now_ms;
  ctx->previous_now_ms = now_ms;

  while (ctx->unprocessed_ms > MS_PER_UPDATE)
  {
    ctx->test_spritesheet->update();

    ctx->unprocessed_ms -= MS_PER_UPDATE;
  }

  const double now = ((double)SDL_GetTicks()) /
                     1000.0; /* convert from milliseconds to seconds. */
  /* choose the color for the frame we will draw. The sine wave trick makes it
   * fade between colors smoothly. */
  const float red = (float)(0.5 + 0.5 * SDL_sin(now));
  const float green = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
  const float blue = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));
  SDL_SetRenderDrawColorFloat(
      ctx->renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT
  ); /* new color, full alpha. */

  /* clear the window to the draw color. */
  SDL_RenderClear(ctx->renderer);

  ctx->test_spritesheet->draw(ctx->renderer, 10, 10);

  /* put the newly-cleared rendering on the screen. */
  SDL_RenderPresent(ctx->renderer);

  return SDL_APP_CONTINUE; /* carry on with the program! */
}

void SDL_AppQuit(void *appstate, [[maybe_unused]] SDL_AppResult result)
{
  auto ctx = (AppCtx *)appstate;

  SDL_DestroyTexture(ctx->test_texture);
  SDL_DestroyRenderer(ctx->renderer);
  SDL_DestroyWindow(ctx->window);

  delete ctx->test_spritesheet;
  delete ctx;

  SDL_Log("Quitting...");
  /* SDL will clean up the window/renderer for us. */
}

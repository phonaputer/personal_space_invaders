#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <vector>

struct Frame
{
  int x;
  int y;
};

std::vector<Frame> get_frames(const int y, const int start_x, const int end_x)
{
  std::vector<Frame> frames;

  for (int i = start_x; i < end_x; i++)
  {
    frames.push_back(Frame{.x = i, .y = y});
  }

  return frames;
}

struct DrawRect
{
  float x;
  float y;
  float width;
  float height;
};

class Spritesheet
{
private:
  SDL_Texture *src;
  const float src_frame_width;
  const float src_frame_height;

public:
  Spritesheet(SDL_Texture *src, float src_frame_width, float src_frame_height)
      : src{src},
        src_frame_width{src_frame_width},
        src_frame_height{src_frame_height}
  {
  }

  void draw_frame(
      SDL_Renderer *renderer, const Frame &frame, const DrawRect &draw_rect
  ) const
  {
    auto src_rect = SDL_FRect{
        (frame.x * src_frame_width),
        (frame.y * src_frame_height),
        src_frame_width,
        src_frame_height,
    };
    auto dest_rect = SDL_FRect{
        draw_rect.x,
        draw_rect.y,
        draw_rect.width,
        draw_rect.height,
    };

    SDL_RenderTexture(renderer, src, &src_rect, &dest_rect);
  }
};

class Animation
{
private:
  const Spritesheet *spritesheet;
  const int ticks_per_frame;
  const std::vector<Frame> frames;
  int tick_counter;
  size_t cur_frame;

public:
  Animation(
      Spritesheet *spritesheet, int ticks_per_frame, std::vector<Frame> frames
  )
      : spritesheet{spritesheet},
        ticks_per_frame{ticks_per_frame},
        frames{frames}
  {
    tick_counter = 0;
    cur_frame = 0;
  }

  void update()
  {
    tick_counter++;
    if (tick_counter >= ticks_per_frame)
    {
      tick_counter = 0;
      cur_frame++;

      if (cur_frame >= frames.size())
      {
        cur_frame = 0;
      }
    }
  }

  void draw(SDL_Renderer *renderer, const DrawRect &draw_rect) const
  {
    auto frame = frames.at(cur_frame);

    spritesheet->draw_frame(renderer, frame, draw_rect);
  }
};

// Roughly 60 updates per second. 1000 / 60 = 16.66 (repeating, of course).
const Uint64 MS_PER_UPDATE = 17;

const int WINDOW_WIDTH = 1200;
const int WINDOW_HEIGHT = 600;

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
      renderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX
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

  Spritesheet *spritesheet = new Spritesheet(png_texture, 32, 32);
  Animation *animation = new Animation(spritesheet, 7, get_frames(0, 0, 3));

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
    ctx->test_animation->update();

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

  ctx->test_animation->draw(ctx->renderer, DrawRect{10, 10, 500, 500});

  /* put the newly-cleared rendering on the screen. */
  SDL_RenderPresent(ctx->renderer);

  return SDL_APP_CONTINUE; /* carry on with the program! */
}

void SDL_AppQuit(void *appstate, [[maybe_unused]] SDL_AppResult result)
{
  auto ctx = (AppCtx *)appstate;

  SDL_DestroyTexture(ctx->test_texture);

  delete ctx->test_animation;
  delete ctx->test_spritesheet;
  delete ctx;

  SDL_Log("Quitting...");
  /* SDL will clean up the window/renderer for us. */
}

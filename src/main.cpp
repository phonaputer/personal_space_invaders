#define SDL_MAIN_USE_CALLBACKS

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

namespace
{
   // Roughly 60 updates per second. 1000 / 60 = 16.66 (repeating, of course).
   const Uint64 MS_PER_UPDATE = 17;

   struct AppCtx
   {
      SDL_Window *window;
      SDL_Renderer *renderer;
      Uint64 previous_now_ms;
      Uint64 unprocessed_ms;
      SDL_Texture *dat_png;
   };
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
   SDL_SetAppMetadata("Example Renderer Clear", "1.0", "com.example.renderer-clear");

   if (!SDL_Init(SDL_INIT_VIDEO))
   {
      SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
      return SDL_APP_FAILURE;
   }

   auto window = SDL_CreateWindow("Test window", 640, 480, SDL_WINDOW_RESIZABLE);
   if (!window)
   {
      SDL_Log("Couldn't create window: %s", SDL_GetError());
      return SDL_APP_FAILURE;
   }

   auto renderer = SDL_CreateRenderer(window, NULL);
   if (!renderer)
   {
      SDL_Log("Couldn't create renderer: %s", SDL_GetError());
      return SDL_APP_FAILURE;
   }

   SDL_SetRenderLogicalPresentation(renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);

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

   *appstate = new AppCtx{
       .window = window,
       .renderer = renderer,
       .previous_now_ms = SDL_GetTicks(),
       .unprocessed_ms = 0,
       .dat_png = png_texture,
   };

   SDL_Log("Setup complete...");

   return SDL_APP_CONTINUE;
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
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
      // Don't actually have anything to do here now :/

      ctx->unprocessed_ms -= MS_PER_UPDATE;
   }

   const double now = ((double)SDL_GetTicks()) / 1000.0; /* convert from milliseconds to seconds. */
   /* choose the color for the frame we will draw. The sine wave trick makes it fade between colors smoothly. */
   const float red = (float)(0.5 + 0.5 * SDL_sin(now));
   const float green = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
   const float blue = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));
   SDL_SetRenderDrawColorFloat(ctx->renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT); /* new color, full alpha. */

   /* clear the window to the draw color. */
   SDL_RenderClear(ctx->renderer);

   auto src_rect = SDL_FRect{0, 0, 32, 32};
   auto dest_rect = SDL_FRect{10, 10, 128, 128};

   SDL_RenderTexture(ctx->renderer, ctx->dat_png, &src_rect, &dest_rect);

   /* put the newly-cleared rendering on the screen. */
   SDL_RenderPresent(ctx->renderer);

   return SDL_APP_CONTINUE; /* carry on with the program! */
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
   auto ctx = (AppCtx *)appstate;

   SDL_DestroyTexture(ctx->dat_png);

   SDL_Log("Quitting...");
   /* SDL will clean up the window/renderer for us. */
}

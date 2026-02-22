#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

struct SDLDeleter {
    void operator()(SDL_Texture *p) const {
      SDL_DestroyTexture(p);
    }

    void operator()(SDL_Surface *p) const {
      SDL_DestroySurface(p);
    }

    void operator()(MIX_Mixer *p) const {
      MIX_DestroyMixer(p);
    }

    void operator()(MIX_Track *p) const {
      MIX_DestroyTrack(p);
    }

    void operator()(MIX_Audio *p) const {
      MIX_DestroyAudio(p);
    }
};
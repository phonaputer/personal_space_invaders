#pragma once

#include "core.hpp"
#include "sdl_util.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

struct Frame {
    int x;
    int y;
};

class Spritesheet {
  private:
    std::shared_ptr<SDL_Texture> src;
    const float src_frame_width;
    const float src_frame_height;

  public:
    Spritesheet(std::shared_ptr<SDL_Texture> src, float src_frame_width, float src_frame_height);
    void draw_frame(SDL_Renderer *renderer, const Frame &frame, const core::Rect &draw_rect) const;
};

class Animation {
  private:
    const Spritesheet spritesheet;
    const int ticks_per_frame;
    const std::vector<Frame> frames;
    int tick_counter;
    size_t cur_frame;

  public:
    Animation(Spritesheet spritesheet, int ticks_per_frame, std::vector<Frame> frames);
    void update();
    void update_backwards();
    void next_frame();
    void draw(SDL_Renderer *renderer, const core::Rect &draw_rect) const;
};

class OnceAnimation {
  private:
    const Spritesheet spritesheet;
    const int ticks_per_frame;
    const std::vector<Frame> frames;
    int tick_counter;
    size_t cur_frame;
    bool playing;

  public:
    OnceAnimation(Spritesheet spritesheet, int ticks_per_frame, std::vector<Frame> frames);
    void play();
    void update();
    void draw(SDL_Renderer *renderer, const core::Rect &draw_rect) const;
};
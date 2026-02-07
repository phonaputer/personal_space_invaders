#pragma once

#include <SDL3/SDL.h>
#include <vector>

struct Frame
{
  int x;
  int y;
};

std::vector<Frame> get_frames(const int y, const int start_x, const int end_x);

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
  Spritesheet(SDL_Texture *src, float src_frame_width, float src_frame_height);
  void draw_frame(SDL_Renderer *renderer, const Frame &frame, const DrawRect &draw_rect) const;
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
  Animation(Spritesheet *spritesheet, int ticks_per_frame, std::vector<Frame> frames);
  void update();
  void draw(SDL_Renderer *renderer, const DrawRect &draw_rect) const;
};
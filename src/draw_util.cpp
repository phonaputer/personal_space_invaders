#include "draw_util.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

std::vector<Frame> get_frames(const int y, const int start_x, const int end_x)
{
  std::vector<Frame> frames;

  for (int i = start_x; i < end_x; i++)
  {
    frames.push_back(Frame{.x = i, .y = y});
  }

  return frames;
}

Spritesheet::Spritesheet(std::shared_ptr<SDL_Texture> src, float src_frame_width, float src_frame_height)
    : src{src}, src_frame_width{src_frame_width}, src_frame_height{src_frame_height}
{
}

void Spritesheet::draw_frame(SDL_Renderer *renderer, const Frame &frame, const DrawRect &draw_rect) const
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

  SDL_RenderTexture(renderer, src.get(), &src_rect, &dest_rect);
}

Animation::Animation(Spritesheet spritesheet, int ticks_per_frame, std::vector<Frame> frames)
    : spritesheet{spritesheet}, ticks_per_frame{ticks_per_frame}, frames{frames}
{
  tick_counter = 0;
  cur_frame = 0;
}

void Animation::update()
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

void Animation::draw(SDL_Renderer *renderer, const DrawRect &draw_rect) const
{
  auto frame = frames.at(cur_frame);

  spritesheet.draw_frame(renderer, frame, draw_rect);
}
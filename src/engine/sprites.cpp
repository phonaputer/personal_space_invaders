#include "sprites.hpp"
#include "core.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

Spritesheet::Spritesheet(std::shared_ptr<SDL_Texture> src, float src_frame_width, float src_frame_height)
    : src{src},
      src_frame_width{src_frame_width},
      src_frame_height{src_frame_height} {
}

void Spritesheet::draw_frame(SDL_Renderer *renderer, const Frame &frame, const core::Rect &draw_rect) const {
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
    : spritesheet{spritesheet},
      ticks_per_frame{ticks_per_frame},
      frames{frames} {
  tick_counter = 0;
  cur_frame = 0;
}

void Animation::update() {
  tick_counter++;
  if (tick_counter >= ticks_per_frame) {
    tick_counter = 0;
    cur_frame++;

    if (cur_frame >= frames.size()) {
      cur_frame = 0;
    }
  }
}

void Animation::update_backwards() {
  tick_counter++;
  if (tick_counter >= ticks_per_frame) {
    tick_counter = 0;

    if (cur_frame <= 0) {
      cur_frame = frames.size() - 1;
    } else {
      cur_frame--;
    }
  }
}

void Animation::next_frame() {
  cur_frame++;

  if (cur_frame >= frames.size()) {
    cur_frame = 0;
  }
}

void Animation::rewind() {
  cur_frame = 0;
  tick_counter = 0;
}

void Animation::draw(SDL_Renderer *renderer, const core::Rect &draw_rect) const {
  auto frame = frames.at(cur_frame);

  spritesheet.draw_frame(renderer, frame, draw_rect);
}

OnceAnimation::OnceAnimation(Spritesheet spritesheet, int ticks_per_frame, std::vector<Frame> frames)
    : spritesheet{spritesheet},
      ticks_per_frame{ticks_per_frame},
      frames{frames},
      tick_counter{0},
      cur_frame{0},
      playing{false} {
}

void OnceAnimation::update() {
  if (!playing) {
    return;
  }

  tick_counter++;
  if (tick_counter >= ticks_per_frame) {
    tick_counter = 0;
    cur_frame++;

    if (cur_frame >= frames.size()) {
      cur_frame = 0;
      playing = false;
    }
  }
}

void OnceAnimation::play() {
  playing = true;
  cur_frame = 0;
}

void OnceAnimation::stop() {
  cur_frame = 0;
  playing = false;
}

void OnceAnimation::draw(SDL_Renderer *renderer, const core::Rect &draw_rect) const {
  if (!playing) {
    return;
  }

  auto frame = frames.at(cur_frame);

  spritesheet.draw_frame(renderer, frame, draw_rect);
}
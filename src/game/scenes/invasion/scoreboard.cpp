#include "scoreboard.hpp"
#include "engine/core.hpp"
#include "engine/sprites.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <string>
#include <unordered_map>

std::unordered_map<char, Frame> chars_to_frames({
    {'1', {0, 8}},  {'2', {1, 8}},  {'3', {2, 8}},  {'4', {3, 8}},  {'5', {4, 8}},  {'6', {5, 8}},
    {'7', {6, 8}},  {'8', {7, 8}},  {'9', {8, 8}},  {'0', {9, 8}},  {'a', {0, 9}},  {'b', {1, 9}},
    {'c', {2, 9}},  {'d', {3, 9}},  {'e', {4, 9}},  {'f', {5, 9}},  {'g', {6, 9}},  {'h', {7, 9}},
    {'i', {8, 9}},  {'j', {9, 9}},  {'k', {10, 9}}, {'l', {11, 9}}, {'m', {12, 9}}, {'n', {13, 9}},
    {'o', {14, 9}}, {'p', {15, 9}}, {'q', {0, 10}}, {'r', {1, 10}}, {'s', {2, 10}}, {'t', {3, 10}},
    {'u', {4, 10}}, {'v', {5, 10}}, {'w', {6, 10}}, {'x', {7, 10}}, {'y', {8, 10}}, {'z', {9, 10}},
});

TextRenderer::TextRenderer(std::shared_ptr<SDL_Texture> texture)
    : spritesheet(texture, 8, 8) {
}

void TextRenderer::render_text(SDL_Renderer *renderer, core::Point location, std::string text) {
  float cur_x = location.x;

  for (char c : text) {
    if (chars_to_frames.contains(c)) {
      spritesheet.draw_frame(
          renderer, chars_to_frames.at(c), {cur_x, location.y, CHARACTER_DRAW_WIDTH, CHARACTER_DRAW_HEIGHT}
      );

      cur_x += CHARACTER_X_SPACING;
    }
  }
}
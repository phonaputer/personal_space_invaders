#include "scoreboard.hpp"
#include "engine/core.hpp"
#include "engine/sprites.hpp"
#include "invasion_constants.hpp"
#include <SDL3/SDL.h>
#include <cctype>
#include <format>
#include <memory>
#include <string>
#include <unordered_map>

std::unordered_map<char, Frame> chars_to_frames({
    {'1', {0, 8}},  {'2', {1, 8}},   {'3', {2, 8}},   {'4', {3, 8}},   {'5', {4, 8}},   {'6', {5, 8}},  {'7', {6, 8}},
    {'8', {7, 8}},  {'9', {8, 8}},   {'0', {9, 8}},   {'a', {0, 9}},   {'b', {1, 9}},   {'c', {2, 9}},  {'d', {3, 9}},
    {'e', {4, 9}},  {'f', {5, 9}},   {'g', {6, 9}},   {'h', {7, 9}},   {'i', {8, 9}},   {'j', {9, 9}},  {'k', {10, 9}},
    {'l', {11, 9}}, {'m', {12, 9}},  {'n', {13, 9}},  {'o', {14, 9}},  {'p', {15, 9}},  {'q', {0, 10}}, {'r', {1, 10}},
    {'s', {2, 10}}, {'t', {3, 10}},  {'u', {4, 10}},  {'v', {5, 10}},  {'w', {6, 10}},  {'x', {7, 10}}, {'y', {8, 10}},
    {'z', {9, 10}}, {'-', {10, 10}}, {':', {11, 10}}, {'<', {12, 10}}, {'>', {13, 10}},
});

TextRenderer::TextRenderer(std::shared_ptr<SDL_Texture> texture)
    : spritesheet(texture, 8, 8) {
}

void TextRenderer::render_text(SDL_Renderer *renderer, core::Point location, std::string text) const {
  float cur_x = location.x;

  for (char c : text) {
    char c_normalized = std::tolower(c);

    if (chars_to_frames.contains(c_normalized)) {
      spritesheet.draw_frame(
          renderer, chars_to_frames.at(c_normalized), {cur_x, location.y, CHARACTER_DRAW_WIDTH, CHARACTER_DRAW_HEIGHT}
      );
    }

    cur_x += CHARACTER_X_SPACING;
  }
}

Scoreboard::Scoreboard(std::shared_ptr<SDL_Texture> texture, core::Point position)
    : text_renderer(texture),
      x{position.x},
      y{position.y},
      high_score{0},
      current_score{0},
      current_lives{0},
      player_is_dead{false} {
}

std::string Scoreboard::get_type() const {
  return entityType::SCOREBOARD;
}

void Scoreboard::notify_player_scored(unsigned int amount) {
  current_score += amount;
  if (current_score > high_score) {
    high_score = current_score;
  }
}

void Scoreboard::notify_player_died(int remaining_lives) {
  current_lives = remaining_lives;
  player_is_dead = true;
}

void Scoreboard::notify_player_rerack(int remaining_lives) {
  if (current_lives < 1) {
    current_score = 0;
  }

  current_lives = remaining_lives;
  player_is_dead = false;
}

std::optional<std::reference_wrapper<Drawable>> Scoreboard::as_drawable() {
  return *this;
}

void Scoreboard::draw(SDL_Renderer *renderer) const {
  text_renderer.render_text(renderer, {x, y}, std::format("score:{}", current_score));
  text_renderer.render_text(renderer, {core::WINDOW_WIDTH / 2 - 110, y}, std::format("hi-score:{}", high_score));
  text_renderer.render_text(renderer, {core::WINDOW_WIDTH - 165, y}, std::format("lives:{}", current_lives));

  if (player_is_dead && current_lives < 1) {
    text_renderer.render_text(renderer, {core::WINDOW_WIDTH / 2 - 100, core::WINDOW_HEIGHT / 2}, "game over");
  }
}
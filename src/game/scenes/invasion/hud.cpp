#include "hud.hpp"
#include "engine/core.hpp"
#include "engine/text_renderer.hpp"
#include "invasion_constants.hpp"
#include <SDL3/SDL.h>
#include <format>
#include <memory>

HUD::HUD(std::shared_ptr<SDL_Texture> texture)
    : text_renderer(texture),
      high_score{0},
      current_score{0},
      current_lives{0},
      game_is_over{false} {
}

std::shared_ptr<HUD> HUD::create(SceneCtx ctx) {
  auto result = std::shared_ptr<HUD>(new HUD(ctx.assets.get_texture(image::PRIMARY_SPRITESHEET)));

  ctx.entities.add_drawable(result);

  return result;
}

void HUD::notify_score_change(int new_current_score, int new_high_score) {
  current_score = new_current_score;
  high_score = new_high_score;
}

void HUD::notify_player_lives_change(int new_current_lives) {
  current_lives = new_current_lives;
}

void HUD::notify_game_start() {
  game_is_over = false;
}

void HUD::notify_game_over() {
  game_is_over = true;
}

bool HUD::is_deleted() const {
  return false;
}

unsigned int HUD::get_z() const {
  return zindex::HUD;
}

void HUD::draw(SDL_Renderer *renderer) const {
  text_renderer.render_text(renderer, {10, 10}, std::format("score:{}", current_score));
  text_renderer.render_text(renderer, {core::WINDOW_WIDTH / 2 - 110, 10}, std::format("hi-score:{}", high_score));
  text_renderer.render_text(renderer, {core::WINDOW_WIDTH - 165, 10}, std::format("lives:{}", current_lives));

  if (game_is_over) {
    text_renderer.render_text(renderer, {core::WINDOW_WIDTH / 2 - 100, core::WINDOW_HEIGHT / 2}, "game over");
  }
}
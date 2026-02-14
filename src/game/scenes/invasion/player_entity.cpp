#include "player_entity.hpp"
#include "engine/core.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

PlayerLazerEntity::PlayerLazerEntity(std::shared_ptr<SDL_Texture> texture, core::Point starting_position)
    : spritesheet(texture, 16, 16),
      x{starting_position.x},
      y{starting_position.y},
      draw_width{60},
      draw_height{60},
      speed{5} {
}

void PlayerLazerEntity::update([[maybe_unused]] UpdateCtx const &ctx) {
  y -= speed;
}

void PlayerLazerEntity::draw(SDL_Renderer *renderer) {
  spritesheet.draw_frame(renderer, {0, 6}, {x, y, draw_width, draw_height});
}

bool PlayerLazerEntity::is_deleted() {
  return y <= 0;
}

PlayerEntity::PlayerEntity(std::shared_ptr<SDL_Texture> texture, core::Point starting_position)
    : x{starting_position.x}, y{starting_position.y}, shot_clock{0} {
  std::vector<Frame> frames = {{0, 5}, {1, 5}, {2, 5}};
  animation = std::make_unique<Animation>(Spritesheet(texture, 16, 16), 5, frames);

  std::vector<Frame> muzzle_flash_frames = {{3, 5}, {4, 5}};
  muzzle_flash_animation = std::make_unique<OnceAnimation>(Spritesheet(texture, 16, 16), 10, muzzle_flash_frames);
}

void PlayerEntity::update(UpdateCtx const &ctx) {
  if (ctx.user_inputs.is_engaged(PlayerInput::LEFT)) {
    x -= SPEED;
    animation->update();
  } else if (ctx.user_inputs.is_engaged(PlayerInput::RIGHT)) {
    x += SPEED;
    animation->update_backwards();
  }

  shot_clock++;
  if (ctx.user_inputs.is_engaged(PlayerInput::FIRE) && shot_clock >= TICKS_PER_SHOT) {
    shot_clock = 0;
    muzzle_flash_animation->play();
    ctx.entities.add(
        std::make_unique<PlayerLazerEntity>(ctx.assets.get_texture("space_invaders"), core::Point{x - 7, y - 20})
    );
  }
  muzzle_flash_animation->update();
}

void PlayerEntity::draw(SDL_Renderer *renderer) {
  core::Rect rect = {x, y, DRAW_WIDTH, DRAW_HEIGHT};
  animation->draw(renderer, rect);
  muzzle_flash_animation->draw(renderer, rect);
}
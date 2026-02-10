#include "player_entity.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include "invader_entities.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

PlayerLazerEntity::PlayerLazerEntity(std::shared_ptr<SDL_Texture> texture, Position starting_position)
    : spritesheet(texture, 16, 16),
      x{starting_position.x},
      y{starting_position.y},
      draw_width{60},
      draw_height{60},
      speed{5} {
}

void PlayerLazerEntity::update() {
  y -= speed;
}

void PlayerLazerEntity::draw(SDL_Renderer *renderer) {
  spritesheet.draw_frame(renderer, {0, 6}, {x, y, draw_width, draw_height});
}

bool PlayerLazerEntity::is_deleted() {
  return y <= 0;
}

PlayerEntity::PlayerEntity(SceneCtx ctx, std::shared_ptr<SDL_Texture> texture, Position starting_position)
    : ctx{ctx},
      texture{texture},
      x{starting_position.x},
      y{starting_position.y},
      draw_width{60},
      draw_height{60},
      speed{4},
      ticks_per_shot{50},
      shot_clock{0} {
  std::vector<Frame> frames = {{0, 5}, {1, 5}, {2, 5}};
  animation = std::make_unique<Animation>(Spritesheet(texture, 16, 16), 5, frames);

  std::vector<Frame> muzzle_flash_frames = {{3, 5}, {4, 5}};
  muzzle_flash_animation = std::make_unique<OnceAnimation>(Spritesheet(texture, 16, 16), 10, muzzle_flash_frames);
}

void PlayerEntity::update() {
  if (ctx.user_inputs.is_engaged(PlayerInput::LEFT)) {
    x -= speed;
    animation->update();
  } else if (ctx.user_inputs.is_engaged(PlayerInput::RIGHT)) {
    x += speed;
    animation->update_backwards();
  }

  shot_clock++;
  if (ctx.user_inputs.is_engaged(PlayerInput::FIRE) && shot_clock >= ticks_per_shot) {
    shot_clock = 0;
    muzzle_flash_animation->play();
    ctx.entities.add(std::make_unique<PlayerLazerEntity>(texture, Position{x - 7, y - 20}));
  }
  muzzle_flash_animation->update();
}

void PlayerEntity::draw(SDL_Renderer *renderer) {
  DrawRect rect = {x, y, draw_width, draw_height};
  animation->draw(renderer, rect);
  muzzle_flash_animation->draw(renderer, rect);
}
#include "player_entity.hpp"
#include "engine/core.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include "invader_entities.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

PlayerLazerEntity::PlayerLazerEntity(std::shared_ptr<SDL_Texture> texture, core::Point starting_position)
    : spritesheet(texture, 16, 16), x{starting_position.x}, y{starting_position.y} {
}

void PlayerLazerEntity::update([[maybe_unused]] UpdateCtx const &ctx) {
  y -= SPEED;
}

void PlayerLazerEntity::draw(SDL_Renderer *renderer) const {
  spritesheet.draw_frame(renderer, {0, 6}, {x, y, DRAW_WIDTH, DRAW_HEIGHT});

#ifndef NDEBUG
  auto hitbox = get_hitbox();
  auto sdl_hitbox = SDL_FRect{hitbox.x, hitbox.y, hitbox.width, hitbox.height};
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
  SDL_RenderRect(renderer, &sdl_hitbox);
#endif
}

bool PlayerLazerEntity::is_deleted() const {
  return y <= 0;
}

core::Rect PlayerLazerEntity::get_hitbox() const {
  return {
      .x = x + 25,
      .y = y + 25,
      .width = 6,
      .height = 12,
  };
}

CollideAction PlayerLazerEntity::get_collide_action() {
  return CollideAction::DAMAGE;
}

std::optional<std::reference_wrapper<Collidable>> PlayerLazerEntity::as_collidable() {
  return std::ref<Collidable>(*this);
}

std::optional<std::reference_wrapper<Drawable>> PlayerLazerEntity::as_drawable() {
  return std::ref<Drawable>(*this);
}

std::optional<std::reference_wrapper<Updateable>> PlayerLazerEntity::as_updateable() {
  return std::ref<Updateable>(*this);
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

void PlayerEntity::draw(SDL_Renderer *renderer) const {
  core::Rect rect = {x, y, DRAW_WIDTH, DRAW_HEIGHT};
  animation->draw(renderer, rect);
  muzzle_flash_animation->draw(renderer, rect);

#ifndef NDEBUG
  auto hitbox = get_hitbox();
  auto sdl_hitbox = SDL_FRect{hitbox.x, hitbox.y, hitbox.width, hitbox.height};
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
  SDL_RenderRect(renderer, &sdl_hitbox);
#endif
}

core::Rect PlayerEntity::get_hitbox() const {
  return {
      .x = x,
      .y = y + 23,
      .width = DRAW_WIDTH - 2,
      .height = DRAW_HEIGHT - 38,
  };
}

std::optional<std::reference_wrapper<Collidable>> PlayerEntity::as_collidable() {
  return std::ref<Collidable>(*this);
}

std::optional<std::reference_wrapper<Drawable>> PlayerEntity::as_drawable() {
  return std::ref<Drawable>(*this);
}

std::optional<std::reference_wrapper<Updateable>> PlayerEntity::as_updateable() {
  return std::ref<Updateable>(*this);
}
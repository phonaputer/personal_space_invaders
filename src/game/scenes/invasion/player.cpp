#include "player.hpp"
#include "engine/core.hpp"
#include "engine/input.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include "invasion_constants.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

PlayerProjectile::PlayerProjectile(std::shared_ptr<SDL_Texture> texture, core::Point starting_position)
    : spritesheet(texture, 16, 16),
      x{starting_position.x},
      y{starting_position.y},
      deleted{false} {
}

std::string PlayerProjectile::get_type() const {
  return entityType::PLAYER_PROJECTILE;
}

void PlayerProjectile::update([[maybe_unused]] UpdateCtx const &ctx) {
  y -= SPEED;
}

void PlayerProjectile::draw(SDL_Renderer *renderer) const {
  spritesheet.draw_frame(renderer, {7, 1}, {x, y, DRAW_WIDTH, DRAW_HEIGHT});

#ifndef NDEBUG
  auto hitbox = get_hitbox();
  auto sdl_hitbox = SDL_FRect{hitbox.x, hitbox.y, hitbox.width, hitbox.height};
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
  SDL_RenderRect(renderer, &sdl_hitbox);
#endif
}

bool PlayerProjectile::is_deleted() const {
  return deleted == true || y <= 0;
}

core::Rect PlayerProjectile::get_hitbox() const {
  return {
      .x = x + 25,
      .y = y + 25,
      .width = 6,
      .height = 12,
  };
}

void PlayerProjectile::collide_with([[maybe_unused]] CollideCtx const &ctx, Collidable &other) {
  if (other.get_type() == entityType::ALIEN || other.get_type() == entityType::ALIEN_PROJECTILE) {
    deleted = true;
  }
}

std::optional<std::reference_wrapper<Collidable>> PlayerProjectile::as_collidable() {
  return std::ref<Collidable>(*this);
}

std::optional<std::reference_wrapper<Drawable>> PlayerProjectile::as_drawable() {
  return std::ref<Drawable>(*this);
}

std::optional<std::reference_wrapper<Updateable>> PlayerProjectile::as_updateable() {
  return std::ref<Updateable>(*this);
}

Player::Player(std::shared_ptr<SDL_Texture> texture, core::Point starting_position)
    : starting_position{starting_position},
      x{starting_position.x},
      y{starting_position.y},
      shot_clock{0},
      exploding{false},
      explosion_clock{0} {
  std::vector<Frame> frames = {{0, 2}, {1, 2}, {2, 2}};
  animation = std::make_unique<Animation>(Spritesheet(texture, 16, 16), 5, frames);

  std::vector<Frame> muzzle_flash_frames = {{3, 2}, {4, 2}};
  muzzle_flash_animation = std::make_unique<OnceAnimation>(Spritesheet(texture, 16, 16), 10, muzzle_flash_frames);

  std::vector<Frame> explosion_frames = {{0, 3}, {1, 3}, {2, 3}, {1, 3}};
  explosion_animation = std::make_unique<Animation>(Spritesheet(texture, 16, 16), 6, explosion_frames);
}

std::string Player::get_type() const {
  return entityType::PLAYER;
}

void Player::rerack() {
  exploding = false;
  x = starting_position.x;
  y = starting_position.y;
  animation->rewind();
  explosion_animation->rewind();
}

void Player::update(UpdateCtx const &ctx) {
  if (exploding) {
    if (explosion_clock >= EXPLOSION_TICKS) {
      explosion_clock = 0;
      rerack();
      return;
    }
    explosion_clock++;
    explosion_animation->update();
    return;
  }

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
        std::make_unique<PlayerProjectile>(
            ctx.assets.get_texture(asset::PRIMARY_SPRITESHEET), core::Point{x - 7, y - 20}
        )
    );
  }
  muzzle_flash_animation->update();
}

void Player::draw(SDL_Renderer *renderer) const {
  core::Rect rect = {x, y, DRAW_WIDTH, DRAW_HEIGHT};

  if (exploding) {
    explosion_animation->draw(renderer, rect);
    return;
  }

  animation->draw(renderer, rect);
  muzzle_flash_animation->draw(renderer, rect);

#ifndef NDEBUG
  auto hitbox = get_hitbox();
  auto sdl_hitbox = SDL_FRect{hitbox.x, hitbox.y, hitbox.width, hitbox.height};
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
  SDL_RenderRect(renderer, &sdl_hitbox);
#endif
}

core::Rect Player::get_hitbox() const {
  return {
      .x = x,
      .y = y + 23,
      .width = DRAW_WIDTH - 2,
      .height = DRAW_HEIGHT - 38,
  };
}

void Player::collide_with([[maybe_unused]] CollideCtx const &ctx, Collidable &other) {
  if (other.get_type() == entityType::ALIEN_PROJECTILE) {
    exploding = true;
  }
}

std::optional<std::reference_wrapper<Collidable>> Player::as_collidable() {
  return std::ref<Collidable>(*this);
}

std::optional<std::reference_wrapper<Drawable>> Player::as_drawable() {
  return std::ref<Drawable>(*this);
}

std::optional<std::reference_wrapper<Updateable>> Player::as_updateable() {
  return std::ref<Updateable>(*this);
}
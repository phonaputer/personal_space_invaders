#include "player.hpp"
#include "engine/core.hpp"
#include "engine/input.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include "invasion_constants.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

PlayerProjectile::PlayerProjectile(std::shared_ptr<SDL_Texture> texture)
    : spritesheet(texture, 16, 16) {
}

std::shared_ptr<PlayerProjectile> PlayerProjectile::create(SceneCtx ctx) {
  auto result =
      std::shared_ptr<PlayerProjectile>(new PlayerProjectile(ctx.assets.get_texture(image::PRIMARY_SPRITESHEET)));

  ctx.entities.add_collidable(result);
  ctx.entities.add_drawable(result);

  return result;
}

void PlayerProjectile::shoot_from(core::Point position) {
  active = true;
  x = position.x;
  y = position.y;
}

bool PlayerProjectile::is_active() const {
  return active;
}

void PlayerProjectile::deactivate() {
  active = false;
}

bool PlayerProjectile::is_deleted() const {
  return false;
}

std::string PlayerProjectile::get_type() const {
  return entityType::PLAYER_PROJECTILE;
}

void PlayerProjectile::update() {
  if (!is_active()) {
    return;
  }

  y -= SPEED;

  if (y <= 0) {
    active = false;
  }
}

unsigned int PlayerProjectile::get_z() const {
  return zindex::PLAYER_PROJECTILE;
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

core::Rect PlayerProjectile::get_hitbox() const {
  return {
      .x = x + 25,
      .y = y + 25,
      .width = 6,
      .height = 12,
  };
}

void PlayerProjectile::collide_with([[maybe_unused]] CollideCtx ctx, Collidable &other) {
  if (other.get_type() == entityType::ALIEN || other.get_type() == entityType::ALIEN_PROJECTILE ||
      other.get_type() == entityType::FORTRESS_TILE) {
    active = false;
  }
}

Player::Player(SceneCtx ctx, core::Point starting_position)
    : ctx{ctx},
      starting_position{starting_position},
      x{starting_position.x},
      y{starting_position.y} {
  auto texture = ctx.assets.get_texture(image::PRIMARY_SPRITESHEET);

  std::vector<Frame> frames = {{0, 2}, {1, 2}, {2, 2}};
  animation = std::make_unique<Animation>(Spritesheet(texture, 16, 16), 5, frames);

  std::vector<Frame> muzzle_flash_frames = {{3, 2}, {4, 2}};
  muzzle_flash_animation = std::make_unique<OnceAnimation>(Spritesheet(texture, 16, 16), 10, muzzle_flash_frames);

  std::vector<Frame> explosion_frames = {{0, 3}, {1, 3}, {2, 3}, {1, 3}};
  explosion_animation = std::make_unique<Animation>(Spritesheet(texture, 16, 16), 6, explosion_frames);

  projectile = PlayerProjectile::create(ctx);
}

std::shared_ptr<Player> Player::create(SceneCtx ctx, core::Point starting_position) {
  auto result = std::shared_ptr<Player>(new Player(ctx, starting_position));

  ctx.entities.add_collidable(result);
  ctx.entities.add_drawable(result);

  return result;
}

std::string Player::get_type() const {
  return entityType::PLAYER;
}

void Player::add_notifier(std::weak_ptr<PlayerDeathNotifier> notifier) {
  status_notifiers.push_back(notifier);
}

void Player::update() {
  if (am_dead) {
    explosion_animation->update();
    return;
  }

  if (paused) {
    return;
  }

  projectile->update();

  if (ctx.user_inputs.is_engaged(PlayerInput::LEFT)) {
    x -= SPEED;
    animation->update();
  } else if (ctx.user_inputs.is_engaged(PlayerInput::RIGHT)) {
    x += SPEED;
    animation->update_backwards();
  }

  shot_clock++;
  if (ctx.user_inputs.is_engaged(PlayerInput::FIRE) && !projectile->is_active() && shot_clock >= TICKS_PER_SHOT) {
    shot_clock = 0;
    muzzle_flash_animation->play();
    projectile->shoot_from({x - 7, y - 20});
    ctx.assets.play_audio(sound::PLAYER_SHOT);
  }
  muzzle_flash_animation->update();
}

void Player::notify_player_rerack() {
  am_dead = false;
  x = starting_position.x;
  y = starting_position.y;
  animation->rewind();
  explosion_animation->rewind();
  ctx.assets.stop_audio(sound::PLAYER_EXPLOSION);
}

void Player::notify_game_over() {
  paused = true;
}

void Player::notify_game_start() {
  paused = false;
}

bool Player::is_deleted() const {
  return false;
}

unsigned int Player::get_z() const {
  return zindex::PLAYER;
}

void Player::draw(SDL_Renderer *renderer) const {
  core::Rect rect = {x, y, DRAW_WIDTH, DRAW_HEIGHT};

  if (am_dead) {
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

void Player::collide_with([[maybe_unused]] CollideCtx ctx, Collidable &other) {
  if (other.get_type() == entityType::ALIEN_PROJECTILE) {
    am_dead = true;
    ctx.assets.play_audio(sound::PLAYER_EXPLOSION);
    muzzle_flash_animation->stop();

    for (const auto &notifier : status_notifiers) {
      notifier.lock()->notify_player_died();
      projectile->deactivate();
    }
  }
}
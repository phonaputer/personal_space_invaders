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

std::shared_ptr<PlayerProjectile> PlayerProjectile::create(SceneCtx ctx, core::Point starting_position) {
  auto result = std::shared_ptr<PlayerProjectile>(
      new PlayerProjectile(ctx.assets.get_texture(image::PRIMARY_SPRITESHEET), starting_position)
  );

  ctx.entities.add_collidable(result);
  ctx.entities.add_drawable(result);

  return result;
}

std::string PlayerProjectile::get_type() const {
  return entityType::PLAYER_PROJECTILE;
}

void PlayerProjectile::update() {
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

void PlayerProjectile::mark_deleted() {
  deleted = true;
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
  if (other.get_type() == entityType::ALIEN || other.get_type() == entityType::ALIEN_PROJECTILE) {
    deleted = true;
  }
}

void PlayerProjectileOrchestrator::add(std::shared_ptr<PlayerProjectile> projectile) {
  projectiles.push_back(projectile);
}

void PlayerProjectileOrchestrator::update() {
  for (const auto &projectile : projectiles) {
    projectile->update();
  }

  std::erase_if(projectiles, [](std::shared_ptr<PlayerProjectile> const &p) { return p->is_deleted(); });
}

void PlayerProjectileOrchestrator::delete_all() {
  for (const auto &projectile : projectiles) {
    projectile->mark_deleted();
  }
}

Player::Player(SceneCtx ctx, core::Point starting_position)
    : ctx{ctx},
      starting_position{starting_position},
      x{starting_position.x},
      y{starting_position.y},
      shot_clock{0} {
  auto texture = ctx.assets.get_texture(image::PRIMARY_SPRITESHEET);

  std::vector<Frame> frames = {{0, 2}, {1, 2}, {2, 2}};
  animation = std::make_unique<Animation>(Spritesheet(texture, 16, 16), 5, frames);

  std::vector<Frame> muzzle_flash_frames = {{3, 2}, {4, 2}};
  muzzle_flash_animation = std::make_unique<OnceAnimation>(Spritesheet(texture, 16, 16), 10, muzzle_flash_frames);

  std::vector<Frame> explosion_frames = {{0, 3}, {1, 3}, {2, 3}, {1, 3}};
  explosion_animation = std::make_unique<Animation>(Spritesheet(texture, 16, 16), 6, explosion_frames);
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

  projectiles.update();

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
    projectiles.add(PlayerProjectile::create(ctx, {x - 7, y - 20}));
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

bool Player::is_deleted() const {
  return false;
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
      projectiles.delete_all();
    }
  }
}
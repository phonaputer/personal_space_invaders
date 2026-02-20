#include "invaders.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include "invasion_constants.hpp"
#include <SDL3/SDL.h>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

AlienProjectile::AlienProjectile(std::shared_ptr<SDL_Texture> texture, core::Point starting_position)
    : x{starting_position.x},
      y{starting_position.y},
      deleted{false} {
  std::vector<Frame> frames = {{5, 2}, {6, 2}};
  animation = std::make_unique<Animation>(Spritesheet(texture, 16, 16), 10, frames);
}

std::string AlienProjectile::get_type() const {
  return entityType::ALIEN_PROJECTILE;
}

void AlienProjectile::update([[maybe_unused]] UpdateCtx const &ctx) {
  y += SPEED;
  animation->update();
}

void AlienProjectile::draw(SDL_Renderer *renderer) const {
  animation->draw(renderer, {x, y, DRAW_WIDTH, DRAW_HEIGHT});

#ifndef NDEBUG
  auto hitbox = get_hitbox();
  auto sdl_hitbox = SDL_FRect{hitbox.x, hitbox.y, hitbox.width, hitbox.height};
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
  SDL_RenderRect(renderer, &sdl_hitbox);
#endif
}

bool AlienProjectile::is_deleted() const {
  return deleted == true || y >= core::WINDOW_HEIGHT;
}

core::Rect AlienProjectile::get_hitbox() const {
  return {
      .x = x + 22,
      .y = y + 13,
      .width = 12,
      .height = 24,
  };
}

void AlienProjectile::collide_with([[maybe_unused]] CollideCtx const &ctx, Collidable &other) {
  if (other.get_type() == entityType::PLAYER) {
    deleted = true;
  }

  if (other.get_type() == entityType::PLAYER_PROJECTILE) {
    deleted = true;
    ctx.entities.add(
        std::make_shared<AlienExplosion>(ctx.assets.get_texture(asset::PRIMARY_SPRITESHEET), core::Point{x, y})
    );
  }
}

std::optional<std::reference_wrapper<Collidable>> AlienProjectile::as_collidable() {
  return std::ref<Collidable>(*this);
}

std::optional<std::reference_wrapper<Drawable>> AlienProjectile::as_drawable() {
  return std::ref<Drawable>(*this);
}

std::optional<std::reference_wrapper<Updateable>> AlienProjectile::as_updateable() {
  return std::ref<Updateable>(*this);
}

AlienExplosion::AlienExplosion(std::shared_ptr<SDL_Texture> texture, core::Point position)
    : tick_counter{0},
      x{position.x},
      y{position.y} {
  std::vector<Frame> frames = {{3, 1}, {4, 1}, {5, 1}, {6, 1}};
  animation = std::make_unique<Animation>(Spritesheet(texture, 16, 16), 5, frames);
}

std::string AlienExplosion::get_type() const {
  return entityType::ALIEN_EXPLOSION;
}

void AlienExplosion::draw(SDL_Renderer *renderer) const {
  animation->draw(renderer, {x, y, DRAW_WIDTH, DRAW_HEIGHT});
}

bool AlienExplosion::is_deleted() const {
  return tick_counter >= LIFETIME_TICKS;
}

std::optional<std::reference_wrapper<Drawable>> AlienExplosion::as_drawable() {
  return std::ref<Drawable>(*this);
}

void AlienExplosion::update([[maybe_unused]] UpdateCtx const &ctx) {
  tick_counter++;
  animation->update();
}

std::optional<std::reference_wrapper<Updateable>> AlienExplosion::as_updateable() {
  return std::ref<Updateable>(*this);
}

AlienFactory::AlienFactory(
    SceneCtx ctx, std::shared_ptr<SDL_Texture> texture, std::shared_ptr<ScoreNotifier> score_notifier
)
    : ctx{ctx},
      texture{texture},
      score_notifier{score_notifier} {
}

std::shared_ptr<Alien> AlienFactory::new_jellyfish(core::Point starting_position) {
  auto entity = std::make_shared<Alien>(AlienParams{
      .texture = texture,
      .starting_position = starting_position,
      .frames = {{1, 0}, {0, 0}, {1, 0}, {2, 0}},
      .hitbox = {5, 12, 50, 35},
      .score_notifier = score_notifier,
      .score = 30,
  });

  ctx.entities.add(entity);

  return entity;
}

std::shared_ptr<Alien> AlienFactory::new_tadpole(core::Point starting_position) {
  auto entity = std::make_shared<Alien>(AlienParams{
      .texture = texture,
      .starting_position = starting_position,
      .frames = {{4, 0}, {3, 0}, {4, 0}, {5, 0}},
      .hitbox = {17, 11, 23, 35},
      .score_notifier = score_notifier,
      .score = 10,
  });

  ctx.entities.add(entity);

  return entity;
}

std::shared_ptr<Alien> AlienFactory::new_octopus(core::Point starting_position) {
  auto entity = std::make_shared<Alien>(AlienParams{
      .texture = texture,
      .starting_position = starting_position,
      .frames = {{6, 0}, {7, 0}},
      .hitbox = {0, 15, 60, 32},
      .score_notifier = score_notifier,
      .score = 40,
  });

  ctx.entities.add(entity);

  return entity;
}

std::shared_ptr<Alien> AlienFactory::new_crab(core::Point starting_position) {
  auto entity = std::make_shared<Alien>(AlienParams{
      .texture = texture,
      .starting_position = starting_position,
      .frames = {{1, 1}, {0, 1}, {1, 1}, {2, 1}},
      .hitbox = {5, 14, 50, 31},
      .score_notifier = score_notifier,
      .score = 20,
  });

  ctx.entities.add(entity);

  return entity;
}

Alien::Alien(AlienParams params)
    : starting_position{params.starting_position},
      x{params.starting_position.x},
      y{params.starting_position.y},
      move_right{true},
      hitbox{params.hitbox},
      deactivated{false},
      score_notifier{params.score_notifier},
      score{params.score} {
  animation = std::make_unique<Animation>(Spritesheet(params.texture, 16, 16), 17, params.frames);
}

std::string Alien::get_type() const {
  return entityType::ALIEN;
}

void Alien::move(float speed) {
  animation->next_frame();

  if (move_right) {
    x += speed;
  } else {
    x -= speed;
  }
}

void Alien::draw(SDL_Renderer *renderer) const {
  if (deactivated) {
    return;
  }

  animation->draw(renderer, {x, y, DRAW_WIDTH, DRAW_HEIGHT});

#ifndef NDEBUG
  auto hitbox = get_hitbox();
  auto sdl_hitbox = SDL_FRect{hitbox.x, hitbox.y, hitbox.width, hitbox.height};
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
  SDL_RenderRect(renderer, &sdl_hitbox);
#endif
}

void Alien::descend_and_turn(float descend_speed) {
  move_right = !move_right;
  y += descend_speed;
}

bool Alien::has_reached_edge() const {
  return x <= 60 || x + DRAW_WIDTH >= core::WINDOW_WIDTH - 60;
}

void Alien::rerack() {
  x = starting_position.x;
  y = starting_position.y;
  move_right = true;
  deactivated = false;
}

core::Point Alien::get_position() const {
  return {x, y};
}

bool Alien::is_active() const {
  return !deactivated;
}

void Alien::collide_with(CollideCtx const &ctx, Collidable &other) {
  if (other.get_type() == entityType::PLAYER_PROJECTILE) {
    deactivated = true;
    ctx.entities.add(
        std::make_shared<AlienExplosion>(ctx.assets.get_texture(asset::PRIMARY_SPRITESHEET), core::Point{x, y})
    );
    score_notifier->notify_player_scored(score);
  }
}

core::Rect Alien::get_hitbox() const {
  return {
      .x = x + hitbox.x,
      .y = y + hitbox.y,
      .width = hitbox.width,
      .height = hitbox.height,
  };
};

std::optional<std::reference_wrapper<Drawable>> Alien::as_drawable() {
  return std::ref<Drawable>(*this);
}

std::optional<std::reference_wrapper<Collidable>> Alien::as_collidable() {
  if (deactivated) {
    return std::nullopt;
  }

  return std::ref<Collidable>(*this);
}

AlienOrchestrator::AlienOrchestrator()
    : tick_counter{0},
      is_player_dead{false},
      player_lives{1000} {
}

std::string AlienOrchestrator::get_type() const {
  return entityType::ALIEN_ORCHESTRATOR;
}

void AlienOrchestrator::add_alien(std::shared_ptr<Alien> alien) {
  aliens.push_back(alien);
}

void AlienOrchestrator::notify_player_died(int remaining_lives) {
  is_player_dead = true;
  player_lives = remaining_lives;
}

void AlienOrchestrator::notify_player_rerack(int remaining_lives) {
  is_player_dead = false;
  if (remaining_lives > player_lives) {
    for (auto &alien : aliens) {
      alien->rerack();
    }
  }
  player_lives = remaining_lives;
}

void AlienOrchestrator::update([[maybe_unused]] UpdateCtx const &ctx) {
  if (is_player_dead) {
    return;
  }

  tick_counter++;
  if (tick_counter < TICKS_PER_MOVE) {
    return;
  }

  tick_counter = 0;

  for (auto &alien : aliens) {
    if (alien->is_active() && alien->has_reached_edge()) {
      for (auto &alien : aliens) {
        alien->descend_and_turn(60);
      }
      break;
    }
  }

  for (auto &alien : aliens) {
    alien->move(15);
  }

  std::vector<std::shared_ptr<Alien>> active_aliens;
  for (const auto &a : aliens) {
    if (a->is_active()) {
      active_aliens.push_back(a);
    }
  }

  if (active_aliens.size() < 1) {
    for (auto &alien : aliens) {
      alien->rerack();
    }
  }

  if (active_aliens.size() > 0 && rand() % ALIEN_SHOOT_CHANCE == 0) {
    size_t selected_alien = rand() % active_aliens.size();
    auto position = active_aliens.at(selected_alien)->get_position();
    ctx.entities.add(
        std::make_shared<AlienProjectile>(
            ctx.assets.get_texture(asset::PRIMARY_SPRITESHEET), core::Point{position.x, position.y + 30}
        )
    );
  }
}

std::optional<std::reference_wrapper<Updateable>> AlienOrchestrator::as_updateable() {
  return std::ref<Updateable>(*this);
}
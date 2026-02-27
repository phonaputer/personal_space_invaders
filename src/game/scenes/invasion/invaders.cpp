#include "invaders.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include "invasion_constants.hpp"
#include <SDL3/SDL.h>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

AlienProjectile::AlienProjectile(
    std::shared_ptr<SDL_Texture> texture,
    core::Point starting_position,
    std::shared_ptr<AlienExplosionOrchestrator> explosions
)
    : x{starting_position.x},
      y{starting_position.y},
      deleted{false},
      explosions{explosions} {
  std::vector<Frame> frames = {{5, 2}, {6, 2}};
  animation = std::make_unique<Animation>(Spritesheet(texture, 16, 16), 10, frames);
}

std::shared_ptr<AlienProjectile> AlienProjectile::create(
    SceneCtx ctx, core::Point starting_position, std::shared_ptr<AlienExplosionOrchestrator> explosions
) {
  auto result = std::shared_ptr<AlienProjectile>(
      new AlienProjectile(ctx.assets.get_texture(image::PRIMARY_SPRITESHEET), starting_position, explosions)
  );

  ctx.entities.add_collidable(result);
  ctx.entities.add_drawable(result);

  return result;
}

std::string AlienProjectile::get_type() const {
  return entityType::ALIEN_PROJECTILE;
}

void AlienProjectile::update() {
  y += SPEED;
  animation->update();
}

unsigned int AlienProjectile::get_z() const {
  return zindex::ALIEN_PROJECTILE;
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
  return deleted == true || y + 40 >= GROUND_Y;
}

void AlienProjectile::mark_deleted() {
  deleted = true;
}

core::Rect AlienProjectile::get_hitbox() const {
  return {
      .x = x + 22,
      .y = y + 13,
      .width = 12,
      .height = 24,
  };
}

void AlienProjectile::collide_with(CollideCtx ctx, Collidable &other) {
  if (other.get_type() == entityType::PLAYER) {
    deleted = true;
  }

  if (other.get_type() == entityType::PLAYER_PROJECTILE) {
    deleted = true;
    explosions->add(AlienExplosion::create(ctx.assets, ctx.entities, {x, y}));
    ctx.assets.play_audio(sound::ALIEN_EXPLOSION);
  }

  if (other.get_type() == entityType::FORTRESS_TILE) {
    deleted = true;
  }
}

AlienExplosion::AlienExplosion(std::shared_ptr<SDL_Texture> texture, core::Point position)
    : tick_counter{0},
      x{position.x},
      y{position.y} {
  std::vector<Frame> frames = {{3, 1}, {4, 1}, {5, 1}, {6, 1}};
  animation = std::make_unique<Animation>(Spritesheet(texture, 16, 16), 5, frames);
}

std::shared_ptr<AlienExplosion>
AlienExplosion::create(Assets const &assets, EntityAdder &entities, core::Point position) {
  auto result =
      std::shared_ptr<AlienExplosion>(new AlienExplosion(assets.get_texture(image::PRIMARY_SPRITESHEET), position));

  entities.add_drawable(result);

  return result;
}

unsigned int AlienExplosion::get_z() const {
  return zindex::ALIEN_EXPLOSION;
}

void AlienExplosion::draw(SDL_Renderer *renderer) const {
  animation->draw(renderer, {x, y, DRAW_WIDTH, DRAW_HEIGHT});
}

bool AlienExplosion::is_deleted() const {
  return tick_counter >= LIFETIME_TICKS;
}

void AlienExplosion::update() {
  tick_counter++;
  animation->update();
}

AlienFactory::AlienFactory(
    SceneCtx ctx, std::weak_ptr<ScoreNotifier> score_notifier, std::shared_ptr<AlienExplosionOrchestrator> explosions
)
    : ctx{ctx},
      score_notifier{score_notifier},
      explosions{explosions} {
}

std::shared_ptr<Alien> AlienFactory::new_jellyfish(core::Point starting_position) {
  auto entity = std::make_shared<Alien>(AlienParams{
      .texture = ctx.assets.get_texture(image::PRIMARY_SPRITESHEET),
      .starting_position = starting_position,
      .frames = {{1, 0}, {0, 0}, {1, 0}, {2, 0}},
      .hitbox = {5, 12, 50, 35},
      .score_notifier = score_notifier,
      .score = 30,
      .explosions = explosions,
  });

  ctx.entities.add_drawable(entity);
  ctx.entities.add_collidable(entity);

  return entity;
}

std::shared_ptr<Alien> AlienFactory::new_tadpole(core::Point starting_position) {
  auto entity = std::make_shared<Alien>(AlienParams{
      .texture = ctx.assets.get_texture(image::PRIMARY_SPRITESHEET),
      .starting_position = starting_position,
      .frames = {{4, 0}, {3, 0}, {4, 0}, {5, 0}},
      .hitbox = {17, 11, 23, 35},
      .score_notifier = score_notifier,
      .score = 10,
      .explosions = explosions,
  });

  ctx.entities.add_drawable(entity);
  ctx.entities.add_collidable(entity);

  return entity;
}

std::shared_ptr<Alien> AlienFactory::new_octopus(core::Point starting_position) {
  auto entity = std::make_shared<Alien>(AlienParams{
      .texture = ctx.assets.get_texture(image::PRIMARY_SPRITESHEET),
      .starting_position = starting_position,
      .frames = {{6, 0}, {7, 0}},
      .hitbox = {0, 15, 60, 32},
      .score_notifier = score_notifier,
      .score = 40,
      .explosions = explosions,
  });

  ctx.entities.add_drawable(entity);
  ctx.entities.add_collidable(entity);

  return entity;
}

std::shared_ptr<Alien> AlienFactory::new_crab(core::Point starting_position) {
  auto entity = std::make_shared<Alien>(AlienParams{
      .texture = ctx.assets.get_texture(image::PRIMARY_SPRITESHEET),
      .starting_position = starting_position,
      .frames = {{1, 1}, {0, 1}, {1, 1}, {2, 1}},
      .hitbox = {5, 14, 50, 31},
      .score_notifier = score_notifier,
      .score = 20,
      .explosions = explosions,
  });

  ctx.entities.add_drawable(entity);
  ctx.entities.add_collidable(entity);

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
      score{params.score},
      explosions{params.explosions} {
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

unsigned int Alien::get_z() const {
  return zindex::ALIEN;
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
  return x <= 30 || x + DRAW_WIDTH >= core::WINDOW_WIDTH - 30;
}

bool Alien::has_reached_ground() const {
  return !deactivated && y + DRAW_HEIGHT >= GROUND_Y;
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

bool Alien::is_deleted() const {
  return false;
}

void Alien::collide_with(CollideCtx ctx, Collidable &other) {
  if (other.get_type() == entityType::PLAYER_PROJECTILE) {
    deactivated = true;
    explosions->add(AlienExplosion::create(ctx.assets, ctx.entities, {x, y}));
    ctx.assets.play_audio(sound::ALIEN_EXPLOSION);
    score_notifier.lock()->notify_scored(score);
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

void AlienProjectileOrchestrator::add(std::shared_ptr<AlienProjectile> projectile) {
  projectiles.push_back(projectile);
}

void AlienProjectileOrchestrator::update() {
  for (const auto &projectile : projectiles) {
    projectile->update();
  }

  std::erase_if(projectiles, [](std::shared_ptr<AlienProjectile> const &p) { return p->is_deleted(); });
}

void AlienProjectileOrchestrator::delete_all() {
  for (const auto &projectile : projectiles) {
    projectile->mark_deleted();
  }
}

void AlienExplosionOrchestrator::add(std::shared_ptr<AlienExplosion> explosion) {
  explosions.push_back(explosion);
}

void AlienExplosionOrchestrator::update() {
  for (const auto &explosion : explosions) {
    explosion->update();
  }

  std::erase_if(explosions, [](std::shared_ptr<AlienExplosion> const &e) { return e->is_deleted(); });
}

void BGMOrchestrator::play(SceneCtx const &ctx) {
  switch (cur_arp_idx) {
    case 0:
      ctx.assets.play_audio(sound::ARP_1);
      cur_arp_idx++;
      break;
    case 1:
      ctx.assets.play_audio(sound::ARP_2);
      cur_arp_idx++;
      break;
    case 2:
      ctx.assets.play_audio(sound::ARP_3);
      cur_arp_idx++;
      break;
    default:
      ctx.assets.play_audio(sound::ARP_4);
      cur_arp_idx = 0;
      break;
  }
}

void BGMOrchestrator::reset() {
  cur_arp_idx = 0;
}

AlienOrchestrator::AlienOrchestrator(std::shared_ptr<AlienExplosionOrchestrator> explosions)
    : explosions{explosions},
      tick_counter{0},
      shot_tick_counter{0},
      paused{false} {
}

void AlienOrchestrator::add_alien(std::shared_ptr<Alien> alien) {
  aliens.push_back(alien);
}

void AlienOrchestrator::update(SceneCtx ctx) {
  explosions->update();

  if (paused) {
    return;
  }

  projectiles.update();

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
    return;
  }

  shot_tick_counter++;
  if (shot_tick_counter >= TICKS_PER_SHOOT_CHANCE) {
    shot_tick_counter = 0;
    if (active_aliens.size() > 0 && rand() % ALIEN_SHOOT_CHANCE == 0) {
      size_t selected_alien = rand() % active_aliens.size();
      auto position = active_aliens.at(selected_alien)->get_position();
      projectiles.add(AlienProjectile::create(ctx, {position.x, position.y + 30}, explosions));
      ctx.assets.play_audio(sound::ALIEN_SHOT);
    }
  }

  int adjusted_ticks_per_move = TICKS_PER_MOVE - (aliens.size() - active_aliens.size());
  tick_counter++;
  if (tick_counter < adjusted_ticks_per_move) {
    return;
  }

  tick_counter = 0;

  bool descend_and_turn = false;
  for (auto &active_alien : active_aliens) {
    if (active_alien->has_reached_edge()) {
      descend_and_turn = true;
      break;
    }
  }

  for (auto &alien : aliens) {
    if (descend_and_turn) {
      alien->descend_and_turn(Y_SPEED);
    }
    alien->move(X_SPEED);
  }

  bgm.play(ctx);

  for (auto &alien : active_aliens) {
    if (alien->has_reached_ground()) {
      for (const auto &notifier : overrun_notifiers) {
        notifier.lock()->notify_aliens_overran_earth();
      }
      break;
    }
  }
}

void AlienOrchestrator::add_overrun_notifier(std::weak_ptr<AlienOverrunNotifier> notifier) {
  overrun_notifiers.push_back(notifier);
}

void AlienOrchestrator::notify_player_died() {
  paused = true;
  projectiles.delete_all();
}

void AlienOrchestrator::notify_player_rerack() {
  paused = false;
}

void AlienOrchestrator::notify_game_over() {
  paused = true;
  projectiles.delete_all();
}

void AlienOrchestrator::notify_game_start() {
  for (auto &alien : aliens) {
    alien->rerack();
  }
  bgm.reset();
  tick_counter = 0;
  shot_tick_counter = 0;
}
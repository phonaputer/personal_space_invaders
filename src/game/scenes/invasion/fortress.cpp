#include "fortress.hpp"
#include "engine/core.hpp"
#include "engine/entity.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include "game_state.hpp"
#include "invasion_constants.hpp"
#include <memory>
#include <vector>

FortressTile::FortressTile(core::Point position, Spritesheet spritesheet, Frame start_frame, core::Rect hitbox)
    : position{position},
      spritesheet{spritesheet},
      start_frame{start_frame},
      hitbox{hitbox} {
}

std::shared_ptr<FortressTile>
FortressTile::create(SceneCtx ctx, core::Point position, Frame start_frame, core::Rect hitbox) {
  auto result = std::shared_ptr<FortressTile>(new FortressTile(
      position, Spritesheet(ctx.assets.get_texture(image::PRIMARY_SPRITESHEET), 4, 4), start_frame, hitbox
  ));

  ctx.entities.add_collidable(result);
  ctx.entities.add_drawable(result);

  return result;
}

std::string FortressTile::get_type() const {
  return entityType::FORTRESS_TILE;
}

core::Rect FortressTile::get_hitbox() const {
  return hitbox;
}

void FortressTile::collide_with([[maybe_unused]] CollideCtx ctx, [[maybe_unused]] Collidable &other) {
  if (other.get_type() == entityType::ALIEN_PROJECTILE || other.get_type() == entityType::PLAYER_PROJECTILE) {
    if (health > 0) {
      health--;
    }
  }
}

unsigned int FortressTile::get_z() const {
  return zindex::FORTRESS;
}

void FortressTile::draw(SDL_Renderer *renderer) const {
  if (health < 1) {
    return;
  }

  int frame_x_offset = (MAX_HEALTH - health) * 6;

  spritesheet.draw_frame(
      renderer,
      {
          start_frame.x + frame_x_offset,
          start_frame.y,
      },
      {
          position.x,
          position.y,
          DRAW_WIDTH,
          DRAW_HEIGHT,
      }
  );

#ifndef NDEBUG
  auto hitbox = get_hitbox();
  auto sdl_hitbox = SDL_FRect{hitbox.x, hitbox.y, hitbox.width, hitbox.height};
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
  SDL_RenderRect(renderer, &sdl_hitbox);
#endif
}

bool FortressTile::is_deleted() const {
  return false;
}

bool FortressTile::is_active() const {
  return health > 0;
}

void FortressTile::rerack() {
  health = MAX_HEALTH;
}

Fortress::Fortress(SceneCtx ctx, core::Point position) {
  for (int col = 0; col < 6; col++) {
    float x = position.x + col * FortressTile::DRAW_WIDTH;

    for (int row = 0; row < 4; row++) {
      if (row == 3 && (col == 2 || col == 3)) {
        continue;
      }

      float y = position.y + row * FortressTile::DRAW_HEIGHT;

      tiles.push_back(
          FortressTile::create(
              ctx, {x, y}, {12 + col, 12 + row}, {x, y, FortressTile::DRAW_WIDTH, FortressTile::DRAW_HEIGHT}
          )
      );
    }
  }
}

std::shared_ptr<Fortress> Fortress::create(SceneCtx ctx, core::Point position) {
  return std::shared_ptr<Fortress>(new Fortress(ctx, position));
}

void Fortress::notify_game_start() {
  for (auto &tile : tiles) {
    tile->rerack();
  }
}

std::shared_ptr<Ground> Ground::create(SceneCtx ctx) {
  auto result = std::make_shared<Ground>();

  ctx.entities.add_drawable(result);

  return result;
}

unsigned int Ground::get_z() const {
  return zindex::GROUND;
}

void Ground::draw(SDL_Renderer *renderer) const {
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_RenderLine(renderer, 0, GROUND_Y, core::WINDOW_WIDTH, GROUND_Y);
}

bool Ground::is_deleted() const {
  return false;
}

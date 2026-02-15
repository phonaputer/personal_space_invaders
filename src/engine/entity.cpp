#include "entity.hpp"
#include <SDL3/SDL.h>
#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

void Entities::add(std::shared_ptr<Entity> entity) {
  entities_to_add.push_back(entity);
}

void Entities::update(UpdateCtx const &ctx) {
  for (size_t i = 0; i < entities_to_add.size(); i++) {
    entities.push_back(std::move(entities_to_add.at(i)));
  }
  entities_to_add.clear();

  for (const auto &e : entities) {
    auto maybe_updateable = e->as_updateable();
    if (maybe_updateable) {
      maybe_updateable.value().get().update(ctx);
    }
  }

  check_collisions({
      .assets = ctx.assets,
      .entities = ctx.entities,
  });

  std::erase_if(entities, [](std::shared_ptr<Entity> const &e) { return e->is_deleted(); });
}

void Entities::draw(SDL_Renderer *renderer) {
  for (const auto &e : entities) {
    auto maybe_drawable = e->as_drawable();
    if (maybe_drawable) {
      maybe_drawable.value().get().draw(renderer);
    }
  }
}

bool are_touching(core::Rect a, core::Rect b) {
  return !(a.y + a.height < b.y || a.x + a.width < b.x || a.y > b.y + b.height || a.x > b.x + b.width);
}

void Entities::check_collisions(CollideCtx const &ctx) {
  std::vector<std::reference_wrapper<Collidable>> collidables;

  for (const auto &e : entities) {
    auto maybe_collidable = e->as_collidable();
    if (maybe_collidable) {
      collidables.push_back(maybe_collidable.value());
    };
  }

  size_t num_collidables = collidables.size();
  for (size_t i = 0; i < num_collidables - 1; i++) {
    auto &a = collidables.at(i).get();

    for (size_t j = i + 1; j < num_collidables; j++) {
      auto &b = collidables.at(j).get();

      if (are_touching(a.get_hitbox(), b.get_hitbox())) {
        a.receive_collision(ctx, b.get_collide_action());
        b.receive_collision(ctx, a.get_collide_action());
      }
    }
  }
}
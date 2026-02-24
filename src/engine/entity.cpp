#include "entity.hpp"
#include "scene.hpp"
#include <SDL3/SDL.h>
#include <algorithm>
#include <functional>
#include <memory>
#include <vector>

void Entities::add_drawable(std::shared_ptr<Drawable> entity) {
  drawables_to_add.push_back(entity);
}
void Entities::add_collidable(std::shared_ptr<Collidable> entity) {
  collidables_to_add.push_back(entity);
}

void Entities::update(UpdateCtx const &ctx) {
  for (size_t i = 0; i < drawables_to_add.size(); i++) {
    drawables.push_back(drawables_to_add.at(i));
  }
  drawables_to_add.clear();

  for (size_t i = 0; i < collidables_to_add.size(); i++) {
    collidables.push_back(collidables_to_add.at(i));
  }
  collidables_to_add.clear();

  check_collisions({
      .assets = ctx.assets,
      .entities = ctx.entities,
  });

  std::erase_if(collidables, [](std::shared_ptr<Collidable> const &e) { return e->is_deleted(); });
  std::erase_if(drawables, [](std::shared_ptr<Drawable> const &e) { return e->is_deleted(); });
}

void Entities::draw(SDL_Renderer *renderer) {
  for (const auto &e : drawables) {
    e->draw(renderer);
  }
}

bool are_touching(core::Rect a, core::Rect b) {
  return !(a.y + a.height < b.y || a.x + a.width < b.x || a.y > b.y + b.height || a.x > b.x + b.width);
}

void Entities::check_collisions(CollideCtx const &ctx) {
  std::vector<std::shared_ptr<Collidable>> active_collidables;
  for (const auto &e : collidables) {
    if (e->is_active()) {
      active_collidables.push_back(e);
    }
  }

  size_t num_collidables = active_collidables.size();

  for (size_t i = 0; i < num_collidables - 1; i++) {
    const auto &a = active_collidables.at(i);

    for (size_t j = i + 1; j < num_collidables; j++) {
      const auto &b = active_collidables.at(j);

      if (are_touching(a->get_hitbox(), b->get_hitbox())) {
        a->collide_with(ctx, *b);
        b->collide_with(ctx, *a);
      }
    }
  }
}
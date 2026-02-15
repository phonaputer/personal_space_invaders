#include "entity.hpp"
#include <SDL3/SDL.h>
#include <algorithm>
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
#include "entity.hpp"
#include <SDL3/SDL.h>
#include <algorithm>
#include <memory>
#include <vector>

void Entities::add(std::unique_ptr<Entity> entity) {
  entities_to_add.push_back(std::move(entity));
}

void Entities::update(UpdateCtx const &ctx) {
  for (size_t i = 0; i < entities_to_add.size(); i++) {
    entities.push_back(std::move(entities_to_add.at(i)));
  }
  entities_to_add.clear();

  for (const auto &e : entities) {
    e->update(ctx);
  }

  std::erase_if(entities, [](std::unique_ptr<Entity> const &e) { return e->is_deleted(); });
}

void Entities::draw(SDL_Renderer *renderer) {
  for (const auto &e : entities) {
    e->draw(renderer);
  }
}
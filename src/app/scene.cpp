#include "scene.hpp"

void Entities::add(std::unique_ptr<Entity> entity) {
  entities.push_back(std::move(entity));
}

void Entities::update() {
  for (const auto &e : entities) {
    e->update();
  }
}

void Entities::draw(SDL_Renderer *renderer) {
  for (const auto &e : entities) {
    e->draw(renderer);
  }
}
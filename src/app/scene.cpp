#include "scene.hpp"

// TODO list
// * order drawables by Z axis
// * consider entities that either don't update or don't draw

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
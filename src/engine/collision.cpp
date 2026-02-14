#include "collision.hpp"
#include <memory>
#include <vector>

void Collidables::add(std::unique_ptr<Collidable> collidable) {
  collidables_to_add.push_back(std::move(collidable));
}

void Collidables::update() {
  for (size_t i = 0; i < collidables_to_add.size(); i++) {
    collidables.push_back(std::move(collidables_to_add.at(i)));
  }
  collidables_to_add.clear();

  std::erase_if(collidables, [](std::unique_ptr<Collidable> const &c) { return c->is_deleted(); });
}
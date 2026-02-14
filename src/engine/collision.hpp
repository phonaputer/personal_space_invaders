#pragma once

#include "core.hpp"
#include <memory>
#include <vector>

class Collidable {
  public:
    virtual ~Collidable() = default;
    virtual core::Rect get_hitbox() const = 0;
    virtual void collide_with([[maybe_unused]] Collidable &other) {};
    virtual bool is_deleted() const {
      return false;
    }
};

// This probably isn't going to work since there's already a unique pointer to every entity in the Entities type
class CollidableAdder {
  public:
    virtual ~CollidableAdder() = default;
    virtual void add(std::unique_ptr<Collidable> entity) = 0;
};

class Collidables : public CollidableAdder {
  private:
    std::vector<std::unique_ptr<Collidable>> collidables;
    std::vector<std::unique_ptr<Collidable>> collidables_to_add;

  public:
    void add(std::unique_ptr<Collidable> collidable);
    void update();
};
#pragma once

#include "assets.hpp"
#include "core.hpp"
#include "input.hpp"
#include <SDL3/SDL.h>
#include <functional>
#include <memory>
#include <optional>
#include <vector>

class Entities;

class Collidable {
  public:
    virtual core::Rect get_hitbox() const = 0;
    virtual void collide_with([[maybe_unused]] Collidable &other) {};
};

class Drawable {
  public:
    virtual void draw(SDL_Renderer *renderer) const = 0;
};

struct UpdateCtx {
    Assets const &assets;
    Entities &entities;
    UserInputs const &user_inputs;
};

class Updateable {
  public:
    virtual void update(UpdateCtx const &ctx) = 0;
};

class Entity {
  public:
    virtual ~Entity() = default;

    virtual std::optional<std::reference_wrapper<Collidable>> as_collidable() {
      return std::nullopt;
    }

    virtual std::optional<std::reference_wrapper<Drawable>> as_drawable() {
      return std::nullopt;
    }

    virtual std::optional<std::reference_wrapper<Updateable>> as_updateable() {
      return std::nullopt;
    }

    virtual bool is_deleted() const {
      return false;
    }
};

class EntityAdder {
  public:
    virtual ~EntityAdder() = default;
    virtual void add(std::shared_ptr<Entity> entity) = 0;
};

class Entities : public EntityAdder {
  private:
    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<std::shared_ptr<Entity>> entities_to_add;

  public:
    void add(std::shared_ptr<Entity> entity);
    void update(UpdateCtx const &ctx);
    void draw(SDL_Renderer *renderer);
};
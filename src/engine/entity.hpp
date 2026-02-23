#pragma once

#include "assets.hpp"
#include "core.hpp"
#include "input.hpp"
#include <SDL3/SDL.h>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class Entities;

struct CollideCtx {
    Assets const &assets;
    Entities &entities;
};

class Collidable {
  public:
    virtual std::string get_type() const = 0;
    virtual core::Rect get_hitbox() const = 0;
    virtual void collide_with([[maybe_unused]] CollideCtx const &ctx, [[maybe_unused]] Collidable &other) {};
};

class Drawable {
  public:
    virtual void draw(SDL_Renderer *renderer) const = 0;
};

class Entity {
  public:
    virtual ~Entity() = default;

    virtual std::string get_type() const = 0;

    virtual std::optional<std::reference_wrapper<Collidable>> as_collidable() {
      return std::nullopt;
    }

    virtual std::optional<std::reference_wrapper<Drawable>> as_drawable() {
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

struct UpdateCtx {
    Assets const &assets;
    Entities &entities;
    UserInputs const &user_inputs;
};

class Entities : public EntityAdder {
  private:
    std::vector<std::shared_ptr<Entity>> entities;
    std::vector<std::shared_ptr<Entity>> entities_to_add;

    void check_collisions(CollideCtx const &ctx);

  public:
    void add(std::shared_ptr<Entity> entity);
    void update(UpdateCtx const &ctx);
    void draw(SDL_Renderer *renderer);
};
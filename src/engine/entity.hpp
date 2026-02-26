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

class EntityAdder;

struct CollideCtx {
    Assets const &assets;
    EntityAdder &entities;
};

class Collidable {
  public:
    virtual ~Collidable() = default;
    virtual std::string get_type() const = 0;
    virtual core::Rect get_hitbox() const = 0;
    virtual void collide_with([[maybe_unused]] CollideCtx ctx, [[maybe_unused]] Collidable &other) {};
    virtual bool is_deleted() const = 0;
    virtual bool is_active() const {
      return true;
    }
};

class Drawable {
  public:
    virtual ~Drawable() = default;
    virtual void draw(SDL_Renderer *renderer) const = 0;
    virtual bool is_deleted() const = 0;
    virtual bool is_active() const {
      return true;
    }
};

class EntityAdder {
  public:
    virtual ~EntityAdder() = default;
    virtual void add_drawable(std::shared_ptr<Drawable> entity) = 0;
    virtual void add_collidable(std::shared_ptr<Collidable> entity) = 0;
};

struct UpdateCtx {
    Assets const &assets;
    EntityAdder &entities;
    UserInputs const &user_inputs;
};

class Entities : public EntityAdder {
  private:
    std::vector<std::shared_ptr<Drawable>> drawables;
    std::vector<std::shared_ptr<Drawable>> drawables_to_add;
    std::vector<std::shared_ptr<Collidable>> collidables;
    std::vector<std::shared_ptr<Collidable>> collidables_to_add;

    void check_collisions(CollideCtx const &ctx);

  public:
    void add_drawable(std::shared_ptr<Drawable> entity) override;
    void add_collidable(std::shared_ptr<Collidable> entity) override;
    void update(UpdateCtx const &ctx);
    void draw(SDL_Renderer *renderer);
};
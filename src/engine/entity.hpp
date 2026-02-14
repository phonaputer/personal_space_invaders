#pragma once

#include "assets.hpp"
#include "input.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

class Entities;

struct UpdateCtx {
    Assets const &assets;
    Entities &entities;
    UserInputs const &user_inputs;
};

class Entity {
  public:
    virtual ~Entity() = default;
    virtual void update(UpdateCtx const &ctx) = 0;
    virtual void draw(SDL_Renderer *renderer) = 0;
    virtual bool is_deleted() {
      return false;
    }
};

class EntityAdder {
  public:
    virtual ~EntityAdder() = default;
    virtual void add(std::unique_ptr<Entity> entity) = 0;
};

class Entities : public EntityAdder {
  private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::unique_ptr<Entity>> entities_to_add;

  public:
    void add(std::unique_ptr<Entity> entity);
    void update(UpdateCtx const &ctx);
    void draw(SDL_Renderer *renderer);
};
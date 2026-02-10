#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <vector>

class Entity {
  public:
    virtual void update() = 0;
    virtual void draw(SDL_Renderer *renderer) = 0;
    virtual ~Entity() = default;
};

class EntityAdder {
  public:
    virtual ~EntityAdder() = default;
    virtual void add(std::unique_ptr<Entity> entity) = 0;
};

class Entities : public EntityAdder {
  private:
    std::vector<std::unique_ptr<Entity>> entities;

  public:
    void add(std::unique_ptr<Entity> entity);
    void update();
    void draw(SDL_Renderer *renderer);
};
#pragma once

#include "assets.hpp"
#include "input.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

class Entity {
  public:
    virtual void update() = 0;
    virtual void draw(SDL_Renderer *renderer) = 0;
    virtual ~Entity() = default;
};

class Entities {
  private:
    std::vector<std::unique_ptr<Entity>> entities;

  public:
    void add(std::unique_ptr<Entity> entity);
    void update();
    void draw(SDL_Renderer *renderer);
};

struct SceneCtx {
    Assets &assets;
    Entities &entities;
    UserInputs &user_inputs;
};

struct PreloadAssetsCtx {
    Assets &assets;
};

class Scene {
  public:
    virtual void preload_assets(PreloadAssetsCtx const &ctx) = 0;
    virtual void initialize(SceneCtx const &ctx) = 0;
    virtual ~Scene() = default;
};
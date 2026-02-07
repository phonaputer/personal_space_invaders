#pragma once

#include "assets.hpp"
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
    std::shared_ptr<Assets> assets;
    std::shared_ptr<Entities> entities;
};

struct PreloadAssetsCtx {
    std::shared_ptr<Assets> assets;
};

class Scene {
  public:
    virtual void preload_assets(std::unique_ptr<PreloadAssetsCtx> const &ctx) = 0;
    virtual void initialize(std::unique_ptr<SceneCtx> const &ctx) = 0;
    virtual ~Scene() = default;
};
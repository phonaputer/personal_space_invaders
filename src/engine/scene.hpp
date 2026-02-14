#pragma once

#include "assets.hpp"
#include "entity.hpp"
#include "input.hpp"
#include <SDL3/SDL.h>

struct SceneCtx {
    Assets const &assets;
    Entities &entities;
    UserInputs const &user_inputs;
};

struct PreloadAssetsCtx {
    Assets &assets;
};

class Scene {
  public:
    virtual void preload_assets(PreloadAssetsCtx const &ctx) = 0;
    virtual void initialize(SceneCtx ctx) = 0;
    virtual ~Scene() = default;
};
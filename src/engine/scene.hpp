#pragma once

#include "assets.hpp"
#include "entity.hpp"
#include "input.hpp"

struct SceneCtx {
    Assets &assets;
    EntityAdder &entities;
    UserInputs const &user_inputs;
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
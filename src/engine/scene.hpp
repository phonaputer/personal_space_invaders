#pragma once

#include "assets.hpp"
#include "entity.hpp"
#include "input.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <optional>

struct SceneCtx {
    Assets const &assets;
    EntityAdder &entities;
    UserInputs const &user_inputs;
};

struct PreloadAssetsCtx {
    Assets &assets;
};

class Scene {
  public:
    virtual void preload_assets(PreloadAssetsCtx ctx) = 0;
    virtual void initialize(SceneCtx ctx) = 0;
    virtual std::optional<std::unique_ptr<Scene>> update(SceneCtx ctx) = 0;
    virtual ~Scene() = default;
};
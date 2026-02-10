#pragma once

#include "engine/scene.hpp"
#include <memory>

class InvasionScene : public Scene {
  public:
    InvasionScene();
    void preload_assets(PreloadAssetsCtx const &ctx);
    void initialize(SceneCtx ctx);
};

#pragma once

#include "engine/scene.hpp"
#include "invaders.hpp"
#include "player.hpp"
#include <memory>
#include <vector>

class InvasionScene : public Scene {
  public:
    void preload_assets(PreloadAssetsCtx const &ctx) override;
    void initialize(SceneCtx const &ctx) override;
};

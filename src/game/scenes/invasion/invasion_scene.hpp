#pragma once

#include "engine/scene.hpp"
#include "invader_entities.hpp"
#include "player_entity.hpp"
#include <memory>
#include <vector>

class InvasionScene : public Scene {
  public:
    void preload_assets(PreloadAssetsCtx const &ctx) override;
    void initialize(SceneCtx ctx) override;
};

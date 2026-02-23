#pragma once

#include "engine/scene.hpp"
#include "invaders.hpp"
#include "player.hpp"
#include "scoreboard.hpp"
#include <memory>
#include <vector>

class InvasionScene : public Scene {
  private:
    std::shared_ptr<AlienOrchestrator> alien_orchestrator;
    std::shared_ptr<Player> player;

  public:
    void preload_assets(PreloadAssetsCtx const &ctx) override;
    void initialize(SceneCtx const &ctx) override;
    void update(SceneCtx const &ctx) override;
};

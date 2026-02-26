#pragma once

#include "engine/scene.hpp"
#include "fortress.hpp"
#include "game_state.hpp"
#include "invaders.hpp"
#include "player.hpp"
#include <memory>
#include <vector>

class InvasionScene : public Scene {
  private:
    std::shared_ptr<GameStateOrchestrator> game_state;
    std::shared_ptr<AlienOrchestrator> alien_orchestrator;
    std::shared_ptr<Player> player;
    std::vector<std::shared_ptr<Fortress>> fortresses;

  public:
    void preload_assets(PreloadAssetsCtx ctx) override;
    void initialize(SceneCtx ctx) override;
    void update(SceneCtx ctx) override;
};

#pragma once

#include "engine/scene.hpp"
#include "main_menu.hpp"
#include <memory>
#include <optional>

class MainMenuScene : public Scene {
  private:
    std::shared_ptr<MainMenu> main_menu;

  public:
    void preload_assets(PreloadAssetsCtx ctx) override;
    void initialize(SceneCtx ctx) override;
    std::optional<std::unique_ptr<Scene>> update(SceneCtx ctx) override;
};

#include "main_menu_scene.hpp"
#include "engine/scene.hpp"
#include "game/scenes/invasion/invasion_scene.hpp"
#include "main_menu.hpp"
#include "main_menu_constants.hpp"
#include <iostream>
#include <memory>
#include <optional>

void MainMenuScene::preload_assets(PreloadAssetsCtx ctx) {
  ctx.assets.load_png_texture(image::PRIMARY_SPRITESHEET, "./assets/space_invaders.png");
  ctx.assets.load_audio(sound::MENU_SELECT, "./assets/menu_select.wav");
}

void MainMenuScene::initialize(SceneCtx ctx) {
  main_menu = MainMenu::create(ctx);
}

std::optional<std::unique_ptr<Scene>> MainMenuScene::update([[maybe_unused]] SceneCtx ctx) {
  main_menu->update();

  if (main_menu->start_game()) {
    return std::make_unique<InvasionScene>();
  }

  return std::nullopt;
}
#include "invasion_scene.hpp"
#include "invasion_entities.hpp"

InvasionScene::InvasionScene() {
}

void InvasionScene::preload_assets(std::unique_ptr<PreloadAssetsCtx> const &ctx) {
  ctx->assets->load_png_texture("space_invaders", "./assets/space_invaders.png");
}

void InvasionScene::initialize(std::unique_ptr<SceneCtx> const &ctx) {
  ctx->entities->add(std::make_unique<AlienEntity>(ctx->assets->get_texture("space_invaders")));
}

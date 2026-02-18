#include "invasion_scene.hpp"
#include "engine/core.hpp"
#include "engine/scene.hpp"
#include "invaders.hpp"
#include "invasion_constants.hpp"
#include "player.hpp"
#include <memory>

void InvasionScene::preload_assets(PreloadAssetsCtx const &ctx) {
  ctx.assets.load_png_texture(asset::PRIMARY_SPRITESHEET, "./assets/space_invaders.png");
}

void InvasionScene::initialize(SceneCtx ctx) {
  auto alien_orchestrator = std::make_unique<AlienOrchestrator>();

  // TODO can the preload be removed and the texture loading added here instead?
  auto alien_factory = AlienFactory(ctx, ctx.assets.get_texture(asset::PRIMARY_SPRITESHEET));

  const float starting_x_pos = 200;
  float x_pos = starting_x_pos;
  float y_pos = 100;
  const float alien_width = 60;
  const float alien_height = 60;
  const float col_spacing = 5;
  const float row_spacing = -2;

  for (int i = 0; i < 11; i++) {
    alien_orchestrator->add_alien(alien_factory.new_octopus({x_pos, y_pos}));
    x_pos += alien_width + col_spacing;
  }

  x_pos = starting_x_pos;
  y_pos += alien_height + row_spacing;
  for (int i = 0; i < 11; i++) {
    alien_orchestrator->add_alien(alien_factory.new_jellyfish({x_pos, y_pos}));
    x_pos += alien_width + col_spacing;
  }

  x_pos = starting_x_pos;
  y_pos += alien_height + row_spacing;
  for (int i = 0; i < 11; i++) {
    alien_orchestrator->add_alien(alien_factory.new_crab({x_pos, y_pos}));
    x_pos += alien_width + col_spacing;
  }

  x_pos = starting_x_pos;
  y_pos += alien_height + row_spacing;
  for (int i = 0; i < 11; i++) {
    alien_orchestrator->add_alien(alien_factory.new_tadpole({x_pos, y_pos}));
    x_pos += alien_width + col_spacing;
  }

  x_pos = starting_x_pos;
  y_pos += alien_height + row_spacing;
  for (int i = 0; i < 11; i++) {
    alien_orchestrator->add_alien(alien_factory.new_tadpole({x_pos, y_pos}));
    x_pos += alien_width + col_spacing;
  }

  ctx.entities.add(std::move(alien_orchestrator));

  ctx.entities.add(std::make_unique<Player>(ctx.assets.get_texture(asset::PRIMARY_SPRITESHEET), core::Point{500, 700}));
}
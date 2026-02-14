#include "invasion_scene.hpp"
#include "engine/core.hpp"
#include "engine/scene.hpp"
#include "invader_entities.hpp"
#include "player_entity.hpp"
#include <memory>

void InvasionScene::preload_assets(PreloadAssetsCtx const &ctx) {
  ctx.assets.load_png_texture("space_invaders", "./assets/space_invaders.png");
}

void InvasionScene::initialize(SceneCtx ctx) {
  auto alien_orchestrator = std::make_unique<AlienOrchestrator>();

  // TODO can the preload be removed and the texture loading added here instead?
  auto alien_factory = AlienFactory(ctx.assets.get_texture("space_invaders"));

  const float starting_x_pos = 200;
  float x_pos = starting_x_pos;
  float y_pos = 100;
  const float alien_width = 60;
  const float alien_height = 60;

  for (int i = 0; i < 11; i++) {
    alien_orchestrator->add_alien(alien_factory.new_octopus({x_pos, y_pos}));
    x_pos += alien_width + 5;
  }

  x_pos = starting_x_pos;
  y_pos += alien_height + 5;
  for (int i = 0; i < 11; i++) {
    alien_orchestrator->add_alien(alien_factory.new_jellyfish({x_pos, y_pos}));
    x_pos += alien_width + 5;
  }

  x_pos = starting_x_pos;
  y_pos += alien_height + 5;
  for (int i = 0; i < 11; i++) {
    alien_orchestrator->add_alien(alien_factory.new_crab({x_pos, y_pos}));
    x_pos += alien_width + 5;
  }

  x_pos = starting_x_pos;
  y_pos += alien_height + 5;
  for (int i = 0; i < 11; i++) {
    alien_orchestrator->add_alien(alien_factory.new_tadpole({x_pos, y_pos}));
    x_pos += alien_width + 5;
  }

  x_pos = starting_x_pos;
  y_pos += alien_height + 5;
  for (int i = 0; i < 11; i++) {
    alien_orchestrator->add_alien(alien_factory.new_tadpole({x_pos, y_pos}));
    x_pos += alien_width + 5;
  }

  ctx.entities.add(std::move(alien_orchestrator));

  ctx.entities.add(std::make_unique<PlayerEntity>(ctx.assets.get_texture("space_invaders"), core::Point{500, 700}));
}
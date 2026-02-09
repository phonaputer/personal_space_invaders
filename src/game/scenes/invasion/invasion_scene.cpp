#include "invasion_scene.hpp"
#include "invader_entities.hpp"
#include "player_entity.hpp"
#include <memory>

InvasionScene::InvasionScene() {
}

void InvasionScene::preload_assets(PreloadAssetsCtx const &ctx) {
  ctx.assets.load_png_texture("space_invaders", "./assets/space_invaders.png");
}

void InvasionScene::initialize(SceneCtx const &ctx) {
  // TODO can the preload be removed and the texture loading added here instead?
  auto space_invaders_texture = ctx.assets.get_texture("space_invaders");

  const float starting_x_pos = 200;
  float x_pos = starting_x_pos;
  float y_pos = 100;
  const float alien_width = 60;
  const float alien_height = 60;

  for (int i = 0; i < 11; i++) {
    ctx.entities.add(std::make_unique<OctopusEntity>(space_invaders_texture, Position{x_pos, y_pos}));
    x_pos += alien_width + 5;
  }

  x_pos = starting_x_pos;
  y_pos += alien_height + 5;
  for (int i = 0; i < 11; i++) {
    ctx.entities.add(std::make_unique<JellyfishEntity>(space_invaders_texture, Position{x_pos, y_pos}));
    x_pos += alien_width + 5;
  }

  x_pos = starting_x_pos;
  y_pos += alien_height + 5;
  for (int i = 0; i < 11; i++) {
    ctx.entities.add(std::make_unique<CrabEntity>(space_invaders_texture, Position{x_pos, y_pos}));
    x_pos += alien_width + 5;
  }

  x_pos = starting_x_pos;
  y_pos += alien_height + 5;
  for (int i = 0; i < 11; i++) {
    ctx.entities.add(std::make_unique<TadpoleEntity>(space_invaders_texture, Position{x_pos, y_pos}));
    x_pos += alien_width + 5;
  }

  x_pos = starting_x_pos;
  y_pos += alien_height + 5;
  for (int i = 0; i < 11; i++) {
    ctx.entities.add(std::make_unique<TadpoleEntity>(space_invaders_texture, Position{x_pos, y_pos}));
    x_pos += alien_width + 5;
  }

  ctx.entities.add(std::make_unique<PlayerEntity>(space_invaders_texture, Position{500, 700}));
}

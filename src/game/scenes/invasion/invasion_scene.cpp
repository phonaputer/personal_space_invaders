#include "invasion_scene.hpp"
#include "engine/core.hpp"
#include "engine/scene.hpp"
#include "hud.hpp"
#include "invaders.hpp"
#include "invasion_constants.hpp"
#include "player.hpp"
#include <memory>

void InvasionScene::preload_assets(PreloadAssetsCtx ctx) {
  // TODO this could just go in the initialize function...
  ctx.assets.load_png_texture(image::PRIMARY_SPRITESHEET, "./assets/space_invaders.png");

  ctx.assets.load_audio(sound::ALIEN_EXPLOSION, "./assets/alien_explosion.wav");
  ctx.assets.load_audio(sound::ALIEN_SHOT, "./assets/alien_shot.wav");
  ctx.assets.load_audio(sound::PLAYER_EXPLOSION, "./assets/player_explosion.wav");
  ctx.assets.load_audio(sound::PLAYER_SHOT, "./assets/player_shot.wav");
  ctx.assets.load_audio(sound::ARP_1, "./assets/arp1.wav");
  ctx.assets.load_audio(sound::ARP_2, "./assets/arp2.wav");
  ctx.assets.load_audio(sound::ARP_3, "./assets/arp3.wav");
  ctx.assets.load_audio(sound::ARP_4, "./assets/arp4.wav");
}

void InvasionScene::initialize(SceneCtx ctx) {
  game_state = std::make_shared<GameStateOrchestrator>();

  auto hud = HUD::create(ctx);
  game_state->add_notifier(hud);

  auto explosions = std::make_shared<AlienExplosionOrchestrator>();

  alien_orchestrator = std::make_shared<AlienOrchestrator>(explosions);
  game_state->add_notifier(alien_orchestrator);
  alien_orchestrator->add_overrun_notifier(game_state);

  player = Player::create(ctx, {500, GROUND_Y - 45});
  player->add_notifier(game_state);
  game_state->add_notifier(player);

  auto alien_factory = AlienFactory(ctx, game_state, explosions);

  const float starting_x_pos = 200;
  float x_pos = starting_x_pos;
  float y_pos = 50;
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

  float fort_x_pos = 200;
  const float fort_y_pos = core::WINDOW_HEIGHT - 175;
  const float fort_x_spacing = 200;

  for (int i = 0; i < 4; i++) {
    auto fort = Fortress::create(ctx, {fort_x_pos, fort_y_pos});
    fortresses.push_back(fort);
    game_state->add_notifier(fort);

    fort_x_pos += fort_x_spacing;
  }

  ground = Ground::create(ctx);

  game_state->restart_game();
}

void InvasionScene::update(SceneCtx ctx) {
  game_state->update();
  alien_orchestrator->update(ctx);
  player->update();
}
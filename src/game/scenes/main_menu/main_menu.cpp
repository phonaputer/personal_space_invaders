#include "main_menu.hpp"
#include "engine/entity.hpp"
#include "engine/input.hpp"
#include "engine/scene.hpp"
#include "engine/text_renderer.hpp"
#include "main_menu_constants.hpp"
#include <memory>

MainMenu::MainMenu(SceneCtx ctx, Spritesheet spritesheet, TextRenderer text_renderer)
    : ctx{ctx},
      spritesheet{spritesheet},
      text_renderer{text_renderer} {
}

std::shared_ptr<MainMenu> MainMenu::create(SceneCtx ctx) {
  auto sheet_image = ctx.assets.get_texture(image::PRIMARY_SPRITESHEET);

  auto result =
      std::shared_ptr<MainMenu>(new MainMenu(ctx, Spritesheet(sheet_image, 16, 16), TextRenderer(sheet_image)));

  ctx.entities.add_drawable(result);

  return result;
}

void MainMenu::update() {
  if (game_start_engaged) {
    game_start_tick_counter++;
    if (game_start_tick_counter % GAME_START_BLINK_TICKS == 0) {
      game_start_blink = !game_start_blink;
    }
    return;
  }

  if (ctx.user_inputs.is_initiated(PlayerInput::FIRE)) {
    game_start_engaged = true;
    ctx.assets.play_audio(sound::MENU_SELECT);
  }
}

bool MainMenu::start_game() {
  return game_start_tick_counter > GAME_START_TOTAL_TICKS;
}

void MainMenu::draw(SDL_Renderer *renderer) const {
  text_renderer.render_text_centered(renderer, 36, "personal space invaders");

  spritesheet.draw_frame(renderer, {4, 0}, {222, 123, 36, 36});
  text_renderer.render_text_centered(renderer, 135, " - 10 points");

  spritesheet.draw_frame(renderer, {1, 0}, {222, 153, 36, 36});
  text_renderer.render_text_centered(renderer, 165, " - 20 points");

  spritesheet.draw_frame(renderer, {1, 1}, {222, 183, 36, 36});
  text_renderer.render_text_centered(renderer, 195, " - 30 points");

  spritesheet.draw_frame(renderer, {7, 0}, {222, 213, 36, 36});
  text_renderer.render_text_centered(renderer, 225, " - 40 points");

  text_renderer.render_text_centered(renderer, 294, "<a> and <d> to move");
  text_renderer.render_text_centered(renderer, 318, "<space> to shoot");
  text_renderer.render_text_centered(renderer, 342, "<esc> to pause");

  if (game_start_engaged && game_start_blink) {
    text_renderer.render_text_centered(renderer, 420, "press <space> to begin");
  } else if (!game_start_engaged) {
    text_renderer.render_text_centered(renderer, 420, "press <space> to begin");
  }
}

unsigned int MainMenu::get_z() const {
  return 1;
}

bool MainMenu::is_deleted() const {
  return false;
}
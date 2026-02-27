#include "pause_menu.hpp"
#include "engine/core.hpp"
#include "engine/entity.hpp"
#include "engine/scene.hpp"
#include "invasion_constants.hpp"
#include "text_renderer.hpp"
#include <SDL3/SDL.h>
#include <memory>

PauseMenu::PauseMenu(SceneCtx ctx, std::shared_ptr<SDL_Texture> texture)
    : ctx{ctx},
      text_renderer(texture) {
}

void PauseMenu::execute_option() {
  switch (option_idx) {
    case 1:
      // restart
      break;
    default:
      active = false;
      break;
  }

  option_selected = false;
}

std::shared_ptr<PauseMenu> PauseMenu::create(SceneCtx ctx) {
  auto result = std::shared_ptr<PauseMenu>(new PauseMenu(ctx, ctx.assets.get_texture(image::PRIMARY_SPRITESHEET)));

  ctx.entities.add_drawable(result);

  return result;
}

void PauseMenu::activate() {
  active = true;
  option_selected = false;
  option_selected_blink = true;
}

void PauseMenu::update() {
  if (option_selected) {
    option_selected_tick_counter++;
    if (option_selected_tick_counter % OPTION_SELECTED_BLINK_TICKS == 0) {
      option_selected_blink = !option_selected_blink;
    }
    if (option_selected_tick_counter > OPTION_SELECTED_TOTAL_TICKS) {
      execute_option();
    }
    return;
  }

  if (ctx.user_inputs.is_initiated(PlayerInput::UP)) {
    if (option_idx == 0) {
      option_idx = MAX_SELECTION;
    } else {
      option_idx--;
    }
  }

  else if (ctx.user_inputs.is_initiated(PlayerInput::DOWN)) {
    if (option_idx == MAX_SELECTION) {
      option_idx = 0;
    } else {
      option_idx++;
    }
  }

  else if (ctx.user_inputs.is_initiated(PlayerInput::PAUSE)) {
    active = false;
  }

  else if (ctx.user_inputs.is_initiated(PlayerInput::FIRE)) {
    option_selected = true;
    option_selected_tick_counter = 0;
    ctx.assets.play_audio(sound::MENU_SELECT);
  }
}

void PauseMenu::draw(SDL_Renderer *renderer) const {
  const float x = (core::WINDOW_WIDTH * 0.5) - 108;
  const float y = (core::WINDOW_HEIGHT * 0.5) - 75;

  SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
  auto rect = SDL_FRect{x, y, 216, 150};
  SDL_RenderFillRect(renderer, &rect);

  // This is some "janky" stuff...
  if (option_idx == 0) {
    if (option_selected && option_selected_blink) {
      text_renderer.render_text(renderer, {x + 20, y + 40}, ">RESUME<");
    } else if (!option_selected) {
      text_renderer.render_text(renderer, {x + 20, y + 40}, ">RESUME<");
    }
  } else {
    text_renderer.render_text(renderer, {x + 42, y + 40}, "RESUME");
  }

  if (option_idx == 1) {
    if (option_selected && option_selected_blink) {
      text_renderer.render_text(renderer, {x + 10, y + 90}, ">RESTART<");
    } else if (!option_selected) {
      text_renderer.render_text(renderer, {x + 10, y + 90}, ">RESTART<");
    }
  } else {
    text_renderer.render_text(renderer, {x + 32, y + 90}, "RESTART");
  }
}

unsigned int PauseMenu::get_z() const {
  return zindex::PAUSE_MENU;
}

bool PauseMenu::is_deleted() const {
  return false;
}

bool PauseMenu::is_active() const {
  return active;
}
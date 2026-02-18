#pragma once

#include "engine/core.hpp"
#include "engine/sprites.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <string>

class TextRenderer {
  private:
    static constexpr float CHARACTER_DRAW_HEIGHT = 30;
    static constexpr float CHARACTER_DRAW_WIDTH = 30;
    static constexpr float CHARACTER_X_SPACING = 20;

    Spritesheet spritesheet;

  public:
    TextRenderer(std::shared_ptr<SDL_Texture> texture);
    void render_text(SDL_Renderer *renderer, core::Point location, std::string text);
};
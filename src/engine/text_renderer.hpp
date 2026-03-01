#pragma once

#include "engine/core.hpp"
#include "engine/sprites.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <string>

class TextRenderer {
  private:
    static constexpr float CHARACTER_DRAW_HEIGHT = 18;
    static constexpr float CHARACTER_DRAW_WIDTH = 18;

    Spritesheet spritesheet;

  public:
    static constexpr float CHARACTER_X_SPACING = 13;

    TextRenderer(std::shared_ptr<SDL_Texture> texture);
    void render_text(SDL_Renderer *renderer, core::Point location, std::string text) const;
    void render_text_centered(SDL_Renderer *renderer, float y, std::string text) const;
};
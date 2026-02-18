#pragma once

#include "engine/core.hpp"
#include "engine/entity.hpp"
#include "engine/sprites.hpp"
#include "invaders.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <string>

class TextRenderer {
  private:
    static constexpr float CHARACTER_DRAW_HEIGHT = 30;
    static constexpr float CHARACTER_DRAW_WIDTH = 30;
    static constexpr float CHARACTER_X_SPACING = 22;

    Spritesheet spritesheet;

  public:
    TextRenderer(std::shared_ptr<SDL_Texture> texture);
    void render_text(SDL_Renderer *renderer, core::Point location, std::string text) const;
};

class Scoreboard : public Entity, public Drawable, public ScoreNotifier {
  private:
    TextRenderer text_renderer;
    const float x;
    const float y;
    unsigned int current_score;

  public:
    Scoreboard(std::shared_ptr<SDL_Texture> texture, core::Point position);
    std::string get_type() const override;
    void notify_player_scored(unsigned int amount) override;

    std::optional<std::reference_wrapper<Drawable>> as_drawable() override;
    void draw(SDL_Renderer *renderer) const override;
};
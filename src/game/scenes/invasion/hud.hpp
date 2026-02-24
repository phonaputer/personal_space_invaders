#pragma once

#include "engine/core.hpp"
#include "engine/entity.hpp"
#include "engine/sprites.hpp"
#include "game_state.hpp"
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

class HUD : public Drawable, public GameStateNotifier {
  private:
    TextRenderer text_renderer;
    unsigned int high_score;
    unsigned int current_score;
    int current_lives;
    bool game_is_over;

    HUD(std::shared_ptr<SDL_Texture> texture);

  public:
    static std::shared_ptr<HUD> create(SceneCtx ctx);
    void notify_score_change(int new_current_score, int new_high_score) override;
    void notify_player_lives_change(int new_current_lives) override;
    void notify_game_start() override;
    void notify_game_over() override;
    bool is_deleted() const override;
    void draw(SDL_Renderer *renderer) const override;
};
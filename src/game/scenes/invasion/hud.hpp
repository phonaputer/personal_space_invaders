#pragma once

#include "engine/entity.hpp"
#include "engine/scene.hpp"
#include "game_state.hpp"
#include "text_renderer.hpp"
#include <SDL3/SDL.h>
#include <memory>

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
    unsigned int get_z() const override;
    void draw(SDL_Renderer *renderer) const override;
};
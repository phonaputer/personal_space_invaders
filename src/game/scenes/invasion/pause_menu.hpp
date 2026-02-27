#pragma once

#include "engine/entity.hpp"
#include "engine/scene.hpp"
#include "text_renderer.hpp"
#include <SDL3/SDL.h>
#include <memory>

class PauseMenu : public Drawable {
  private:
    static constexpr int MAX_SELECTION = 1;
    static constexpr int OPTION_SELECTED_TOTAL_TICKS = 25;
    static constexpr int OPTION_SELECTED_BLINK_TICKS = 3;

    SceneCtx ctx;
    TextRenderer text_renderer;
    int option_idx = 0;
    bool active = false;
    bool option_selected = false;
    int option_selected_tick_counter = 0;
    bool option_selected_blink = true;

    PauseMenu(SceneCtx ctx, std::shared_ptr<SDL_Texture> texture);
    void execute_option();

  public:
    static std::shared_ptr<PauseMenu> create(SceneCtx ctx);
    void activate();
    void update();
    void draw(SDL_Renderer *renderer) const override;
    unsigned int get_z() const override;
    bool is_deleted() const override;
    bool is_active() const override;
};
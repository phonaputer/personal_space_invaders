#pragma once

#include "engine/entity.hpp"
#include "engine/scene.hpp"
#include "engine/text_renderer.hpp"
#include <memory>
#include <string>

class MainMenu : public Drawable {

  private:
    static constexpr int GAME_START_TOTAL_TICKS = 25;
    static constexpr int GAME_START_BLINK_TICKS = 3;

    SceneCtx ctx;
    Spritesheet spritesheet;
    TextRenderer text_renderer;
    bool game_start_engaged = false;
    int game_start_tick_counter = 0;
    bool game_start_blink = false;
    MainMenu(SceneCtx ctx, Spritesheet spritesheet, TextRenderer text_renderer);

  public:
    static std::shared_ptr<MainMenu> create(SceneCtx ctx);
    void update();
    bool start_game();
    void draw(SDL_Renderer *renderer) const override;
    unsigned int get_z() const override;
    bool is_deleted() const override;
};
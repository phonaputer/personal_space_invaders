#pragma once

#include "engine/core.hpp"
#include "engine/entity.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include "game_state.hpp"
#include <memory>
#include <vector>

class FortressTile : public Drawable, public Collidable {
  private:
    static constexpr int MAX_HEALTH = 3;

    core::Point position;
    Spritesheet spritesheet;
    Frame start_frame;
    core::Rect hitbox;
    int health = MAX_HEALTH;

    FortressTile(core::Point position, Spritesheet spritesheet, Frame start_frame, core::Rect hitbox);

  public:
    static constexpr float DRAW_WIDTH = 15;
    static constexpr float DRAW_HEIGHT = 15;

    static std::shared_ptr<FortressTile>
    create(SceneCtx ctx, core::Point position, Frame start_frame, core::Rect hitbox);
    std::string get_type() const override;
    core::Rect get_hitbox() const override;
    void collide_with(CollideCtx ctx, Collidable &other) override;
    unsigned int get_z() const override;
    void draw(SDL_Renderer *renderer) const override;
    bool is_deleted() const override;
    bool is_active() const override;
    void rerack();
};

class Fortress : public GameStateNotifier {
  private:
    std::vector<std::shared_ptr<FortressTile>> tiles;
    Fortress(SceneCtx ctx, core::Point position);

  public:
    static std::shared_ptr<Fortress> create(SceneCtx ctx, core::Point position);
    void notify_game_start();
};
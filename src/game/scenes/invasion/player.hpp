#pragma once

#include "engine/core.hpp"
#include "engine/input.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include "game_state.hpp"
#include <SDL3/SDL.h>
#include <functional>
#include <memory>

class PlayerProjectile : public Collidable, public Drawable {
  private:
    static constexpr float DRAW_WIDTH = 60;
    static constexpr float DRAW_HEIGHT = 60;
    static constexpr float SPEED = 10;

    Spritesheet spritesheet;
    float x = 0;
    float y = 0;
    bool active = false;

    PlayerProjectile(std::shared_ptr<SDL_Texture> texture);

  public:
    static std::shared_ptr<PlayerProjectile> create(SceneCtx ctx);
    void shoot_from(core::Point position);
    bool is_active() const override;
    void deactivate();
    bool is_deleted() const override;
    std::string get_type() const override;
    void update();
    core::Rect get_hitbox() const override;
    void collide_with(CollideCtx ctx, Collidable &other) override;
    void draw(SDL_Renderer *renderer) const override;
};

class Player : public Collidable, public Drawable, public GameStateNotifier {
  private:
    static constexpr float DRAW_WIDTH = 60;
    static constexpr float DRAW_HEIGHT = 60;
    static constexpr float SPEED = 4;
    static constexpr int TICKS_PER_SHOT = 4;

    SceneCtx ctx;
    core::Point starting_position;
    std::unique_ptr<Animation> animation;
    std::unique_ptr<OnceAnimation> muzzle_flash_animation;
    std::unique_ptr<Animation> explosion_animation;
    float x;
    float y;
    bool am_dead;
    std::vector<std::weak_ptr<PlayerDeathNotifier>> status_notifiers;
    std::shared_ptr<PlayerProjectile> projectile;
    int shot_clock = 0;

    Player(SceneCtx ctx, core::Point starting_position);

  public:
    static std::shared_ptr<Player> create(SceneCtx ctx, core::Point starting_position);
    std::string get_type() const override;
    void add_notifier(std::weak_ptr<PlayerDeathNotifier> notifier);
    void update();
    void notify_player_rerack() override;
    bool is_deleted() const override;
    core::Rect get_hitbox() const override;
    void collide_with(CollideCtx ctx, Collidable &other) override;
    void draw(SDL_Renderer *renderer) const override;
};
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
    float x;
    float y;
    bool deleted;

  public:
    PlayerProjectile(std::shared_ptr<SDL_Texture> texture, core::Point starting_position);
    std::string get_type() const override;
    bool is_deleted() const override;
    void mark_deleted();
    void update();
    core::Rect get_hitbox() const override;
    void collide_with(CollideCtx const &ctx, Collidable &other) override;
    void draw(SDL_Renderer *renderer) const override;
};

class PlayerProjectileOrchestrator {
  private:
    std::vector<std::shared_ptr<PlayerProjectile>> projectiles;

  public:
    void add(std::shared_ptr<PlayerProjectile> projectile);
    void update();
    void delete_all();
};

class Player : public Collidable, public Drawable, public GameStateNotifier {
  private:
    static constexpr float DRAW_WIDTH = 60;
    static constexpr float DRAW_HEIGHT = 60;
    static constexpr float SPEED = 4;
    static constexpr int TICKS_PER_SHOT = 35;

    SceneCtx scene;
    core::Point starting_position;
    std::unique_ptr<Animation> animation;
    std::unique_ptr<OnceAnimation> muzzle_flash_animation;
    std::unique_ptr<Animation> explosion_animation;
    float x;
    float y;
    int shot_clock;
    bool am_dead;
    std::vector<std::weak_ptr<PlayerDeathNotifier>> status_notifiers;
    PlayerProjectileOrchestrator projectiles;

  public:
    Player(std::shared_ptr<SDL_Texture> texture, core::Point starting_position, SceneCtx scene);
    std::string get_type() const override;
    void add_notifier(std::weak_ptr<PlayerDeathNotifier> notifier);
    void update();
    void notify_player_rerack() override;
    bool is_deleted() const override;
    core::Rect get_hitbox() const override;
    void collide_with(CollideCtx const &ctx, Collidable &other) override;
    void draw(SDL_Renderer *renderer) const override;
};
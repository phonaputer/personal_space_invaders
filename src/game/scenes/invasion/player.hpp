#pragma once

#include "engine/core.hpp"
#include "engine/input.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include <SDL3/SDL.h>
#include <functional>
#include <memory>

class PlayerProjectile : public Entity, public Collidable, public Updateable, public Drawable {
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

    std::optional<std::reference_wrapper<Collidable>> as_collidable() override;
    core::Rect get_hitbox() const override;
    void collide_with(CollideCtx const &ctx, Collidable &other) override;
    std::optional<std::reference_wrapper<Drawable>> as_drawable() override;
    void draw(SDL_Renderer *renderer) const override;
    std::optional<std::reference_wrapper<Updateable>> as_updateable() override;
    void update(UpdateCtx const &ctx) override;
};

class Player : public Entity, public Collidable, public Updateable, public Drawable {
  private:
    static constexpr float DRAW_WIDTH = 60;
    static constexpr float DRAW_HEIGHT = 60;
    static constexpr float SPEED = 4;
    static constexpr const int TICKS_PER_SHOT = 35;

    std::unique_ptr<Animation> animation;
    std::unique_ptr<OnceAnimation> muzzle_flash_animation;
    std::unique_ptr<Animation> explosion_animation;
    float x;
    float y;
    int shot_clock;
    bool exploding;

  public:
    Player(std::shared_ptr<SDL_Texture> texture, core::Point starting_position);
    std::string get_type() const override;

    std::optional<std::reference_wrapper<Collidable>> as_collidable() override;
    core::Rect get_hitbox() const override;
    void collide_with(CollideCtx const &ctx, Collidable &other) override;
    std::optional<std::reference_wrapper<Drawable>> as_drawable() override;
    void draw(SDL_Renderer *renderer) const override;
    std::optional<std::reference_wrapper<Updateable>> as_updateable() override;
    void update(UpdateCtx const &ctx) override;
};
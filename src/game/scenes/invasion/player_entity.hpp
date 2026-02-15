#pragma once

#include "engine/core.hpp"
#include "engine/input.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include <SDL3/SDL.h>
#include <functional>
#include <memory>

class PlayerLazerEntity : public Entity, public Collidable, public Updateable, public Drawable {
  private:
    static constexpr float DRAW_WIDTH = 60;
    static constexpr float DRAW_HEIGHT = 60;
    static constexpr float SPEED = 5;

    Spritesheet spritesheet;
    float x;
    float y;

  public:
    PlayerLazerEntity(std::shared_ptr<SDL_Texture> texture, core::Point starting_position);
    void update(UpdateCtx const &ctx) override;
    void draw(SDL_Renderer *renderer) const override;
    bool is_deleted() const override;
    core::Rect get_hitbox() const override;
    std::optional<std::reference_wrapper<Collidable>> as_collidable() override;
    std::optional<std::reference_wrapper<Drawable>> as_drawable() override;
    std::optional<std::reference_wrapper<Updateable>> as_updateable() override;
};

class PlayerEntity : public Entity, public Collidable, public Updateable, public Drawable {
  private:
    static constexpr float DRAW_WIDTH = 60;
    static constexpr float DRAW_HEIGHT = 60;
    static constexpr float SPEED = 4;
    static constexpr const int TICKS_PER_SHOT = 50;

    std::unique_ptr<Animation> animation;
    std::unique_ptr<OnceAnimation> muzzle_flash_animation;
    float x;
    float y;
    int shot_clock;

  public:
    PlayerEntity(std::shared_ptr<SDL_Texture> texture, core::Point starting_position);
    void update(UpdateCtx const &ctx) override;
    void draw(SDL_Renderer *renderer) const override;
    core::Rect get_hitbox() const override;
    std::optional<std::reference_wrapper<Collidable>> as_collidable() override;
    std::optional<std::reference_wrapper<Drawable>> as_drawable() override;
    std::optional<std::reference_wrapper<Updateable>> as_updateable() override;
};
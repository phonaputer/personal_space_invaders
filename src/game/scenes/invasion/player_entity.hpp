#pragma once

#include "engine/collision.hpp"
#include "engine/core.hpp"
#include "engine/input.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include <SDL3/SDL.h>
#include <memory>

class PlayerLazerEntity : public Entity, public Collidable {
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
};

class PlayerEntity : public Entity, public Collidable {
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
};
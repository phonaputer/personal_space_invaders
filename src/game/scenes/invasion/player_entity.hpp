#pragma once

#include "engine/core.hpp"
#include "engine/input.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include <SDL3/SDL.h>
#include <memory>

class PlayerLazerEntity : public Entity {
  private:
    Spritesheet spritesheet;
    float x;
    float y;
    float draw_width;
    float draw_height;
    float speed;

  public:
    PlayerLazerEntity(std::shared_ptr<SDL_Texture> texture, core::Point starting_position);
    void update(UpdateCtx const &ctx) override;
    void draw(SDL_Renderer *renderer) override;
    bool is_deleted() override;
};

class PlayerEntity : public Entity {
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
    void draw(SDL_Renderer *renderer) override;
};
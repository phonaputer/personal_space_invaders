#pragma once

#include "engine/input.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include "invader_entities.hpp"
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
    PlayerLazerEntity(std::shared_ptr<SDL_Texture> texture, Position starting_position);
    void update();
    void draw(SDL_Renderer *renderer);
    bool is_deleted();
};

class PlayerEntity : public Entity {
  private:
    SceneCtx ctx;
    std::shared_ptr<SDL_Texture> texture;
    std::unique_ptr<Animation> animation;
    std::unique_ptr<OnceAnimation> muzzle_flash_animation;
    float x;
    float y;
    const float draw_width;
    const float draw_height;
    float speed;
    const int ticks_per_shot;
    int shot_clock;

  public:
    PlayerEntity(SceneCtx ctx, std::shared_ptr<SDL_Texture> texture, Position starting_position);
    void update();
    void draw(SDL_Renderer *renderer);
};
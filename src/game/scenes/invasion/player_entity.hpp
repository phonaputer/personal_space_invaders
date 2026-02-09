#pragma once

#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include "invader_entities.hpp"
#include <SDL3/SDL.h>
#include <memory>

class PlayerEntity : public Entity {
  private:
    std::unique_ptr<Animation> animation;
    float x;
    float y;
    float draw_width;
    float draw_height;

  public:
    PlayerEntity(std::shared_ptr<SDL_Texture> texture, Position starting_position);
    void update();
    void draw(SDL_Renderer *renderer);
};
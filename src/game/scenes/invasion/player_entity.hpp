#pragma once

#include "engine/input.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include "invader_entities.hpp"
#include <SDL3/SDL.h>
#include <memory>

class PlayerEntity : public Entity {
  private:
    std::unique_ptr<Animation> animation;
    UserInputs const &user_inputs;
    float x;
    float y;
    float draw_width;
    float draw_height;
    float speed;

  public:
    PlayerEntity(std::shared_ptr<SDL_Texture> texture, UserInputs const &user_inputs, Position starting_position);
    void update();
    void draw(SDL_Renderer *renderer);
};
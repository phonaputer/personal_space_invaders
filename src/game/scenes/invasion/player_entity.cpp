#include "player_entity.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include "invader_entities.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

PlayerEntity::PlayerEntity(
    std::shared_ptr<SDL_Texture> texture, UserInputs const &user_inputs, Position starting_position
)
    : user_inputs{user_inputs},
      x{starting_position.x},
      y{starting_position.y},
      draw_width{60},
      draw_height{60},
      speed{4} {
  std::vector<Frame> frames = {{0, 5}, {1, 5}, {2, 5}};
  animation = std::make_unique<Animation>(Spritesheet(texture, 16, 16), 5, frames);
}

void PlayerEntity::update() {
  if (user_inputs.is_engaged(PlayerInput::LEFT)) {
    x -= speed;
    animation->update();
  } else if (user_inputs.is_engaged(PlayerInput::RIGHT)) {
    x += speed;
    animation->update_backwards();
  }
}

void PlayerEntity::draw(SDL_Renderer *renderer) {
  animation->draw(renderer, {x, y, draw_width, draw_height});
}
#include "player_entity.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include "invader_entities.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

PlayerEntity::PlayerEntity(std::shared_ptr<SDL_Texture> texture, Position starting_position) {
  x = starting_position.x;
  y = starting_position.y;
  draw_width = 60;
  draw_height = 60;
  std::vector<Frame> frames = {{0, 5}, {1, 5}, {2, 5}};
  animation = std::make_unique<Animation>(Spritesheet(texture, 16, 16), 10, frames);
}

void PlayerEntity::update() {
  animation->update();
}

void PlayerEntity::draw(SDL_Renderer *renderer) {
  animation->draw(renderer, {x, y, draw_width, draw_height});
}
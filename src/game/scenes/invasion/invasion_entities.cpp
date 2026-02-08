#include "invasion_entities.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include <SDL3/SDL.h>
#include <memory>

AlienEntity::AlienEntity(std::shared_ptr<SDL_Texture> texture) {
  x = 10;
  y = 10;
  draw_width = 100;
  draw_height = 100;
  animation = std::make_unique<Animation>(Spritesheet(texture, 16, 16), 17, get_frames(0, 0, 4));
}

void AlienEntity::update() {
  animation->update();
}

void AlienEntity::draw(SDL_Renderer *renderer) {
  animation->draw(renderer, DrawRect{x, y, draw_width, draw_height});
}
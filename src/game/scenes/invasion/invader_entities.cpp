#include "invader_entities.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include <SDL3/SDL.h>
#include <memory>

std::unique_ptr<Animation> animation;
float x;
float y;
float draw_width;
float draw_height;

AlienEntity::AlienEntity(AlienEntityParams params) {
  x = params.starting_position.x;
  y = params.starting_position.y;
  draw_width = 60;
  draw_height = 60;
  animation = std::make_unique<Animation>(Spritesheet(params.texture, 16, 16), 17, params.frames);
}

void AlienEntity::update() {
  animation->update();
}

void AlienEntity::draw(SDL_Renderer *renderer) {
  animation->draw(renderer, DrawRect{x, y, draw_width, draw_height});
}

JellyfishEntity::JellyfishEntity(std::shared_ptr<SDL_Texture> texture, Position starting_position)
    : AlienEntity(
          AlienEntityParams{
              .texture = texture,
              .starting_position = starting_position,
              .frames = {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
          }
      ) {
}

TadpoleEntity::TadpoleEntity(std::shared_ptr<SDL_Texture> texture, Position starting_position)
    : AlienEntity(
          AlienEntityParams{
              .texture = texture,
              .starting_position = starting_position,
              .frames = {{1, 1}, {0, 1}, {1, 1}, {2, 1}},
          }
      ) {
}

OctopusEntity::OctopusEntity(std::shared_ptr<SDL_Texture> texture, Position starting_position)
    : AlienEntity(
          AlienEntityParams{
              .texture = texture,
              .starting_position = starting_position,
              .frames = {{0, 2}, {1, 2}},
          }
      ) {
}

CrabEntity::CrabEntity(std::shared_ptr<SDL_Texture> texture, Position starting_position)
    : AlienEntity(
          AlienEntityParams{
              .texture = texture,
              .starting_position = starting_position,
              .frames = {{1, 4}, {0, 4}, {1, 4}, {2, 4}},
          }
      ) {
}
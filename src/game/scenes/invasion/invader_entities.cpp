#include "invader_entities.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include <SDL3/SDL.h>
#include <memory>

AlienOrchestrator::AlienOrchestrator() : tick_counter{0} {
}

void AlienOrchestrator::add_alien(std::unique_ptr<Alien> alien) {
  aliens.push_back(std::move(alien));
}

void AlienOrchestrator::update([[maybe_unused]] UpdateCtx const &ctx) {
  tick_counter++;
  if (tick_counter < TICKS_PER_MOVE) {
    return;
  }

  tick_counter = 0;

  for (auto &alien : aliens) {
    if (alien->has_reached_edge()) {
      for (auto &alien : aliens) {
        alien->descend_and_turn(60);
      }
      break;
    }
  }

  for (auto &alien : aliens) {
    alien->move(15);
  }
}

void AlienOrchestrator::draw(SDL_Renderer *renderer) const {
  for (auto &alien : aliens) {
    alien->draw(renderer);
  }
}

AlienFactory::AlienFactory(std::shared_ptr<SDL_Texture> texture) : texture{texture} {
}

std::unique_ptr<Alien> AlienFactory::new_jellyfish(core::Point starting_position) {
  return std::make_unique<Alien>(AlienParams{
      .texture = texture,
      .starting_position = starting_position,
      .frames = {{0, 0}, {1, 0}, {2, 0}, {3, 0}},
  });
}

std::unique_ptr<Alien> AlienFactory::new_tadpole(core::Point starting_position) {
  return std::make_unique<Alien>(AlienParams{
      .texture = texture,
      .starting_position = starting_position,
      .frames = {{1, 1}, {0, 1}, {1, 1}, {2, 1}},
  });
}

std::unique_ptr<Alien> AlienFactory::new_octopus(core::Point starting_position) {
  return std::make_unique<Alien>(AlienParams{
      .texture = texture,
      .starting_position = starting_position,
      .frames = {{0, 2}, {1, 2}},
  });
}

std::unique_ptr<Alien> AlienFactory::new_crab(core::Point starting_position) {
  return std::make_unique<Alien>(AlienParams{
      .texture = texture,
      .starting_position = starting_position,
      .frames = {{1, 4}, {0, 4}, {1, 4}, {2, 4}},
  });
}

Alien::Alien(AlienParams params) : x{params.starting_position.x}, y{params.starting_position.y} {
  move_right = true;
  animation = std::make_unique<Animation>(Spritesheet(params.texture, 16, 16), 17, params.frames);
}

void Alien::move(float speed) {
  animation->next_frame();

  if (move_right) {
    x += speed;
  } else {
    x -= speed;
  }
}

void Alien::draw(SDL_Renderer *renderer) {
  animation->draw(renderer, {x, y, DRAW_WIDTH, DRAW_HEIGHT});
}

void Alien::descend_and_turn(float descend_speed) {
  move_right = !move_right;
  y += descend_speed;
}

bool Alien::has_reached_edge() {
  return x <= 60 || x + DRAW_WIDTH >= core::WINDOW_WIDTH - 60;
}
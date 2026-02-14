#pragma once

#include "engine/core.hpp"
#include "engine/entity.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

class AlienOrchestrator;

struct AlienParams {
    std::shared_ptr<SDL_Texture> texture;
    core::Point starting_position;
    std::vector<Frame> frames;
};

class Alien {
  private:
    static constexpr float DRAW_WIDTH = 60;
    static constexpr float DRAW_HEIGHT = 60;

    std::unique_ptr<Animation> animation;
    float x;
    float y;
    bool move_right;

  public:
    Alien(AlienParams params);
    void move(float speed);
    void draw(SDL_Renderer *renderer);
    void descend_and_turn(float descend_speed);
    bool has_reached_edge();
};

class AlienOrchestrator : public Entity {
  private:
    static constexpr int TICKS_PER_MOVE = 30;

    int tick_counter;
    std::vector<std::unique_ptr<Alien>> aliens;

  public:
    AlienOrchestrator();
    void add_alien(std::unique_ptr<Alien> alien);
    void update(UpdateCtx const &ctx) override;
    void draw(SDL_Renderer *renderer) override;
};

class AlienFactory {
  private:
    std::shared_ptr<SDL_Texture> texture;

  public:
    AlienFactory(std::shared_ptr<SDL_Texture> texture);
    std::unique_ptr<Alien> new_jellyfish(core::Point starting_position);
    std::unique_ptr<Alien> new_tadpole(core::Point starting_position);
    std::unique_ptr<Alien> new_octopus(core::Point starting_position);
    std::unique_ptr<Alien> new_crab(core::Point starting_position);
};
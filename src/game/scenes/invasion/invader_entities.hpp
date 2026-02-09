#pragma once

#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <vector>

// TODO maybe move this to engine?
// Well, it's not actually used there (yet) so maybe not.
struct Position {
    float x;
    float y;
};

struct AlienEntityParams {
    std::shared_ptr<SDL_Texture> texture;
    Position starting_position;
    std::vector<Frame> frames;
};

class AlienEntity : public Entity {
  private:
    std::unique_ptr<Animation> animation;
    float x;
    float y;
    float draw_width;
    float draw_height;

  public:
    AlienEntity(AlienEntityParams params);
    void update();
    void draw(SDL_Renderer *renderer);
};

class JellyfishEntity : public AlienEntity {
  public:
    JellyfishEntity(std::shared_ptr<SDL_Texture> texture, Position starting_position);
};

class TadpoleEntity : public AlienEntity {
  public:
    TadpoleEntity(std::shared_ptr<SDL_Texture> texture, Position starting_position);
};

class OctopusEntity : public AlienEntity {
  public:
    OctopusEntity(std::shared_ptr<SDL_Texture> texture, Position starting_position);
};

class CrabEntity : public AlienEntity {
  public:
    CrabEntity(std::shared_ptr<SDL_Texture> texture, Position starting_position);
};
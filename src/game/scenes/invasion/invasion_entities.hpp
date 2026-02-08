#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include <SDL3/SDL.h>
#include <memory>

class AlienEntity : public Entity {
  private:
    std::unique_ptr<Animation> animation;
    float x;
    float y;
    float draw_width;
    float draw_height;

  public:
    AlienEntity(std::shared_ptr<SDL_Texture> texture);
    void update();
    void draw(SDL_Renderer *renderer);
};
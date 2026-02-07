#include "app/scene.hpp"
#include "util/draw_util.hpp"
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
    AlienEntity(std::shared_ptr<SDL_Texture> texture) {
      x = 10;
      y = 10;
      draw_width = 100;
      draw_height = 100;
      animation = std::make_unique<Animation>(Spritesheet(texture, 16, 16), 17, get_frames(0, 0, 4));
    }

    void update() {
      animation->update();
    }

    void draw(SDL_Renderer *renderer) {
      animation->draw(renderer, DrawRect{x, y, draw_width, draw_height});
    }
};

class TestScene : public Scene {
  public:
    TestScene() {
    }

    void preload_assets(std::unique_ptr<PreloadAssetsCtx> const &ctx) {
      ctx->assets->load_png_texture("space_invaders", "./assets/space_invaders.png");
    }

    void initialize(std::unique_ptr<SceneCtx> const &ctx) {
      ctx->entities->add(std::make_unique<AlienEntity>(ctx->assets->get_texture("space_invaders")));
    }
};

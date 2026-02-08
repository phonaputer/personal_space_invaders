#include "engine/scene.hpp"
#include <memory>

class InvasionScene : public Scene {
  public:
    InvasionScene();
    void preload_assets(std::unique_ptr<PreloadAssetsCtx> const &ctx);
    void initialize(std::unique_ptr<SceneCtx> const &ctx);
};

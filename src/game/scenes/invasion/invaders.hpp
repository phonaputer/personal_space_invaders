#pragma once

#include "engine/core.hpp"
#include "engine/entity.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include <SDL3/SDL.h>
#include <functional>
#include <memory>
#include <vector>

class AlienProjectile : public Entity, public Collidable, public Updateable, public Drawable {
  private:
    static constexpr float DRAW_WIDTH = 60;
    static constexpr float DRAW_HEIGHT = 60;
    static constexpr float SPEED = 3;

    std::unique_ptr<Animation> animation;
    float x;
    float y;
    bool deleted;

  public:
    AlienProjectile(std::shared_ptr<SDL_Texture> texture, core::Point starting_position);
    std::string get_type() const override;
    bool is_deleted() const override;

    std::optional<std::reference_wrapper<Collidable>> as_collidable() override;
    core::Rect get_hitbox() const override;
    void collide_with(CollideCtx const &ctx, Collidable &other) override;
    std::optional<std::reference_wrapper<Drawable>> as_drawable() override;
    void draw(SDL_Renderer *renderer) const override;
    std::optional<std::reference_wrapper<Updateable>> as_updateable() override;
    void update(UpdateCtx const &ctx) override;
};

class AlienExplosion : public Entity, public Drawable, public Updateable {
  private:
    static constexpr int LIFETIME_TICKS = 20;
    static constexpr float DRAW_WIDTH = 60;
    static constexpr float DRAW_HEIGHT = 60;
    int tick_counter;
    std::unique_ptr<Animation> animation;
    float x;
    float y;

  public:
    AlienExplosion(std::shared_ptr<SDL_Texture> texture, core::Point position);
    std::string get_type() const override;
    bool is_deleted() const override;

    std::optional<std::reference_wrapper<Drawable>> as_drawable() override;
    void draw(SDL_Renderer *renderer) const override;
    std::optional<std::reference_wrapper<Updateable>> as_updateable() override;
    void update(UpdateCtx const &ctx) override;
};

class ScoreNotifier {
  public:
    virtual void notify_player_scored(unsigned int amount) = 0;
};

class AlienOrchestrator;

struct AlienParams {
    std::shared_ptr<SDL_Texture> texture;
    core::Point starting_position;
    std::vector<Frame> frames;
    core::Rect hitbox;
    std::shared_ptr<ScoreNotifier> score_notifier;
    unsigned int score;
};

class Alien : public Entity, public Drawable, public Collidable {
  private:
    static constexpr float DRAW_WIDTH = 60;
    static constexpr float DRAW_HEIGHT = 60;

    const core::Point starting_position;
    std::unique_ptr<Animation> animation;
    float x;
    float y;
    bool move_right;
    const core::Rect hitbox;
    bool deactivated;
    std::shared_ptr<ScoreNotifier> score_notifier;
    const unsigned int score;

  public:
    Alien(AlienParams params);
    std::string get_type() const override;
    void move(float speed);
    void descend_and_turn(float descend_speed);
    bool has_reached_edge() const;
    core::Point get_position() const;
    void rerack();
    bool is_active() const;

    std::optional<std::reference_wrapper<Drawable>> as_drawable() override;
    void draw(SDL_Renderer *renderer) const override;
    std::optional<std::reference_wrapper<Collidable>> as_collidable() override;
    core::Rect get_hitbox() const override;
    void collide_with(CollideCtx const &ctx, Collidable &other) override;
};

class AlienOrchestrator : public Entity, Updateable {
  private:
    static constexpr int TICKS_PER_MOVE = 30;
    static constexpr int ALIEN_SHOOT_CHANCE = 5; // The chance is the reciprocal of this number

    int tick_counter;
    std::vector<std::shared_ptr<Alien>> aliens;

  public:
    AlienOrchestrator();
    std::string get_type() const override;
    void add_alien(std::shared_ptr<Alien> alien);

    std::optional<std::reference_wrapper<Updateable>> as_updateable() override;
    void update(UpdateCtx const &ctx) override;
};

class AlienFactory {
  private:
    SceneCtx ctx;
    std::shared_ptr<SDL_Texture> texture;
    std::shared_ptr<ScoreNotifier> score_notifier;

  public:
    AlienFactory(SceneCtx ctx, std::shared_ptr<SDL_Texture> texture, std::shared_ptr<ScoreNotifier> score_notifier);
    std::shared_ptr<Alien> new_jellyfish(core::Point starting_position);
    std::shared_ptr<Alien> new_tadpole(core::Point starting_position);
    std::shared_ptr<Alien> new_octopus(core::Point starting_position);
    std::shared_ptr<Alien> new_crab(core::Point starting_position);
};
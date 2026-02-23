#pragma once

#include "engine/core.hpp"
#include "engine/entity.hpp"
#include "engine/scene.hpp"
#include "engine/sprites.hpp"
#include "player.hpp"
#include <SDL3/SDL.h>
#include <functional>
#include <memory>
#include <vector>

class AlienExplosionOrchestrator;

class AlienProjectile : public Entity, public Collidable, public Drawable {
  private:
    static constexpr float DRAW_WIDTH = 60;
    static constexpr float DRAW_HEIGHT = 60;
    static constexpr float SPEED = 3;

    std::unique_ptr<Animation> animation;
    float x;
    float y;
    bool deleted;
    std::shared_ptr<AlienExplosionOrchestrator> explosions;

  public:
    AlienProjectile(
        std::shared_ptr<SDL_Texture> texture,
        core::Point starting_position,
        std::shared_ptr<AlienExplosionOrchestrator> explosions
    );
    std::string get_type() const override;
    bool is_deleted() const override;
    void mark_deleted();
    void update();

    std::optional<std::reference_wrapper<Collidable>> as_collidable() override;
    core::Rect get_hitbox() const override;
    void collide_with(CollideCtx const &ctx, Collidable &other) override;
    std::optional<std::reference_wrapper<Drawable>> as_drawable() override;
    void draw(SDL_Renderer *renderer) const override;
};

class AlienExplosion : public Entity, public Drawable {
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
    void update();

    std::optional<std::reference_wrapper<Drawable>> as_drawable() override;
    void draw(SDL_Renderer *renderer) const override;
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
    std::shared_ptr<AlienExplosionOrchestrator> explosions;
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
    std::shared_ptr<AlienExplosionOrchestrator> explosions;

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

class AlienProjectileOrchestrator {
  private:
    std::vector<std::shared_ptr<AlienProjectile>> projectiles;

  public:
    void add(std::shared_ptr<AlienProjectile> projectile);
    void update();
    void delete_all();
};

class AlienExplosionOrchestrator {
  private:
    std::vector<std::shared_ptr<AlienExplosion>> explosions;

  public:
    void add(std::shared_ptr<AlienExplosion> explosion);
    void update();
};

class BGMOrchestrator {
  private:
    int cur_arp_idx = 0;

  public:
    void play(SceneCtx const &ctx);
    void reset();
};

class AlienOrchestrator : public Entity, public PlayerStatusNotifier {
  private:
    static constexpr int TICKS_PER_MOVE = 58;
    static constexpr int TICKS_PER_SHOOT_CHANCE = 30;
    static constexpr int ALIEN_SHOOT_CHANCE = 3; // The chance is the reciprocal of this number

    AlienProjectileOrchestrator projectiles;
    std::shared_ptr<AlienExplosionOrchestrator> explosions;
    BGMOrchestrator bgm;

    int tick_counter;
    int shot_tick_counter;
    std::vector<std::shared_ptr<Alien>> aliens;
    bool is_player_dead;
    int player_lives;

  public:
    AlienOrchestrator(std::shared_ptr<AlienExplosionOrchestrator> explosions);
    std::string get_type() const override;
    void add_alien(std::shared_ptr<Alien> alien);
    void update(SceneCtx const &ctx);

    void notify_player_died(int remaining_lives) override;
    void notify_player_rerack(int remaining_lives) override;
};

class AlienFactory {
  private:
    SceneCtx ctx;
    std::shared_ptr<SDL_Texture> texture;
    std::shared_ptr<ScoreNotifier> score_notifier;
    std::shared_ptr<AlienExplosionOrchestrator> explosions;

  public:
    AlienFactory(
        SceneCtx ctx,
        std::shared_ptr<SDL_Texture> texture,
        std::shared_ptr<ScoreNotifier> score_notifier,
        std::shared_ptr<AlienExplosionOrchestrator> explosions
    );
    std::shared_ptr<Alien> new_jellyfish(core::Point starting_position);
    std::shared_ptr<Alien> new_tadpole(core::Point starting_position);
    std::shared_ptr<Alien> new_octopus(core::Point starting_position);
    std::shared_ptr<Alien> new_crab(core::Point starting_position);
};
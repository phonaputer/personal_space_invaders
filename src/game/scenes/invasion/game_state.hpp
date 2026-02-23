#pragma once

#include <memory>
#include <vector>

class GameStateNotifier {
  public:
    virtual ~GameStateNotifier() = default;
    virtual void notify_game_start() {};
    virtual void notify_game_over() {};
    virtual void notify_player_died() {};
    virtual void notify_player_rerack() {};
    virtual void notify_player_lives_change([[maybe_unused]] int new_current_lives) {};
    virtual void notify_score_change([[maybe_unused]] int new_current_score, [[maybe_unused]] int new_high_score) {};
};

class PlayerDeathNotifier {
  public:
    virtual ~PlayerDeathNotifier() = default;
    virtual void notify_player_died() = 0;
};

class AlienOverrunNotifier {
  public:
    virtual ~AlienOverrunNotifier() = default;
    virtual void notify_aliens_overran_earth() = 0;
};

class ScoreNotifier {
  public:
    virtual ~ScoreNotifier() = default;
    virtual void notify_scored(int amount) = 0;
};

class GameStateOrchestrator : public PlayerDeathNotifier, public AlienOverrunNotifier, public ScoreNotifier {
  private:
    static constexpr int MAX_LIVES = 3;
    static constexpr int PLAYER_DEATH_TICKS = 150;
    static constexpr int GAME_OVER_TICKS = 300;

    int cur_lives = MAX_LIVES;
    int player_death_tick_counter = 0;
    bool player_is_dead = false;
    int game_over_tick_counter = 0;
    bool game_is_over = false;
    int current_score = 0;
    int high_score = 0;

    std::vector<std::weak_ptr<GameStateNotifier>> notifiers;

  public:
    void add_notifier(std::weak_ptr<GameStateNotifier> notifier);
    void update();
    void restart_game();
    void notify_player_died() override;
    void notify_aliens_overran_earth() override;
    void notify_scored(int amount) override;
};
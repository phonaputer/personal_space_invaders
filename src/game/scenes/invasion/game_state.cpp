#include "game_state.hpp"
#include <memory>
#include <vector>

void GameStateOrchestrator::add_notifier(std::weak_ptr<GameStateNotifier> notifier) {
  notifiers.push_back(notifier);
}

void GameStateOrchestrator::update() {
  if (game_is_over) {
    game_over_tick_counter++;

    if (game_over_tick_counter >= GAME_OVER_TICKS) {
      restart_game();
    }

    return;
  }

  if (player_is_dead) {
    player_death_tick_counter++;
    if (player_death_tick_counter >= PLAYER_DEATH_TICKS) {
      player_death_tick_counter = 0;
      player_is_dead = false;
      for (const auto &notifier : notifiers) {
        notifier.lock()->notify_player_rerack();
      }
    }
  }
}

void GameStateOrchestrator::restart_game() {
  game_over_tick_counter = 0;
  player_death_tick_counter = 0;
  game_is_over = false;
  player_is_dead = false;
  cur_lives = MAX_LIVES;
  current_score = 0;

  for (const auto &notifier : notifiers) {
    notifier.lock()->notify_game_start();
  }
  for (const auto &notifier : notifiers) {
    notifier.lock()->notify_player_rerack();
  }
  for (const auto &notifier : notifiers) {
    notifier.lock()->notify_player_lives_change(cur_lives);
  }
  for (const auto &notifier : notifiers) {
    notifier.lock()->notify_score_change(current_score, high_score);
  }
}

void GameStateOrchestrator::notify_player_died() {
  cur_lives--;
  for (const auto &notifier : notifiers) {
    notifier.lock()->notify_player_lives_change(cur_lives);
  }

  player_is_dead = true;
  for (const auto &notifier : notifiers) {
    notifier.lock()->notify_player_died();
  }

  if (cur_lives < 1) {
    game_is_over = true;
    for (const auto &notifier : notifiers) {
      notifier.lock()->notify_game_over();
    }
  }
}

void GameStateOrchestrator::notify_aliens_overran_earth() {
  game_is_over = true;
  for (const auto &notifier : notifiers) {
    notifier.lock()->notify_game_over();
  }
}

void GameStateOrchestrator::notify_scored(int amount) {
  current_score += amount;
  if (high_score < current_score) {
    high_score = current_score;
  }

  for (const auto &notifier : notifiers) {
    notifier.lock()->notify_score_change(current_score, high_score);
  }
}
#include "input.hpp"
#include <set>

bool UserInputs::is_engaged(PlayerInput input) const {
  return current_inputs.contains(input);
}

bool UserInputs::is_initiated(PlayerInput input) const {
  return current_initiations.contains(input);
}

void UserInputs::engage(PlayerInput input) {
  current_inputs.insert(input);
}

void UserInputs::disengage(PlayerInput input) {
  current_inputs.erase(input);
}

void UserInputs::update() {
  current_initiations.clear();

  for (auto input : current_inputs) {
    if (!previous_tick_inputs.contains(input)) {
      current_initiations.insert(input);
    }
  }

  previous_tick_inputs = current_inputs;
}
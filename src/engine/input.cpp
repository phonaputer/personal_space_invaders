#include "input.hpp"
#include <iostream>
#include <set>

bool UserInputs::is_engaged(PlayerInput input) {
  return current_inputs.contains(input);
}

void UserInputs::engage(PlayerInput input) {
  current_inputs.insert(input);
}

void UserInputs::disengage(PlayerInput input) {
  current_inputs.erase(input);
}
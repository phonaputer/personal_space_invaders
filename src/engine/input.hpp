#pragma once

#include <set>

enum PlayerInput { LEFT, RIGHT, FIRE };

class UserInputs {
  private:
    std::set<PlayerInput> current_inputs;

  public:
    bool is_engaged(PlayerInput input) const;
    void engage(PlayerInput input);
    void disengage(PlayerInput input);
};
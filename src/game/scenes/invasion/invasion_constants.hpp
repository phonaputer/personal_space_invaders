#pragma once

#include <string>

namespace entityType {

const std::string PLAYER = "player";
const std::string PLAYER_PROJECTILE = "player-projectile";

const std::string ALIEN = "alien";
const std::string ALIEN_PROJECTILE = "alien-projectile";
const std::string ALIEN_EXPLOSION = "alien-explosion";
const std::string ALIEN_ORCHESTRATOR = "alien-orchestrator";

const std::string HUD = "hud";

const std::string FORTRESS_TILE = "fortress-tile";

}; // namespace entityType

namespace image {

const std::string PRIMARY_SPRITESHEET = "primary-spritesheet";

};

namespace sound {

const std::string ALIEN_EXPLOSION = "alien-explosion";
const std::string ALIEN_SHOT = "alien-shot";
const std::string PLAYER_EXPLOSION = "player-explosion";
const std::string PLAYER_SHOT = "player-shot";

const std::string ARP_1 = "arp-1";
const std::string ARP_2 = "arp-2";
const std::string ARP_3 = "arp-3";
const std::string ARP_4 = "arp-4";

const std::string MENU_SELECT = "menu-select";

} // namespace sound

namespace zindex {

const unsigned int PAUSE_MENU = 101;
const unsigned int HUD = 100;
const unsigned int ALIEN_EXPLOSION = 50;
const unsigned int PLAYER_PROJECTILE = 31;
const unsigned int ALIEN_PROJECTILE = 30;
const unsigned int ALIEN = 22;
const unsigned int PLAYER = 21;
const unsigned int FORTRESS = 20;
const unsigned int GROUND = 1;

} // namespace zindex

const float GROUND_Y = 745;
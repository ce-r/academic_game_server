#include "GameLogic.hpp"

void GameLogic::add_player(const std::string &player_id) {
    players[player_id] = 0;
    std::cout << "[Game] Player added: " << player_id << std::endl;
}

void GameLogic::process_move(const std::string &player_id) {
    if (players.find(player_id) != players.end()) {
        players[player_id] += 1;
        std::cout << "[Game] Player " << player_id << " moved to position " << players[player_id] << std::endl;
    }
}
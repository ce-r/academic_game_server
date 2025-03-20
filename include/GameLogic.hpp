#ifndef GAME_LOGIC_HPP
#define GAME_LOGIC_HPP

#include <iostream>
#include <unordered_map>
#include <string>

class GameLogic {
    public:
        void add_player(const std::string &player_id);
        void process_move(const std::string &player_id);

    private:
        std::unordered_map<std::string, int> players;
};

#endif
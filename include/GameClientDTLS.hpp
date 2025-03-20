#ifndef GAME_CLIENT_DTLS_HPP
#define GAME_CLIENT_DTLS_HPP

#include "TTU_Client.hpp"
#include <string>

class GameClientDTLS : public TTU_Client {
    public:
        GameClientDTLS(int domain, int protocol, int port, uint32_t interface);
        void send_game_action(const std::string &action);
        bool pending_message();
        std::string receive_message();
    };

#endif
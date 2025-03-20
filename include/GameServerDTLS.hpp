#ifndef GAME_SERVER_DTLS_HPP
#define GAME_SERVER_DTLS_HPP

#include "GameLogic.hpp"
#include "TTU_Server.hpp"
#include <unordered_map>
#include <openssl/ssl.h>
#include <netinet/in.h>

class GameServerDTLS : public TTU_Server {
    public:
        GameServerDTLS(const std::string &crt, const std::string &key, int domain, int protocol, int port, uint32_t interface, int backlog);
        void handle_dtls_game(SSL_CTX* dtls_ctx, int udp_sock, struct sockaddr_in &client_addr);
        void process_game_message(const std::string &msg, const std::string &client_key);
        void set_running(bool state);
        int get_udp_socket() const;
    private:
        std::unordered_map<std::string, struct sockaddr_in> active_clients;
        GameLogic game_logic;
        bool running = true;
        std::unique_ptr<BindingSocket> udp_socket;
};

#endif // GAME_SERVER_DTLS_HPP
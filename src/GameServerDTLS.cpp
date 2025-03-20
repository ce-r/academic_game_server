#include "GameServerDTLS.hpp"

GameServerDTLS::GameServerDTLS(const std::string &crt, const std::string &key, int domain, int protocol, int port, uint32_t interface, int backlog)
    : TTU_Server(crt, key, domain, SOCK_DGRAM, protocol, port, interface, backlog) {}

void GameServerDTLS::handle_dtls_game(SSL_CTX* dtls_ctx, int udp_sock, struct sockaddr_in &client_addr) {
    char buffer[1024];
    socklen_t client_len = sizeof(client_addr);

    while (running) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recvfrom(udp_sock, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len);
        if (bytes <= 0) continue;

        std::string client_key = inet_ntoa(client_addr.sin_addr) + std::to_string(ntohs(client_addr.sin_port));
        active_clients[client_key] = client_addr;
        process_game_message(std::string(buffer, bytes), client_key);
    }
}

void GameServerDTLS::process_game_message(const std::string &msg, const std::string &client_key) {
    if (msg == "PLAYER_JOIN") {
        game_logic.add_player(client_key);
    } else if (msg == "MOVE") {
        game_logic.process_move(client_key);
    }
}

void GameServerDTLS::set_running(bool state) {
    running = state;
}


int GameServerDTLS::get_udp_socket() const {
    return udp_socket ? udp_socket->get_sock() : -1; 
}
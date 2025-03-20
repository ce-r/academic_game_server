#include "GameClientDTLS.hpp"

GameClientDTLS::GameClientDTLS(int domain, int protocol, int port, uint32_t interface)
    : TTU_Client(domain, SOCK_DGRAM, protocol, port, interface) {}

void GameClientDTLS::send_game_action(const std::string &action) {
    send_udp_message(action);
}

bool GameClientDTLS::pending_message() {
    struct timeval timeout = {0, 50000}; // 50ms timeout
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(get_udp_socket()->get_sock(), &fds);
    
    int result = select(get_udp_socket()->get_sock() + 1, &fds, nullptr, nullptr, &timeout);
    return result > 0 && FD_ISSET(get_udp_socket()->get_sock(), &fds);
}

std::string GameClientDTLS::receive_message() {
    char buffer[1024];
    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(server_addr);

    int bytes = recvfrom(get_udp_socket()->get_sock(), buffer, sizeof(buffer), 0, (struct sockaddr*) &server_addr, &server_len);
    if (bytes > 0) {
        buffer[bytes] = '\0';
        return std::string(buffer);
    }
    return "";
}
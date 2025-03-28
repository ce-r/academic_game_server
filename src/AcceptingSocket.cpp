

#include "AcceptingSocket.hpp"


AcceptingSocket::AcceptingSocket(int domain, int service, int protocol, int port, uint32_t interface, int bklg)
                                        : ListeningSocket(domain, service, protocol, port, interface, bklg) {}

AcceptingSocket::~AcceptingSocket() {
    close(get_sock());
}

int AcceptingSocket::acceptserv(int sock, const struct sockaddr_in &server_address) {
    struct sockaddr_in client_address;
    socklen_t addrlen = sizeof(client_address);

    accept_sock = accept(sock, (struct sockaddr*) &client_address, &addrlen);

    if (accept_sock < 0) {
        perror("[AcceptingSocket] Accept failed");
    } else {
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
        std::cout << "[AcceptingSocket] Accepted connection from " << client_ip
                  << ":" << ntohs(client_address.sin_port) << std::endl;
    }

    return accept_sock;
}

int AcceptingSocket::get_accepting() const {
    return accept_sock;
}
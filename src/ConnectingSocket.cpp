#include "ConnectingSocket.hpp"
#include <string.h>


ConnectingSocket::ConnectingSocket(int domain, int service, int protocol, int port, uint32_t interface)
                                            : Socket(domain, service, protocol, port, interface) {
    std::cout << "[ConnectingSocket] Raw interface value before setting address: " << interface << std::endl;
    struct sockaddr_in address = get_address(); // must be modifiable
    address.sin_family = domain;
    address.sin_port = htons(port);
   
    if (interface == INADDR_LOOPBACK) {  
        inet_pton(AF_INET, "127.0.0.1", &address.sin_addr);
    } else {
        address.sin_addr.s_addr = htonl(interface);  
    }
    set_address(address);
}

ConnectingSocket::~ConnectingSocket() {
    close(get_sock());
}

int ConnectingSocket::conn2serv(int sock, const struct sockaddr_in &address) {
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &address.sin_addr, ip_str, INET_ADDRSTRLEN);

    std::cout << "[ConnectingSocket] Attempting to connect to " << ip_str << ":" << ntohs(address.sin_port) << std::endl;
    std::cout << "[ConnectingSocket] Attempting to connect to "
              << inet_ntoa(address.sin_addr)
              << ":" << ntohs(address.sin_port) << std::endl;
    if (connect(sock, (struct sockaddr*) &address, sizeof(address)) < 0) {
        perror("[ConnectingSocket] TCP connect failed");
        throw std::runtime_error("Connection to server failed");
    }
   
    std::cout << "[ConnectingSocket] TCP connection established." << std::endl;
    return sock;
}

int ConnectingSocket::bind2self(int sock, const struct sockaddr_in &address) {
    // Clients typically don't bind, so this method will
    // not call bind() and will return success
    return 0;
}
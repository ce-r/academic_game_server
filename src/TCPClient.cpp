

#include "TCPClient.hpp"


TCPClient::TCPClient(int domain, int service, int protocol, int port, uint32_t interface) {
    cli_socket = new ConnectingSocket(domain, service, protocol, port, interface);
    cli_socket->conn2serv(cli_socket->get_sock(), cli_socket->get_address());
}

TCPClient::~TCPClient() {
    delete cli_socket;
}

ConnectingSocket* TCPClient::get_cli_socket() const {
    return cli_socket;
} 

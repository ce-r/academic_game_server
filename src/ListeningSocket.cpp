#include "ListeningSocket.hpp"


ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, uint32_t interface, int bklg)
                                : BindingSocket(domain, service, protocol, port, interface) {
    backlog = bklg;
    listen4client();
    test_sock(listening);
}

ListeningSocket::~ListeningSocket() {
    close(get_sock());
}

void ListeningSocket::listen4client() {
    listening = listen(get_sock(), backlog);
    if (listening < 0) {
        throw std::runtime_error("Failed to listen on socket...");
    }
}

int ListeningSocket::get_listening() {
    return listening;
}

int ListeningSocket::get_backlog() {
    return backlog;
}

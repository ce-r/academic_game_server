#ifndef BindingSocket_HPP
#define BindingSocket_HPP

#include "Socket.hpp"


class BindingSocket : public Socket {
    private:
        int binding;
        int socket_type; 
        int bind2self(int sock, const struct sockaddr_in &address) override;
    
    public: 
        BindingSocket(int domain, int service, int protocol, int port, uint32_t interface);
        ~BindingSocket();
        int get_binding() const;
};

#endif

#ifndef TTU_Client_HPP
#define TTU_Client_HPP

#include "TTU_ChatBase.hpp"
#include "TCPClient.hpp"
#include "UDPClient.hpp"


class TTU_Client : public TTU_ChatBase, public UDPClient, public TCPClient {
    public:
        TTU_Client(int domain, int service, int protocol, int port, uint32_t interface);
        void launch();
};

#endif
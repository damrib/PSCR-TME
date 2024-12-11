#include "Socket.h"

namespace pr {

    inline std::ostream & operator<< (std::ostream & os, struct sockaddr_in * addr) {
        char host[1024];
        if ( getnameinfo ((sockaddr*) addr, sizeof(struct sockaddr_in), host, 1024, NULL, 0, 0) == 0 ) {
            os << "host= " << host << std::endl;
        }
        os << inet_ntoa(addr->sin_addr) << ":" << ntohs << std::endl;
        return os;
    }

    void Socket::connect(const std::string & host, int port) {
        struct addrinfo* result;
        if ( getaddrinfo (host.c_str(), nullptr, nullptr, &result) != 0) {
            perror("resolution_DNS");
        } 

        struct in_addr ip;
        for (struct addrinfo* rp = result; rp != nullptr; rp = rp->ai_next) {
            if ( rp->ai_family == AF_INET ) {
                ip = ((struct sockaddr_in*) (rp->ai_addr))->sin_addr;
                break;
            }
        }

        freeaddrinfo(result);
        connect(ip, port);
    }

    void Socket::connect(in_addr ip, int port) {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr = ip;
        fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

        if ( fd == -1 ) {
            perror("socket");
            return;
        }

        if ( ::connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("connect");
            ::close(fd);
            fd = -1;
            return ;
        }
    }

    void Socket::close() {
        if (fd != -1) {
            std::cout << "client socket closed" << std::endl;
            shutdown(fd, 2);
            ::close(fd);
            fd = -1;
        }
    }

    //Socket::~Socket() { close(); }

}
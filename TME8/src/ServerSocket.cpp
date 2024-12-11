#include "ServerSocket.h"

namespace pr {

    ServerSocket::ServerSocket(int port) {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;
        socketfd = socket(AF_INET, SOCK_STREAM, 0);
        if ( socketfd < 0) return;
        if ( bind (socketfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
            perror("bind");
            ::close(socketfd);
            socketfd = -1;
            return ;
        }

        if (listen(socketfd, 10) < 0) {
            perror("listen");
            ::close(socketfd);
            socketfd = -1;
            return;
        }
    }

    Socket ServerSocket::accept() {
        if ( ! isOpen() ) { return Socket(); }
        struct sockaddr exp;
        socklen_t size = (socklen_t) sizeof(exp);
        int fdcom = ::accept(socketfd, &exp, &size);
        if (fdcom < 0) {
            perror("accept");
            return Socket();
        }
        std::cout << "Incoming connection" << &exp << std::endl;
        return Socket(fdcom);
    }

    void ServerSocket::close() {
        if ( socketfd != -1 ) {
            std::cout << "server socket closed" << std::endl;
            shutdown(socketfd, 2);
            ::close(socketfd);
            socketfd = - 1;
        }
    }

}
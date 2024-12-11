#include "TCPServer.h"

namespace pr {

    void TCPServer::startServer(int port) {
        ss = new ServerSocket(port); 
        connections.emplace_back([&](){
            Socket scan;
            while ( isOperating ) {
                fd_set mselect;
                FD_ZERO(&mselect);
                FD_SET(0, &mselect);
                FD_SET(ss->getFD(), &mselect);
                if (select(ss->getFD() + 1, &mselect, nullptr, nullptr, nullptr) == -1) {
                    std::cerr << "error in select" << std::endl;
                    return;
                }

                if (FD_ISSET(ss->getFD(), &mselect)){
                    scan = ss->accept();
                    if ( scan.isOpen() ) {
                        handler = handler->clone();
                        connections.emplace_back(&ConnectionHandler::handleConnection, handler, std::ref(scan));
                    }
                } else break;
            
            }
        });
    }

    void TCPServer::stopServer() {
        isOperating = false;
        for ( auto& thr: connections ) {
            thr.join();
        }
        ss->close();
        delete ss;
        //delete handler;
    }

}
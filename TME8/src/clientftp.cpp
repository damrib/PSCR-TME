#include "Socket.h"

#include <iostream>
#include <string>

#define MAX_LENGTH 16
#define MAX_LINE 256

bool input(int fd, fd_set* mselect) {
    FD_ZERO(mselect);
    FD_SET(0, mselect);
    FD_SET(fd+1, mselect);
    select(fd+1, mselect, nullptr, nullptr, nullptr);

    if (FD_ISSET(fd, mselect)) {
        std::cout << "Serveur a interrompu la connexion" << std::endl;
        return false;
    }
    return true;
}

int main(int argc, char** argv) {

    if ( argc != 3 ) {
        std::cerr << "Mauvais nombre d'arguments" << std::endl;
        exit(1);
    }

    pr::Socket sock;

    int port = atoi(argv[2]);

    sock.connect(argv[1], port);

    if ( sock.isOpen() ) {
        int fd = sock.getFD();

        while (true) {
            
            std::string s;

            fd_set mselect;
            if ( ! input(fd, &mselect) ) break;

            std::getline(std::cin, s);
            write(fd, s.data(), sizeof(char) * MAX_LENGTH);

            char buffer[MAX_LINE];
            if ( s == "UPLOAD" ) {
                std::cout << "name of file to upload: " << std::flush;

                if ( ! input(fd, &mselect) ) break;
                std::getline(std::cin, s);

                write(fd, s.data(), sizeof(char) * MAX_LENGTH);

                FILE* fdo = fopen(s.data(), "r");
                while ( fgets(buffer, sizeof(char) * MAX_LINE, fdo) ) {
                    write(fd, buffer, sizeof(char) * MAX_LINE);
                }
                buffer[0] = '\0';
                write(fd, buffer, sizeof(char));

                fclose(fdo);
            } else if ( s == "DOWNLOAD" ) {
                std::cout << "name of file to download: " << std::flush;

                if ( ! input(fd, &mselect) ) break;
                std::getline(std::cin, s);

                write(fd, s.data(), sizeof(char) * MAX_LENGTH);

                char path[50];
                sprintf(path, "%s", s.data());
                FILE* fdo = fopen(path, "w");
                if ( fdo == nullptr) {
                    std::cout << "couldn't open file" << std::endl;
                }
                buffer[0] = 'a';
                while ( read(fd, buffer, sizeof(char) * MAX_LINE) > 0 && buffer[0] != '\0') {
                    fprintf(fdo, "%s", buffer);
                }
                fclose(fdo);
                std::cout << "download finished" << std::endl;

            } else if ( s == "LIST" ) {
                while ( read(fd, buffer, sizeof(char) * MAX_LINE) > 0 && buffer[0] != '\0') {
                    std::cout << buffer << std::endl;
                }
            } else if ( s == "QUIT") 
                break;

        }

    }

    sock.close();

    return 0;
}


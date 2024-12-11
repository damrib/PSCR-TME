#include "TCPServer.h"
#include <iostream>
#include <sys/types.h>
#include <cstring>
#include <stdio.h>
#include <dirent.h>

#define MAX_LENGTH 16
#define MAX_LINE 256

class FTPConnections : public pr::ConnectionHandler {

    private:

        DIR* dir;
        std::string path;

        bool readFD(int fd, char* buf, ssize_t len) {
            auto nblu = read(fd, buf, len);
            if ( nblu == 0 ) {
                std::cout << "fin de connexion par le client" << std::endl;
                return false;
            } else if (nblu > len) {
                std::cerr << "error in read" << std::endl;
                return false;
            }
            return true;
        }

        void finishedOperation(int fd) {
            char c = '\0';
            write(fd, &c, sizeof(char));
        }

    public:

        FTPConnections(DIR* d, const char* p) : dir(d), path(p) {
        }

        void handleConnection(pr::Socket& sc) {
            int fd = sc.getFD();

            ssize_t msz = sizeof(char) * MAX_LENGTH;
            while (1) {
                char buffer[MAX_LENGTH];
                if ( ! readFD(fd, buffer, msz) ) break;

                struct dirent* file;
                char line_buffer[MAX_LINE];

                if ( ! strcmp(buffer, "LIST") ) {

                    file = readdir(dir);
                    while( file != NULL ) {
                        write(fd, file->d_name, sizeof(char) * MAX_LINE);
                        file = readdir(dir);
                    }
                    std::cout << "reached end of directory" << std::endl;
                                        
                    finishedOperation(fd); // signals end of operations
                    rewinddir(dir); // readies the pointer for a new request LIST

                } else if ( ! strcmp(buffer, "UPLOAD") ) {
                    if ( readFD(fd, buffer, msz) ) {
                        std::cout << "start of upload on server" << std::endl;
                        
                        FILE* fdo;
                        char fpath[MAX_LINE];
                        sprintf(fpath, "%s/%s", path.data(), buffer);
                        fdo = fopen(fpath, "w");

                        if ( fdo == nullptr ){
                            std::cerr << "error during upload" << std::endl;
                        } else {
                            while ( read(fd, line_buffer, sizeof(char) * MAX_LINE) > 0 && line_buffer[0] != '\0' ) {
                                fprintf(fdo, "%s", line_buffer);
                            }
                            fclose(fdo);
                            std::cout << "upload on server finished" << std::endl;
                        }

                    }
                } else if ( ! strcmp(buffer, "DOWNLOAD") ) {
                    if ( readFD(fd, buffer, msz) ) {
                        std::cout << "start of download on server" << std::endl;

                        FILE* fdo;
                        char fpath[MAX_LINE];
                        sprintf(fpath,"%s/%s", path.data(), buffer);
                        fdo = fopen(fpath, "r");

                        if (fdo == nullptr) {
                            std::cerr << "error during download" << std::endl;
                        } else {
                            while ( fgets(line_buffer, sizeof(char) * MAX_LINE, fdo) ) {
                                write(fd, line_buffer, sizeof(char) * MAX_LINE);
                            }
                            fclose(fdo);  
                            std::cout << "download on server finished" << std::endl;                          
                        }
                        finishedOperation(fd);
                    }
                }

            }
            sc.close();
        }

        ConnectionHandler* clone() const {
            return new FTPConnections(dir, path.data());
        }

};

int main(int argc, char** argv) {

    if ( argc != 3 ){
        std::cerr << "mauvais nombre d'arguments" << std::endl;
        exit(1);
    }

    int port = atoi(argv[1]);
    char* d = argv[2];

    DIR *dir = opendir(d);

    if (dir == nullptr) {
        std::cerr << "couldn't find directory" << std::endl;
        exit(1);
    }

    pr::TCPServer server(new FTPConnections(dir, d));

    server.startServer(port);

    std::string s;
    std::cin >> s;

    std::cout << "Interruption Serveur" << std::endl;
    server.stopServer();
    std::cout << "Serveur Shutdown" << std::endl;

    return 0;
}
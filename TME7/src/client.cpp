#include <iostream>
#include <thread>
#include <string>

#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>

#include "chat_common.h"

bool flag = true;

void handler(int sig) {
    flag = false;
    signal(SIGINT, SIG_DFL);
}

void transmit(const char* content, myshm* server, long mode) {

    sem_wait(&(server->isFull)); // sem_wait sur isFull avant sem pour éviter deadlock
    sem_wait(&(server->sem));

    server->messages[(server->read + server->write) % MAX_MESS].type = mode;
    memcpy(server->messages[(server->read + server->write) % MAX_MESS].content, content, TAILLE_MESS);
    ++(server->write);
    
    sem_post(&(server->sem));
}

void read_message(const char* id, message* received, myshm * server) {
    // Connection
    transmit(id, server, 0);

    while ( flag && received->type != 2 ) {
        if ( received->type == 1 ) {
            std::cout << received->content << std::endl;
            received->type = 0;
        }
    }

    handler(0);

    // Disconnection
    if ( received->type != 2 )
        transmit(id, server, 2);  

}

void write_message(myshm* server) {

    std::string s;
    while ( flag ) {
        std::getline(std::cin, s);
        if ( flag ) transmit(s.data(), server, 1);
    }

}

int main(int argc, char** argv) {

    if ( argc != 3) {
        std::cerr << "Client needs two identifier" << std::endl;
        exit(1);
    }

    int fd = shm_open(argv[1], O_RDWR | O_CREAT, 0600);
    if ( fd == -1 ){
        std::cerr << "1 failed to open shared memory" << std::endl;
        exit(1);
    }

    if ( ftruncate(fd, sizeof(message)) == -1 ){
        std::cerr << "1 failled to allocate mem for shared mem" << std::endl;
        close(fd);
        unlink(argv[1]);
        exit(1);
    }

    message* msg = (message*) mmap(0, sizeof(message), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if ( msg == MAP_FAILED ){
        std::cerr << "1 failed to map shared mem" << std::endl;
        exit(1);
    }

    msg->type = 0;

    int fd_serv = shm_open(argv[2], O_RDWR, 0600);
    if ( fd_serv == -1 ){
        std::cerr << "2 failed to open shared memory" << std::endl;
        exit(1);
    }

    /**if ( ftruncate(fd_serv, sizeof(myshm)) == -1 ){
        std::cerr << "2 failled to allocate mem for shared mem" << std::endl;
        exit(1);
    }**/

    myshm * serv_data = (myshm*) mmap(0, sizeof(myshm), PROT_WRITE, MAP_SHARED, fd_serv, 0);
    if ( serv_data == MAP_FAILED ){
        std::cerr << "2 failed to map shared mem" << std::endl;
        exit(1);
    }

    signal(SIGINT, &handler);

    std::thread t_write(&write_message, serv_data);
    read_message(argv[1], msg, serv_data);

    t_write.join();

    close(fd);
    munmap(msg, sizeof(message));
    munmap(serv_data, sizeof(myshm));

    unlink(argv[1]);

    close(fd_serv);

    return 0;
}
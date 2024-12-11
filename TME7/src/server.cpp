#include <unordered_map>
#include <iostream>

#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#include "chat_common.h"

bool flag = true;

void handler(int sig){
    flag = false;
}

message* open_msg(const char* name) {
    int fd = shm_open(name, O_RDWR, 0600);
    if ( fd == -1 ){
        std::cerr << "failed to open shared_mem" << std::endl;
        exit(1);
    }
    message * msg = (message*) mmap(0, sizeof(message), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if ( msg == MAP_FAILED ) {
        std::cerr << "failed to map" << std::endl;
        exit(1);
    }    

    close(fd);
    return msg;
}

void treat_message(std::unordered_map<std::string, message*>& connected, myshm * shm){
    sem_wait(&(shm->sem));
    long type = shm->messages[shm->read % MAX_MESS].type;
    std::string content(shm->messages[shm->read % MAX_MESS].content);

    if ( type == 0 ) {
        std::cout << connected.insert(std::pair<std::string, message*>(content, open_msg(content.data()))).second;
        std::cout << " new_connection: " << content << std::endl;
    } else if ( type == 1 ) {
        std::cout << "message: " << content << std::endl;
        for ( const auto& sh : connected ){
            sh.second->type = type;
            memcpy(sh.second->content, content.data(), TAILLE_MESS);
            ++(shm->nb);
        }
    } else if ( type == 2 ){
        auto node = connected.extract(content);
        munmap(node.mapped(), sizeof(message));
        std::cout << "disconnection: " << content << std::endl;
    }

    ++(shm->read);
    --(shm->write);
    sem_post(&(shm->isFull));
    sem_post(&(shm->sem));
}

void disconnect_all(std::unordered_map<std::string, message*> connected) {
    for ( const auto& sh : connected ) {
        sh.second->type = 2;
        munmap(sh.second, sizeof(message));
    }
}

int main(int argc, char** argv) {

    if ( argc != 2 ){
        std::cerr << "name of server expected" << std::endl;
        exit(1);
    }

    int fd = shm_open(argv[1], O_RDWR | O_CREAT, 0600);
    if ( fd == -1 ){
        std::cerr << "failed to open new shared memory" << std::endl;
        exit(1);
    }

    if ( ftruncate(fd, sizeof(myshm)) == -1){
        std::cerr << "failed to allocate mem for shared memory" << std::endl;
        exit(1);
    }

    myshm * shm = ( myshm* ) mmap(nullptr, sizeof(myshm), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if ( shm == MAP_FAILED ){
        std::cerr << "Failed to map" << std::endl;
        exit(1);
    }
    shm->read = 0;
    shm->write = 0;
    shm->nb = 0;
    sem_init(&(shm->sem), 1, 1);
    sem_init(&(shm->isFull), 1, MAX_MESS);

    std::unordered_map<std::string, message*> connected_proc(7);

    signal(SIGINT, &handler);
    while ( flag ){
        if ( shm->write > 0 ){
            treat_message(connected_proc, shm);
        }
    }

    disconnect_all(connected_proc);

    close(fd);
    munmap(shm, sizeof(myshm));
    unlink(argv[1]);

    return 0;
}
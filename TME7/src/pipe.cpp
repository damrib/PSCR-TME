#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <cstring>
#include <stdlib.h>
#include <cstdio>
#include <iostream>

int main(int argc, char** argv) {

    int i = 0;
    int index;

    while( i < argc ) {

        if (! strcmp(argv[i], "|")) {
           index = i+1; 
           argv[i] = nullptr;
           break;
        }
        ++i;
    }

    int fds[2];
    if ( pipe(fds) == -1){
        perror("Errors on pipe");
        exit(1);
    }

    int fils;
    if ( (fils = fork()) == -1) {
        perror("Error on fork");
        exit(2);
    }


    if ( fils == 0 ) {

        dup2(fds[1], STDOUT_FILENO);
        close(fds[0]); close(fds[1]); 

        if ( execv(argv[1], argv+1) == -1) {
            perror("error execv");
            exit(3);
        }

        return 0;
    } else {

        dup2(fds[0], STDIN_FILENO);
        close(fds[1]); close(fds[0]); 

        if ( execv(argv[index], argv+index) == -1) {
            perror("error execv");

            int status;
            wait(&status);
            
            exit(3);
        }

    }

    return 0;
}
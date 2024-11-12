#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <cstdlib>
#include <iostream>

void setHandler_for(int sig, sighandler_t f) {
  struct sigaction act;
  sigfillset(&act.sa_mask);
  act.sa_flags = 0;
  act.sa_handler = f;
  sigaction(sig, &act, NULL);
}

void alarmFired(int sig) {
    std::cout << "Alarme declenchée" << std::endl;
}

int wait_till_pid(pid_t pid, int sec) {
    setHandler_for(SIGALRM, &alarmFired);
    alarm(sec);

    int status;
    pid_t fils = -1;

    do {
        fils = wait(&status);
    } while( fils != pid && fils != -1);

    alarm(0);
    if ( WIFEXITED(status) && fils != -1) {
        return 0;
    } else return -1;

}

int main() {

    int fils = fork();
    if ( fils == 0 ) {
        sleep(5);
        return 0;
    } else {
        std::cout << wait_till_pid(fils, 6) << std::endl;
    }

    int status;
    wait(&status);

    return 0;
}
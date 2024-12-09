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

int wait_till_pid(pid_t pid) {

    int status;
    pid_t fils = -1;

    do {
        fils = wait(&status);
    } while( fils != pid && fils != -1);

    if ( WIFEXITED(status) && fils != -1) {
        return fils;
    } else return -1;

}

int wait_till_pid_signal(pid_t pid, int sec) {
    alarm(sec);

    sigset_t sigmask;
    sigemptyset(&sigmask);
    sigaddset(&sigmask, SIGCHLD);
    sigaddset(&sigmask, SIGALRM);
    sigprocmask(SIG_BLOCK, &sigmask, NULL);

    int fils, status;
    int sig;
    do {
        sigwait(&sigmask, &sig);
        fils = wait(&status);
        std::cout << sig << std::endl;
    } while ( fils != pid && sig != SIGALRM && fils != -1);

    if ( sig == SIGCHLD && WIFEXITED(status) && fils != -1) {
        alarm(0);
        return pid; 
    } else if ( fils != -1 && sig == SIGALRM ){
        return 0;
    } else return -1;

}

int main() {

    int fils = fork();
    if ( fils == 0 ) {
        sleep(5);
        return 0;
    } else {
        std::cout << wait_till_pid_signal(0, 10) << std::endl;
    }

    int status;
    wait(&status);

    return 0;
}
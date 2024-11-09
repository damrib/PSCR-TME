#include <ctime>
#include <cstdlib>
#include <cstring>
#include <signal.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "rsleep.h"

int proc_hp = 5;
std::string name;

void FatherWins(int sig){
  exit(0);
}

void handler(int sig) {
  std::cout << name << ":" << proc_hp << std::endl;
	if ( --proc_hp == 0 ){
		if ( name == "Vader" ) { 
      std::cout << "NOOOOOOOOOOOOOOOOOOOOOOOOOOOO!!!!!!" << std::endl;
    } else if ( name == "Luke") {
      std::cout << "No...that's not true, that's impossible...NOOOOOO!!!!!!!!" << std::endl;
    }
    exit(1);
	}
  signal(SIGINT, &handler);
}

void pary(int sig) {
  std::cout << "coup paré " << std::endl;
}

void attaque(pid_t adversaire) {
  /**struct sigaction act;
  sigfillset(&act.sa_mask);
  act.sa_flags = 0;
  act.sa_handler = &handler;
  sigaction(SIGINT, &act, NULL);**/
  signal(SIGINT, &handler);
  kill(adversaire, SIGINT);
  randsleep();
}

void defense() {
  signal(SIGINT, SIG_IGN);
  /**if (name == "Vader") signal(SIGINT, SIG_IGN);
  else {
    signal(SIGINT, &pary);
    sigset_t setPos;
    sigemptyset(&setPos);
    sigaddset(&setPos, SIGINT);
    sigprocmask(SIG_BLOCK, &setPos, NULL);
    randsleep();
    sigsuspend(&setPos);
  }**/
  randsleep();
}

void combat(pid_t adversaire) {

  while ( true ) {
    defense();
    attaque(adversaire);
  }

}

void randsleep() {
  int r = rand();
  double ratio = (double)r / (double) RAND_MAX;
  struct timespec tosleep;
  tosleep.tv_sec =0;
  // 300 millions de ns = 0.3 secondes
  tosleep.tv_nsec = 300000000 + ratio*700000000;
  struct timespec remain;
  while ( nanosleep(&tosleep, &remain) != 0) {
    tosleep = remain;
  }
}

int main() {

  pid_t pid = fork();
  if (pid == 0) {
    srand(time(NULL));
    name = "Luke";
    proc_hp = 1;
    combat(getppid());
  } else {
    srand(time(NULL));
    name = "Vader";
    signal(SIGCHLD, &FatherWins);
    combat(pid);
  }

}

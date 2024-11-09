#include <iostream>
#include <unistd.h>
#include <sys/wait.h>


int main () {
	const int N = 3;
	std::cout << "main pid=" << getpid() << std::endl;

	int cpt = 0;
	for (int i=1, j=N; i<=N && j==N && ++cpt && fork()==0 ; i++ ) { // main ne cree qu'un seul fils
		cpt = 0;
		std::cout << " i:j " << i << ":" << j << std::endl; // i 
		for (int k=1; k<=i && j==N ; k++) { // Seul les fils du main peuvent creer des fils (3eme fils main -> 3fils, 2nd fils main -> 2fils, 1ee fils main -> 1fils)
			if ( fork() == 0) {
				j=0; // Les fils de fils ont J a 0
				cpt = 0;
				std::cout << " k:j " << k << ":" << j << std::endl;
			} else ++cpt;
		}
	}

	while (cpt-- > 0) {
		int status;
		wait(&status);
	}

	return 0;
}

// Arbre des processus créés par le programme
// main -> i:j 1:3 -> i:j 2:3  -> i:j 3:3 -> k:j 1:0
//                 -> k:j 1:0 |-> k:j 1:0 -> k:j 2:0
//         	                   -> k:j 2:0 |-> k:j 3:0
//                 
             

#include "Banque.h"
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <random>

int threadRandom(int min, int max) { // max inclusif
	static thread_local std::random_device rd;  // a seed source for the random number engine
	static thread_local std::mt19937 gem(rd());
	std::uniform_int_distribution<int> d(min, max);
	return d(gem);
}

void random_transac_then_sleep(pr::Banque& b) {
	size_t id_account1 = threadRandom(0, b.size()-1);
	size_t id_account2;
	do {
		id_account2 = threadRandom(0, b.size()-1);
	} while (id_account1 == id_account2);
	unsigned int v = std::rand() % 100;
	for (int i = 0 ; i < 1000; ++i){
		b.transfert(id_account1, id_account2, v);
	}
	auto sl_t = std::chrono::milliseconds((std::rand() % 20));
	std::this_thread::sleep_for(sl_t);
}

void thread_comptabilisation(pr::Banque& b, int expected, bool* res) {

	for (int i = 0; i < 1000; ++i) {
		*res = *res && b.comptabiliser(expected);
		return;
	}

}


const int NB_THREAD = 10;
int main () {
	std::srand(std::time(nullptr));

	std::vector<std::thread> threads;
	// TODO : creer des threads qui font ce qui est demandé
	threads.reserve(NB_THREAD);

	size_t nb_account = NB_THREAD / 2;
	pr::Banque b(nb_account, 100);

	for ( int i = 0; i < NB_THREAD; ++i) {
		threads.emplace_back(random_transac_then_sleep, std::ref(b));
	}

	bool invariant = true;
	std::thread comptable(thread_comptabilisation, std::ref(b), 100 * b.size(), &invariant);

	for (auto & t : threads) {
		t.join();
	}
	comptable.join();

	std::cout << "l'invariant est-il maintenu le long de l'execution? " << invariant << std::endl;
	std::cout << b.comptabiliser(100 * b.size()) << std::endl; 

	// TODO : tester solde = NB_THREAD * JP
	return 0;
}

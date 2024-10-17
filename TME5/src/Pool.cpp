#include "Pool.h"

namespace pr {

    void Pool::work(int i) {

        while (true) {
            Job* j = queue.pop();
            if ( j == nullptr ) return;
            j->run();
            delete j;
            ++nb_delete;
        }

    }

    Pool::Pool(size_t qsize) : queue(qsize) {
        nb_delete.store(0);
    }

    void Pool::start(int nbthread) {
        threads.reserve(nbthread);
        for (int i = 0; i < nbthread; ++i) {
            threads.emplace_back(&Pool::work, this, i);
        }
    }

    void Pool::submit( Job * job ) {
        queue.push(job);
    }

    void Pool::stop() {
        queue.setBlocking(false);
        for (auto& thr: threads) {
            thr.join();
        }
        threads.clear();
    }

    Pool::~Pool() {
        stop();
    }

}
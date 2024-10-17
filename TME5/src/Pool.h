#pragma once

#include "Queue.h"
#include "Job.h"
#include <vector>
#include <thread>
#include <atomic>

namespace pr {

	class Pool {
		Queue<Job> queue;
		std::vector<std::thread> threads;

		void work(int i);
	public:

		std::atomic<int> nb_delete;

		Pool(size_t qsize=10);
		void start (int nbthread);
		void submit (Job * job) ;
		void stop() ;
		~Pool() ;
	};

}

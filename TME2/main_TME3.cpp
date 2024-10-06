#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include <unordered_map>
#include <forward_list>
#include "TME3.h"

int main () {
	//sing namespace std;
	using namespace std::chrono;

	std::ifstream input = std::ifstream("WarAndPeace.txt");

	auto start = steady_clock::now();
	std::cout << "Parsing War and Peace" << std::endl;

	std::size_t nombre_lu = 0;
	// prochain mot lu
	std::string word;
	// une regex qui reconnait les caractères anormaux (négation des lettres)
	std::regex re( R"([^a-zA-Z])");

	std::unordered_map<std::string, int> map(1024);
	std::unordered_map<int, std::forward_list<std::string>> freq_word_map(1024);
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word =regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		// word est maintenant "tout propre"
		if (nombre_lu % 100000 == 0)
			// on affiche un mot "propre" sur 100
			std::cout << nombre_lu << ": "<< word << std::endl;
		nombre_lu++;

		auto it = map.find(word);
		if ( it == map.end() ){
			map.emplace(word, 1);
		} else {
			(*it).second = (*it).second + 1;
		}
	}
	input.close();
	std::cout << "Finished Parsing War and Peace" << std::endl;

	std::vector<std::pair<std::string, int>> map_data(map.size());
	for (const auto& node : map) {
		map_data.emplace_back(node);
		freq_word_map[node.second].push_front(node.first);
	}
	std::sort(map_data.begin(), map_data.end(), [](const std::pair<std::string, int>& e1, const std::pair<std::string, int>& e2) {return e1.second > e2.second; });

	for (int i = 100; i < 110; ++i) {
		std::cout << "affichage des mots qui ont " << i << " occurences: " << std::flush;
		if ( freq_word_map.find(i) != freq_word_map.end() )
			for (const auto& node: freq_word_map[i] ) {
				std::cout << node << " " << std::flush; 
			}
		std::cout << std::endl;
	}

	auto end = steady_clock::now();
    std::cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    std::cout << "Found a total of " << nombre_lu << " words." << std::endl;

	std::cout << "Found " << map.size() << " different words " << Iterator_utilities_TME::count(map.begin(), map.end()) << std::endl;
	std::cout << "nb occ of war " << (*map.find(std::string("war"))).second << std::endl;
	std::cout << "nb occ of peace " << (*map.find(std::string("peace"))).second << std::endl;

	std::cout << "most frequent words " << std::endl;
	for (int i = 0; i < 10; ++i) {
		std::cout << map_data[i].first <<": " << map_data[i].second << " times " /**<< Iterator_utilities_TME::count_if_equal(occurences.begin(), occurences.end(), map_data[i])**/ << std::endl; 
	}

    return 0;
}


#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include "myhashmap.h"
#include "TME3.h"


void add_different(std::vector<std::string>& Dict, std::string word){
	for (int i = 0; i < Dict.size(); ++i) {
		if (Dict[i] == word) return;
	}
	Dict.push_back(word);
}

void add_occurence(std::vector<std::pair<std::string, int>>& occ, std::string word){
	for (int i = 0; i < occ.size(); ++i) {
		if (occ[i].first == word) {
			occ[i].second += 1;
			return;
		}
	}
	occ.push_back(std::pair<std::string, int>(word, 1));
}

int find_nb_occ(const std::vector<std::pair<std::string, int>>& occ, std::string word){
	for (int i = 0; i < occ.size(); ++i) {
		if (occ[i].first == word){
			return occ[i].second;
		}
	}
	return 0;
}

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

	//std::vector<std::string> dictionnary = std::vector<std::string>(0);
	std::vector<std::pair<std::string, int>> occurences = std::vector<std::pair<std::string, int>>(0);
	myhashmap<std::string, int> map(10000);
	std::vector<std::pair<std::string, int>> map_data;
	while (input >> word) {
		// élimine la ponctuation et les caractères spéciaux
		word =regex_replace ( word, re, "");
		// passe en lowercase
		transform(word.begin(),word.end(),word.begin(),::tolower);

		// word est maintenant "tout propre"
		if (nombre_lu % 10000 == 0)
			// on affiche un mot "propre" sur 100
			std::cout << nombre_lu << ": "<< word << std::endl;
		nombre_lu++;
		//add_different(dictionnary, word);
		add_occurence(occurences, word);
		map.incr(word);
	}
	input.close();

	map_data = map.extract();
	std::sort(map_data.begin(), map_data.end(), [](const std::pair<std::string, int>& e1, const std::pair<std::string, int>& e2) {return e1.second > e2.second; });

	std::cout << "Finished Parsing War and Peace" << std::endl;

	auto end = steady_clock::now();
    std::cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";

    std::cout << "Found a total of " << nombre_lu << " words." << std::endl;
	//cout << "Found " << dictionnary.size() << " different words" << endl;
	/**for (pair p : occurences) {
		if (p.first == "war" || p.first == "peace" || p.first == "toto")
			cout << "nb occ of " << p.first << " " << p.second << endl;
	}**/
	std::cout << "Found " << map.size() << " different words " << Iterator_utilities_TME::count(map.begin(), map.end()) << std::endl;
	std::cout << "nb occ of war " << *map.get(std::string("war")) << std::endl;
	std::cout << "nb occ of peace " << *map.get(std::string("peace")) << std::endl;

	std::cout << "most frequent words " << std::endl;
	for (int i = 0; i < 10; ++i) {
		std::cout << map_data[i].first <<": " << map_data[i].second << " times " /**<< Iterator_utilities_TME::count_if_equal(occurences.begin(), occurences.end(), map_data[i])**/ << std::endl; 
	}

    return 0;
}



#include "List.h"
#include "String.h"
#include <string>
#include <iostream>
#include <cstring>
#include <cassert>



int main () {

	std::string abc = "abc";
	char * str = new char [3];
	str[0] = 'a';
	str[1] = 'b';
	str[2] = 'c';
	int i = 0; // Faute size_t ne peut pas etre neg

	if (! strcmp (str, abc.c_str())) {
		std::cout << "Equal !";
	}

	pr::List list;
	list.push_front(abc);
	list.push_front(abc);

	std::cout << "Liste : " << list << std::endl;
	std::cout << "Taille : " << list.size() << std::endl;

	// Affiche à l'envers
	for (i= list.size() - 1 ; i >= 0 ; i--) {
		std::cout << "elt " << i << ": " << list[i] << std::endl;
	}

	// liberer les char de la chaine
	/**for (char *cp = str ; *cp ; cp++) {
		delete cp;
	}**/

	delete[] str; 
    // Le destructeur est automatique detruit par le destructeur ~List

	std::cout << sbibl::length("abc") << " " << sbibl::newcopy("abc");
	String s("abc");
    std::cout << '\n' << s;
    const char * const test = s.get_string();
	std::cout << "\n" << test;
	s.modify_c('z');
	std::cout << "\n" << test << " " << s << std::endl;

	std::cout << sbibl::compare(String("abc"), String("abc")) << std::endl;
	std::cout << (String("abc") == String("abc")) << std::endl;
	std::cout << (String("abc") < String("abc")) << std::endl;
	std::cout << sbibl::compare(String("abc"), String("bbc")) << std::endl;
	std::cout << (String("abc") == String("bbc")) << std::endl;
	std::cout << (String("abc") < String("bbc")) << std::endl;
	std::cout << sbibl::compare(String("abd"), String("abc")) << std::endl;
	std::cout << (String("abd") == String("abc")) << std::endl;
	std::cout << (String("abd") < String("abc")) << std::endl;

	std::cout << String("abc") << std::endl;

	String test_affect = s;
	std::cout << test_affect << std::endl;
	
	return 0;
}

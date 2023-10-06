#include <iostream>
#include <tuple>
#include <stdexcept>

#include "find.hpp"
#include "trie.hpp"

#ifdef __EMSCRIPTEN__
	#include <emscripten/bind.h>
	using namespace emscripten;
	EMSCRIPTEN_BINDINGS(module) {
		function("init", &find::initJS);
		function("search", &find::search);
		value_object<find::Word>("Word")
			.field("word", &find::Word::word)
			.field("startX", &find::Word::startX)
			.field("startY", &find::Word::startY)
			.field("movement", &find::Word::movement);
		register_vector<int>("IntVector");
		register_vector<find::Word>("WordVector");
	}
#else
#include <fstream>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#endif

const std::string ARROWS[8] = { "↑", "↗", "→", "↘", "↓", "↙", "←", "↖" };

int main() {
	#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
	#endif

	find::init();

	int sizeX, sizeY = 0;
	std::cin >> sizeX;

	std::string temp;
	std::string grid = "";
	for (int i = 0; i < sizeX; i++) {
		std::cin >> temp;
		grid += temp;
		if (i == 0)
			sizeY = temp.length();
		else if (temp.length() != sizeY)
			throw std::invalid_argument("line length different");
	}

	std::vector<find::Word> result = find::search(grid, sizeX, sizeY);

	for (find::Word w : result) {
		std::cout << w.word << " [" << w.startY + 1 << ", " << 4 - w.startX << "]";
		for (int move : w.movement)
			std::cout << " " << ARROWS[move];
		std::cout << "\n";
	}

	return 0;
}
#endif
#ifndef FIND_H
#define FIND_H

#include <string>
#include <vector>

#include "trie.hpp"

namespace find {
	struct Word {
		std::string word;
		int startX, startY;
		std::vector<int> movement;
	};

	void init();
	void initJS(std::string wordList);
	std::vector<Word> search(std::string inputGrid, int sizeX, int sizeY);
}

#endif
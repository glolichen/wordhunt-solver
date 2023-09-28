#include <fstream>
#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>
#include <set>
#include <filesystem>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#endif

#include "trie.hpp"

#define WORDLIST_FILE "wordlist.txt"

struct Word {
	std::string word;
	int startX, startY;
	std::vector<int> movement;
};
bool operator<(const Word &l, const Word &r) {
	if (l.word.length() == r.word.length())
		return l.word < r.word;
	return l.word.length() < r.word.length();
}

const int DIR[8][2] = {
	{ -1, 0 },
	{ -1, 1 },
	{ 0, 1 },
	{ 1, 1 },
	{ 1, 0 },
	{ 1, -1 },
	{ 0, -1 },
	{ -1, -1 },
};
const std::string ARROWS[8] = { "↑", "↗", "→", "↘", "↓", "↙", "←", "↖" };

trie::TrieNode words;

char grid[4][4];
bool visited[4][4];
// used as a stack, but we need to iterate over it
std::vector<int> movements;
std::set<Word> results;

void dfs(int sx, int sy, int x, int y, std::string word, trie::TrieNode *curNode) {
	word += grid[x][y];

	if (word.length() >= 3 && curNode->isWord)
		results.insert({ word, sx, sy, movements });

	for (int i = 0; i < 8; i++) {
		int nx = x + DIR[i][0];
		int ny = y + DIR[i][1];

		if (nx < 0 || ny < 0 || nx >= 4 || ny >= 4)
			continue;
		if (!curNode->has_child(grid[nx][ny]))
			continue;
		if (visited[nx][ny])
			continue;

		movements.push_back(i);
		visited[nx][ny] = true;
		dfs(sx, sy, nx, ny, word, curNode->get_child(grid[nx][ny]));
		visited[nx][ny] = false;
		movements.pop_back();
	}
}

int main() {
	#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
	#endif

	std::ifstream wordListFile(WORDLIST_FILE);

	std::string word;
	while (wordListFile) {
		wordListFile >> word;
		trie::TrieNode *prevNode = words.add_or_get_child(word[0]);
		for (size_t i = 1; i < word.length(); i++)
			prevNode = prevNode->add_or_get_child(word[i]);
		prevNode->isWord = true;
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			std::cin >> grid[i][j];
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			visited[i][j] = true;
			dfs(i, j, i, j, "", words.get_child(grid[i][j]));
			visited[i][j] = false;
		}
	}

	for (Word w : results) {
		std::cout << w.word << " [" << w.startY + 1 << ", " << 4 - w.startX << "]";
		for (int move : w.movement)
			std::cout << " " << ARROWS[move];
		std::cout << "\n";
	}

	return 0;
}

#include <algorithm>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "trie.hpp"
#include "find.hpp"

#define WORDLIST_FILE "wordlist.txt"

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

int xMax, yMax;
char **grid;
bool **visited;
std::vector<int> movements;
std::vector<find::Word> result;

trie::TrieNode words;

void find::init() {
	std::ifstream wordlist(WORDLIST_FILE);
	std::string word;
	while (wordlist) {
		wordlist >> word;
		trie::TrieNode *prevNode = words.add_or_get_child(word[0]);
		for (size_t i = 1; i < word.length(); i++)
			prevNode = prevNode->add_or_get_child(word[i]);
		prevNode->isWord = true;
	}
	wordlist.close();
}
void find::initJS(std::string wordList) {
	std::stringstream ss(wordList);
	std::string word;
	while (ss) {
		ss >> word;
		trie::TrieNode *prevNode = words.add_or_get_child(word[0]);
		for (size_t i = 1; i < word.length(); i++)
			prevNode = prevNode->add_or_get_child(word[i]);
		prevNode->isWord = true;
	}
}

void dfs(int sx, int sy, int x, int y, std::string word, trie::TrieNode *curNode) {
	word += grid[x][y];

	if (word.length() >= 3 && curNode->isWord) {
		bool contains = false;
		for (find::Word w : result) {
			if (w.word == word) {
				contains = true;
				break;
			}
		}
		if (!contains)
			result.push_back({ word, sx, sy, movements });
	}

	for (int i = 0; i < 8; i++) {
		int nx = x + DIR[i][0];
		int ny = y + DIR[i][1];

		if (nx < 0 || ny < 0 || nx >= xMax || ny >= yMax)
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

std::vector<find::Word> find::search(std::string inputGrid, int sizeX, int sizeY) {
	result.clear();

	xMax = sizeX, yMax = sizeY;

	grid = new char *[sizeX];
	for (int i = 0; i < sizeX; i++) {
		grid[i] = new char[sizeY];
		for (int j = 0; j < sizeY; j++)
			grid[i][j] = inputGrid[i * sizeY + j];
	}
	
	visited = new bool *[sizeX];
	for (int i = 0; i < sizeX; i++)
		visited[i] = new bool[sizeY]();
	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			visited[i][j] = true;
			dfs(i, j, i, j, "", words.get_child(grid[i][j]));
			visited[i][j] = false;
		}
	}

	std::sort(result.begin(), result.end(), [](const find::Word &l, const find::Word &r) {
		if (l.word.length() == r.word.length())
			return l.word < r.word;
		return l.word.length() < r.word.length();
	});

	for (int i = 0; i < sizeX; i++) {
		delete[] grid[i];
		delete[] visited[i];
	}
	delete[] grid;
	delete[] visited;

	return result;
}
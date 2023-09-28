#ifndef TRIE_H
#define TRIE_H

#include <map>

namespace trie {
	class TrieNode {
	public:
		std::map<char, TrieNode> children;

		TrieNode() { }

		// add a child to this node, then return a pointer to the new node
		TrieNode *add_child(char c) {
			this->children[c] = TrieNode();
			return &this->children[c];
		}

		// get the child of this node with value c
		TrieNode *get_child(char c) {
			return &this->children[c];
		}

		// if c is a child, return a pointer to the child with value c
		// if c is not a child, create the child and return the pointer
		TrieNode *add_or_get_child(char c) {
			auto it = this->children.find(c);
			if (it == this->children.end())
				return this->add_child(c);
			else
				return this->get_child(c);
		}

		bool has_child(char c) {
			return this->children.count(c);
		}
	};
}

#endif
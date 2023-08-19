#include <iostream>
#include <unordered_map>

class TrieNode {
public:
    bool isEnd;
    std::unordered_map<char, TrieNode*> children;

    TrieNode() : isEnd(false) {}
};

class Trie {
private:
    TrieNode* root;

public:
    Trie() {
        root = new TrieNode();
    }

    void insert(const std::string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->isEnd = true;
    }

    bool search(const std::string& word) {
        TrieNode* node = root;
        for (char c : word) {
            if (node->children.find(c) == node->children.end()) {
                return false;
            }
            node = node->children[c];
        }
        return node->isEnd;
    }

    bool startsWith(const std::string& prefix) {
        TrieNode* node = root;
        for (char c : prefix) {
            if (node->children.find(c) == node->children.end()) {
                return false;
            }
            node = node->children[c];
        }
        return true;
    }
};

// int main() {
//     Trie trie;

//     trie.insert("apple");
//     trie.insert("app");
//     trie.insert("application");

//     std::cout << "Search 'apple': " << trie.search("apple") << std::endl;
//     std::cout << "Search 'app': " << trie.search("app") << std::endl;
//     std::cout << "Search 'appl': " << trie.search("appl") << std::endl;

//     std::cout << "Starts with 'app': " << trie.startsWith("app") << std::endl;
//     std::cout << "Starts with 'appl': " << trie.startsWith("appl") << std::endl;
//     std::cout << "Starts with 'bat': " << trie.startsWith("bat") << std::endl;

//     return 0;
// }

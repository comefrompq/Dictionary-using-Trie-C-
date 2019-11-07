#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#define MARKER '-'

using namespace std;

vector<string> Meaning;

struct  Trie {
	int id;
	char key;
	unordered_map<char, Trie*> map;
};

Trie* newTrie() {
	Trie* node = new Trie;
	node->id = -1;
	node->key = '~';
	return node;
}

long WriteMeaning(const char* meaning) {
	FILE* pFile = fopen("Meaning.txt", "a");
	if (pFile == NULL)
		perror("Error opening file");
	else {
		fputs(meaning, pFile);
	}
	long id = 1;
	return id;
	
}

void Insert(Trie*& root, const string& str, const char* meaning, int& meanId) {
	//TODO: add new word to Trie

	// If root is null
	if (root == NULL)
		root = newTrie();

	Trie* temp = root;
	for (auto c : str) {
		if (temp->map.find(c) == temp->map.end())
		{
			temp->map[c] = newTrie();
		}
		temp = temp->map[c];
		temp->key = c;
	}
	// Mark end of word and store the meaning
	temp->id = meanId;
	//WriteFile("Meaning.txt", meaning,150,meanId);
	Meaning.push_back(meaning);
	meanId++;
}

//TODO: store Trie to file

void SerializeKey(Trie* root, FILE* fTrie) {
	if (root == NULL) return;
	fprintf(fTrie, "%c ", root->key);
	for (auto& x : root->map) {
		SerializeKey(x.second, fTrie);
	}
	fprintf(fTrie, "%c ", MARKER);

}

void SerializeId(Trie* root, FILE* fId) {
	if (root == NULL) return;
	fprintf(fId, "%d ", root->id);
	for (auto& x : root->map) {
		SerializeKey(x.second, fId);
	}
	fprintf(fId, "%d ", -1);
}

//end:store
int Search(Trie* root, const string& word) {
	//TODO: find meanId of the word

	// If root is null i.e. the dictionary is empty
	if (root == NULL)
		return -1;
	Trie* temp = root;
	// Search a word in the Trie
	for (auto c : word) {
		temp = temp->map[c];
		if (temp == NULL)
			return -1;
	}
	// If it is the end of a valid word stored
	// before then return its meaning
	if (temp->id != -1)
		return temp->id;
	return -1;
}

void Print(Trie* root) {
	if (root == nullptr) {
		return;
	}
	cout << root->key << " ";
	for (auto& x : root->map) {
		Print(x.second);
	}
	cout << MARKER << " ";
}


int main() {
	FILE* fTrie = fopen("Trie.txt", "w");
	FILE* fId = fopen("TrieId.txt", "w");

	Trie* root = nullptr;
	char* word;
	int meanId = 0;
	Insert(root, "language", "language meaning ", meanId);
	Insert(root, "computer", "computer meaning ", meanId);
	Insert(root, "map", "map meaning ", meanId);
	Insert(root, "science", "science meaning ", meanId);
	Insert(root, "look", "look meaning ", meanId);
	Insert(root, "come", "come meaning ", meanId);
	Insert(root, "match", "match meaning ", meanId);
	Insert(root, "screen", "screen meaning ", meanId);

	SerializeKey(root, fTrie);
	//    fclose(fTrie);
	//    fclose(fId);
	//    fTrie = fopen("Trie.txt", "r");
	//    Printf(fTrie);
	//    cout << endl;
	//    Trie* deserial;
	//    deSerialize(deserial,fTrie,fId);
		//Print(root);
	//Print(root);
	cout << Meaning[Search(root, "science")];
	fclose(fTrie);
	fclose(fId);

	return 0;
}

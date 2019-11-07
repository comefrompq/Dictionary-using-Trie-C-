#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#define MARKER '-'

using namespace std;

struct  Trie {
	int id;
	char key;
	unordered_map<char, Trie*> children;
	Trie()
	{
		id = -1;
		key = '~';
	}
	Trie(char ch, int i)
	{
		key = ch;
		id = i;
	}
};

long WriteMeaning(const char* meaning) {
	//TODO: write mean to file

	FILE* pFile = fopen("Meaning.txt", "a");
	fseek(pFile, 0, SEEK_END);
	long id = ftell(pFile);
	if (pFile == NULL)
		perror("Error opening file");
	else {
		fprintf(pFile, "%s %c ", meaning, '~');
	}
	fclose(pFile);
	return id;	
}

int Insert(Trie*& root, const string& str, const string & meaning) {
	//TODO: add new word to Trie
	const char* mean = meaning.c_str();
	// If root is null
	if (root == NULL)
		root = new Trie();

	Trie* temp = root;
	for (auto c : str) {
		if (temp->children.find(c) == temp->children.end())
		{
			temp->children[c] = new Trie();
		}
		temp = temp->children[c];
		temp->key = c;
	}
	// Mark end of word and store the meaning
	if (temp->id == -1) {
		temp->id = WriteMeaning(mean);
		return 1;
	}
	else
		return 0;
}

//TODO: store Trie to file

void SerializeKey(Trie* root, FILE* fTrie) {
	if (root == NULL) return;
	fprintf(fTrie, "%c ", root->key);
	for (auto& x : root->children) {
		SerializeKey(x.second, fTrie);
	}
	fprintf(fTrie, "%c ", MARKER);

}

void SerializeId(Trie* root, FILE* fId) {
	if (root == NULL) return;
	fprintf(fId, "%d ", root->id);
	for (auto& x : root->children) {
		SerializeId(x.second, fId);
	}
	fprintf(fId, "%d ", -1);
}

void Serialize(Trie* root) {
	FILE* fTrie = fopen("Trie.txt", "w");
	FILE* fId = fopen("TrieId.txt", "w");
	SerializeKey(root, fTrie);
	SerializeId(root, fId);
	fclose(fTrie);
	fclose(fId);
}
//END:store

//TODO: load Trie
int deSerialize(Trie*& root, FILE* fword, FILE* fkey, char word, int k)
{
	// Else create node with this item and recur for children
	root = new Trie(word, k);
	while (1)
	{
		char val; int k;
		fscanf(fkey, "%d ", &k);
		if (!fscanf(fword, "%c ", &val) || val == MARKER)
			return 1;
		deSerialize(root->children[val], fword, fkey, val, k);
	}
	// Finally return 0 for successful finish
	return 0;
}

void loadTrie(Trie*& root) {
	FILE* fTrie = fopen("Trie.txt", "r");
	FILE* fId = fopen("TrieId.txt", "r");
	root = new Trie();
	char key; int id;
	fscanf(fTrie, "%c ", &key);
	fscanf(fId, "%d ", &id);

	deSerialize(root, fTrie, fId, key, id);
	fclose(fTrie);
	fclose(fId);

}
//END

int Search(Trie* root, const string& word) {
	//TODO: find meanId of the word

	// If root is null i.e. the dictionary is empty
	if (root == NULL)
		return -1;
	Trie* temp = root;
	// Search a word in the Trie
	for (auto c : word) {
		temp = temp->children[c];
		if (temp == NULL)
			return -1;
	}
	// If it is the end of a valid word stored
	// before then return its meaning
	if (temp->id != -1)
		return temp->id;
	return -1;
}

void GetMeaning(int id) {
	//TODO: get mean from file via index

	FILE* pFile = fopen("Meaning.txt", "r");
	char res[50];
	fseek(pFile, id, SEEK_SET);
	do
	{
		fscanf(pFile, "%s ", res);
		if (res[0] == '~') break;
		cout << res << " ";
	} while (true);
	cout << endl;
}

void header() {
	cout << "<======> WELCOME TO MY DICTIONARY <======>" << endl;
	cout << "<========================================>" << endl;
	cout << "Options: " << endl;
	cout << "_ 1. To search." << endl;
	cout << "_ 2. To add new word." << endl;
	cout << "_ 3. To quit." << endl;
}

int find(Trie *root) {
	string word;
	int option;
	cin.ignore();
	cout << "__Enter word to search: ";
	getline(cin, word);
	int id = Search(root, word);
	if (id == -1) {
		cout << "don't exist!! find another" << endl;
	}
	else {
		cout << "Mean: ";
		GetMeaning(id);
	}
	cout << "__Enter your options: ";
	cin >> option;
	return option;
}

int add(Trie* root) {
	string word;
	string meaning;
	int option;
	cin.ignore();
	cout << "__Enter word to add: ";
	getline(cin, word);
	cout << "__Enter mean: ";
	getline(cin, meaning);
	if (Insert(root, word, meaning)) {
		cout << "==> added! <==" << endl;
		Serialize(root);
	}
	else
		cout << "Add failed : existed!" << endl;
	cout << "__Enter your options: ";
	cin >> option;
	return option;
}

void Print(Trie* root) {
	if (root == nullptr) {
		return;
	}
	cout << root->key << " ";
	for (auto& x : root->children) {
		Print(x.second);
	}
	cout << MARKER << " ";
}


int main() {
	Trie* root = NULL;
	loadTrie(root);
	header();
	int option;
	cout << "__Enter your options: ";
	cin >> option;
	do
	{
		switch (option)
		{
		case 0: break;
		case 1: option = find(root); break;
		case 2: option = add(root); break;
		}
	} while (option != 0);
	return 0;
}

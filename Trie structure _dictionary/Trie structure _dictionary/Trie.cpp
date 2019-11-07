#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

using namespace std;
#define MARKER '-'
vector<string> Meaning;
struct Trie {
	int id;
	char key;
	unordered_map<char, Trie*> map;
};
Trie* newTrie(){
	Trie* node = new Trie;
	node->id = -1;
	node->key = '~';
	return node;
}
void WriteFile(const char* FileName,const char* str, int blockSize, int id){
    
}
void Insert(Trie*& root, const string& str, const char* meaning, int &meanId){

	// If root is null
	if (root == NULL)
		root = newTrie();

	Trie* temp = root;
    for(auto c: str){
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
int Search(Trie* root, const string& word){

	// If root is null i.e. the dictionary is empty
	if (root == NULL)
		return -1;
	Trie* temp = root;
	// Search a word in the Trie
    for(auto c:word) {
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
char* GetMeanFromFile(int id){
    FILE * pFile;
    pFile = fopen("Meaning.txt", "rb");
    int _size = 150;
    int blockSize = sizeof(char)*_size*id;
    char* mystring = new char[150];

    fseek(pFile,blockSize,SEEK_SET);
    if (pFile == nullptr)
        perror ("Error opening file");
    else {
        if ( fgets (mystring , _size , pFile) != nullptr )
            puts (mystring);
    }
    fclose(pFile);
    return mystring;
}
void Serialize(Trie* root, FILE* fTrie, FILE* fId){
    if(root == nullptr) return;
    fprintf(fTrie, "%c ", root->key);
    fprintf(fId,"%d ", root->id);
    for(auto&x: root->map){
        Serialize(x.second, fTrie,fId);
    }
    fprintf(fTrie, "%c ", MARKER);
    fprintf(fId, "%d ", -1);

}
void Print(Trie* root){
    if(root == nullptr) {
        return;
    }
    cout << root->key << " ";
    for(auto&x: root->map){
        Print(x.second);
    }
    cout << MARKER << " ";
 }
Trie *newNode(char key, int id)
{
    Trie *temp = new Trie;
    temp->key = key;
    temp->id = id;
    return temp;
}
void deSerialize(Trie* &root, FILE* fTrie, FILE* fId){
    char key;
    int id;
    root = newTrie();
    Trie* temp = root;
    while(fscanf(fTrie,"%c ",&key) != EOF && fscanf(fId,"%d ", &id) != EOF){
        if (temp->map.find(key) == temp->map.end())
        {
			temp->map[key] = newTrie();
        }
		temp = temp->map[key];
		temp->key = key;
    }
    root = newNode(key, id);

    //root->map[key] = newTrie();


}
void Printf(FILE* fTrie){
    char key;
    while(fscanf(fTrie,"%c ",&key)!= EOF){
        cout << key << " ";
    }

}
int main(){
    FILE* fTrie = fopen("Trie.txt","w");
    FILE* fId = fopen("TrieId.txt", "w");

    Trie* root = nullptr;
    char* word;
    int meanId = 0;
	Insert(root, "language", "language meaning ",meanId);
	Insert(root, "computer", "computer meaning ",meanId);
	Insert(root, "map", "map meaning ",meanId);
	Insert(root, "science", "science meaning ",meanId);
	Insert(root, "look", "look meaning ",meanId);
	Insert(root, "come", "come meaning ",meanId);
	Insert(root, "match", "match meaning ",meanId);
	Insert(root, "screen", "screen meaning ",meanId);

    Serialize(root,fTrie,fId);
//    fclose(fTrie);
//    fclose(fId);
//    fTrie = fopen("Trie.txt", "r");
//    Printf(fTrie);
//    cout << endl;
//    Trie* deserial;
//    deSerialize(deserial,fTrie,fId);
    //Print(root);
    //Print(root);
//    cout << endl;
    //cout << GetMeanFromFile(7);
    fclose(fTrie);
    fclose(fId);
    
    return 0;
}

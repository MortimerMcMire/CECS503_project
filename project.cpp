#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

class Trie
{
public:
  bool isEnd = false;
  bool isRoot = false;
  bool isEndofWord = false;
  Trie* parent = NULL;

  unordered_map<char, Trie*> map;

  void set_root(){
    isRoot = true;
  }
};

void insert_to_trie(Trie* root, const char* letter)
{
  //if this has a parent, it isn't a root node
  if (root->parent != NULL){
    return;
  }
  
  Trie* currentNode = root;

  while (*letter){
    if (currentNode->map[*letter] == NULL) {
      Trie *newNode = new Trie;
      newNode->isEnd = true;
      newNode->parent = currentNode;
      currentNode->isEnd = false;
      currentNode->map[*letter] = newNode;
    }
    currentNode = currentNode->map[*letter];
    letter++;
  }
  currentNode->isEndofWord = true;
  
}

//unused, but was my first test into Trie traversal
//bool traverse_trie(Trie* root, const char* word){
//  while (*word){
//    //cout << *word << endl;
//    if (root->map[*word]){
//      root = root->map[*word];
//      cout << root->isEnd << endl;
//      word++;
//    } else {
//      return false;
//    }
//  }
//  return true;
//}

int lev(string startWord, string targetWord) {
	const char *x = startWord.c_str();
	int xsize = startWord.size();
	const char *y = targetWord.c_str();
	int ysize = targetWord.size();
	int cost = 0;
	int d[xsize+1][ysize+1] = {};
	
	for (int i=0; i<=xsize; ++i) {
		d[i][0] = i;
	}
	
	for (int j=0; j<=ysize; ++j) {
		d[0][j] = j;
	}
	
	for (int j=1; j<=ysize; ++j){
		for (int i=1; i<=xsize; ++i) {
			if (x[i-1] == y[j-1]) {
				cost = 0;
			} else {
				cost = 1; }
			//cout << x[i-1] << " vs " << y[j-1] << endl;
			//cout << "deletion: " << (d[i-1][j] + 1) << " insertion: " << (d[i][j-1] + 1) << " sub: " << d[i-1][j-1] + cost << endl;
			int temp = min(d[i-1][j] + 1, d[i][j-1] + 1);
			d[i][j] = min(temp, d[i-1][j-1] + cost);
			//cout << "i: " << i << " j: " << j << " val:" << d[i][j] << endl;
		}
	}
	//cout << xsize << " " << ysize << " " << d[xsize][ysize] << endl;
	return d[xsize][ysize];
}

void recursive_list(Trie *root, vector<Trie> &leafarray, vector<char> &letterlist) {
	for (auto kv : root->map) {
		Trie *next = kv.second;
		if (next) {
		
		if (next->isEndofWord == 1) {
			letterlist.push_back(kv.first);
			leafarray.push_back(*next);
			//cout << kv.first << " is the end of a word " << endl;
		}
		if (next->isEnd == 0) {
			recursive_list(next,leafarray,letterlist);
		}
		}
	}
}

//for length comparison sorting the final list
bool compare(string &string1,string &string2) 
{ 
    return string1.size() < string2.size(); 
}

void suggested_display(vector<Trie> &leafedlist, vector<char> &letterlist, int numToDisplay) {
	vector<string> wordlist;
	if (numToDisplay == -1){
		numToDisplay = leafedlist.size();
	}
	
	for (int i=0;i < leafedlist.size();i++) {
  		
  		string fullword = "";
  		Trie leafnode = leafedlist[i];
  		Trie *pointy = &leafnode;
  		Trie *parentnode = leafnode.parent;	

		fullword += letterlist[i];
  		
  		while (parentnode->isRoot != true){
			Trie *current = parentnode;
  			parentnode = parentnode->parent;
  			for (auto test : parentnode->map) {
  				if (test.second == current) {
  					fullword += test.first;
		        }
		    }
		}
		reverse(fullword.begin(),fullword.end());
		wordlist.push_back(fullword);
		//cout << fullword << endl;
	  	}
	  	
    	// Function to perform sorting 
    	sort(wordlist.begin(), wordlist.end(),compare); 
    	
    	int len = wordlist.size(); //needed for min function in case < numToDisplay results exist
    	for (int j=0;j < min(len,numToDisplay);j++){
    		cout << wordlist[j] << endl;
		}
}

vector<string> dict_builder(vector<Trie> &leafedlist, vector<char> &letterlist) {
	vector<string> wordlist;
	
	for (int i=0;i < leafedlist.size();i++) {
  		
  		string fullword = "";
  		Trie leafnode = leafedlist[i];
  		Trie *pointy = &leafnode;
  		Trie *parentnode = leafnode.parent;	

		fullword += letterlist[i];
  		
  		while (parentnode->isRoot != true){
			Trie *current = parentnode;
  			parentnode = parentnode->parent;
  			for (auto test : parentnode->map) {
  				if (test.second == current) {
  					fullword += test.first;
		        }
		    }
		}
		reverse(fullword.begin(),fullword.end());
		wordlist.push_back(fullword);
	  	}
	  	return wordlist;
}

//unused, was used to generate other functions
//void simple_search(Trie* originalRoot, const char* word, vector<string> &matchList, string originalWord){
//	while (*word ) {
//    	if (originalRoot->map[*word]) {
//      		originalRoot = originalRoot->map[*word];
//			word++;
//    	} else {
//    		return;
//		}
//	}
//	matchList.push_back(originalWord);
//}

void search_dictionary(Trie* root, string fullword){
	
	Trie* originalRoot = root;
	const char * word = fullword.c_str();
	
	while (*word ) {
    	if (root->map[*word]) {
      		root = root->map[*word];
			word++;
    	} else {

			vector<Trie> leafedlist;
  			vector<char> letterlist;
  			vector<string> wordlist;
  			
  			recursive_list(originalRoot,leafedlist,letterlist);
			wordlist = dict_builder(leafedlist,letterlist);
			
			vector<pair<int,string>> levResults;
			for (auto word : wordlist){
				levResults.push_back(make_pair(lev(fullword, word),word)); //value , key in order to use default pair func
			}
			
			sort(levResults.begin(), levResults.end());
			int minLev = levResults[0].first; //lowest levenshtein score
			
			vector<string> printedList;
			
			cout << "Did you mean:\n";
			
			int printed = 0;
			for (auto word : levResults) {
				if (printed < 3) {
					if ((word.first == minLev) && (word.second[0] == fullword[0])) {
						cout << word.second << endl;
						printedList.push_back(word.second);
						printed += 1;
					} else {
						if (word.first > minLev) {
							break;
						}
					}
				} else {
					break;
				}
			}

			for (auto word : levResults) {
				if ((printed < 3) && (find(printedList.begin(), printedList.end(), word.second) == printedList.end())) {
					cout << word.second << endl;
					printed += 1;
				}
			}
			return;
		}

  	}
  	
  	if (root->isEnd == 0) {
  		//autocomplete
  		int numToDisplay = 3;
		cout << "Suggested results:" << endl;
  		
  		vector<Trie> leafedlist;
  		vector<char> letterlist;
  				
  		recursive_list(root,leafedlist,letterlist);
  		suggested_display(leafedlist,letterlist,numToDisplay);
	} else {
		cout << "Success!\n"; //Word is an end result
	}
  	return;
}

int main() {
  Trie* x = new Trie;
  x->set_root();

  //dictionary import
  fstream t;
  t.open("Dictionary.txt");
  string word;
  int next_char = t.peek();
  
  while (next_char != -1){
  	getline(t,word);
  	const char * c = word.c_str();
  	insert_to_trie(x,c);
  	next_char = t.peek();
  }
  
  string input;
  string filteredinput;
  
  cout << "Enter a word: ";
  cin >> input;
  
  for (auto y : input) {
  	filteredinput += tolower(y); //in case the user uses caps lock
  }
  
  search_dictionary(x,filteredinput);

}

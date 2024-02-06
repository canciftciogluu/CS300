#include "HashTable.h"
#include "BinaryTree.h"
#include "BinaryTree.cpp"
#include <iostream>
#include <string>
#include <sstream>
#include "strutils.h"
#include <chrono>
#include <vector>
#include <fstream>
using namespace std;


//This function cleans the string according to wanted input.
string stringEditor(string s){
	for(unsigned int i=0; i< s.length();i++){
		if ((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z')){

		}
		else{
			s[i] = ' ';
		}
	}	
	StripWhite(s);
	ToLower(s);
	return s;
}

//This function is classical linear search, nothing special.
bool searchInVector(vector<DocumentItem> vector, string search){
	for (unsigned int i=0; i< vector.size();i++){
		if(vector[i].documentName == search){
			return true;
		}
	}
	return false;
}

//This function is classical linear search, nothing special.
int documentFinder(vector<DocumentItem> vector, string search){
	for (unsigned int i=0; i< vector.size();i++){
		if(vector[i].documentName == search){
			return vector[i].count;
		}
	}
	return -1;
}


//This function takes fileName and wordItem which is to be updated and edit the related wordItem.
void updateTable(string fileName, WordItem * itemToBeUpdated){
	bool condition = false;
	for (unsigned int i=0; i<itemToBeUpdated->documentList.size();i++){
		if (itemToBeUpdated->documentList[i].documentName == fileName){
			condition = true;
			itemToBeUpdated->documentList[i].count++; /*if document is already part of the vector just increment the counter. */
		}
	}
	if (condition == false){ /*if condition is false then that's mean there is no document which we searching so create new and push back to the vector. */
		DocumentItem newDocument;
		newDocument.count = 1;
		newDocument.documentName = fileName;
		itemToBeUpdated->documentList.push_back(newDocument);
	}
}

//This function is same as the previous one but for the Binary Search Tree. */
WordItem updateTree(string fileName, WordItem itemToBeUpdated){
	bool condition = false;
	for (unsigned int i=0; i<itemToBeUpdated.documentList.size();i++){
		if (itemToBeUpdated.documentList[i].documentName == fileName){
			condition = true;
			itemToBeUpdated.documentList[i].count++;
		}
	}
	if (condition == false){
		DocumentItem newDocument;
		newDocument.count = 1;
		newDocument.documentName = fileName;
		itemToBeUpdated.documentList.push_back(newDocument);
	}
	return itemToBeUpdated;
}


int main(){
	vector<string> fileNames;
	WordItem notFound;
	BinarySearchTree<WordItem> wordTree(notFound); /*create the tree. */
	HashTable<WordItem> wordTable(notFound,53); /*create the table, 53 is chosen for the size. */
	int numberOfFiles;
	string fileName;
	int num=0;
	cout << "Enter number of input files: ";	cin >> numberOfFiles;	for(int i=0; i < numberOfFiles; i++){  /*take names of the files and add them to the vector. */		cout << "Enter " << i+1 << ". file name: ";		cin >> fileName;		fileNames.push_back(fileName);	}	for(int i=0; i < fileNames.size();i++){ /*then process all the files. */		string currFileName = fileNames[i],line;		ifstream input;		input.open(currFileName.c_str());		while(getline(input,line)){			string word = stringEditor(line),words; /*clean the string with the help of the editor function. */			istringstream linestream(word);			while(linestream >> words){				WordItem wordToBeSearched = WordItem(words);				if(wordTable.find(wordToBeSearched).word.length() > 0){ /*if find returns something then it's found, so update the related element. */					WordItem * wordToBeUpdated = wordTable.updateHash(wordToBeSearched); /*take the pointer of the word which is to be updated. */					updateTable(currFileName,wordToBeUpdated); /*update the table. */				}				else{ /*if length is 0 then there is no such element, since not found, create the document and add to the table. */					DocumentItem newDocument;
					newDocument.count = 1;
					newDocument.documentName = currFileName;					wordToBeSearched.documentList.push_back(newDocument);					wordTable.insert(wordToBeSearched);					}				if(wordTree.find(wordToBeSearched).word.length() > 0){ /*Same process as hashTable but these ones for the table. */					WordItem oldOne = wordTree.find(wordToBeSearched);					WordItem newOne = updateTree(currFileName,oldOne);					wordTree.updateTree(oldOne, newOne);				}				else{					if(wordToBeSearched.word.length() > 0){						wordTree.insert(wordToBeSearched);						}				}			}		}	}	cout << endl;	wordTable.showInfo(); /*Show the current information of the table. */	cout << "Enter queried words in one line: ";	string search, word;	cin.ignore();	getline(cin, search); /*Take queried words in one line. */	vector<string> queryWords;	search = stringEditor(search); /*clean the input. */
	istringstream linestream2(search);
	while(linestream2 >> word){
		queryWords.push_back(word); /*Add all the words to the vector with the help of the linestream. */
	}
	bool con = true, con2 = false; /*these conditions help the query process. */
	int threshold = 5000; /*run threshold times. */
	string printThat = ""; 
	auto start = std::chrono::high_resolution_clock::now(); /*measure the time. */
	for (int i = 0; i < threshold; i++)
	{
		printThat = "";
		for(int j=0; j<fileNames.size(); j++){ /*search in all files. */
			vector<WordItem> wordItems;  
			con = true;
			for(int k=0; k < queryWords.size(); k++){
				WordItem wordItem = wordTree.find(WordItem(queryWords[k]));
				if(searchInVector(wordItem.documentList, fileNames[j]) == false){ /*if some word doesn't have related document inside then convert condition to false. */
					con = false;
				}
				wordItems.push_back(wordItem); /*We will use vector in printing process. */
			}
			if(con == true){ /*If document has all the queried words. */
				con2 = true;
				printThat += "in Document " + fileNames[j];				for(int k=0; k< queryWords.size(); k++){					printThat +=  ", " + wordItems[k].word + " found " +  to_string(documentFinder(wordItems[k].documentList,fileNames[j])) + " times";				}				printThat += ".\n" ;			
			}
		}
	}
	if (con2 == false){ /*If condition 2 is false, then no document contains the given query. */
		cout << "No document contains the given query" << endl;
	}
	if(con2 == true){ /*Else, some document contains the given query then print it. */
		cout << printThat;
	}
	auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
	(std::chrono::high_resolution_clock::now() -
	start);
	con2 = false;
	con = true;
	start = std::chrono::high_resolution_clock::now(); /*Same process for the binary search tree. */
	for (int i = 0; i < threshold; i++)
	{
		printThat = "";
		for(int j=0; j<fileNames.size(); j++){
			vector<WordItem> wordItems;
			con = true;
			for(int k=0; k < queryWords.size(); k++){
				WordItem wordItem = wordTable.find(WordItem(queryWords[k]));
				if(searchInVector(wordItem.documentList, fileNames[j]) == false){
					con = false;
				}
				wordItems.push_back(wordItem);
			}
			if(con == true){
				con2 = true;
				printThat += "in Document " + fileNames[j];				for(int k=0; k< queryWords.size(); k++){					printThat +=  ", " + wordItems[k].word + " found " + to_string(documentFinder(wordItems[k].documentList,fileNames[j])) + " times";				}				printThat += ".\n" ;			
			}
		}
	}
	if (con2 == false){
		cout << "No document contains the given query" << endl;
	}
	if(con2 == true){
		cout << printThat;
	}
	auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
	(std::chrono::high_resolution_clock::now() - start);
	cout << "\nTime: " << BSTTime.count() / threshold  << "\n";
	cout << "\nTime: " << HTTime.count() / threshold   << "\n";
	cout << "Speed Up: " << double(BSTTime.count()) / double(HTTime.count());
}
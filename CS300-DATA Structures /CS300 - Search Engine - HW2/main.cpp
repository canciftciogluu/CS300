

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <sstream>
#include "Avltree.h"

using namespace std;

void ToLower(string & s);
// postcondition: all alphabetic characters in s changed to lowercase
//                (only uppercase letters changed)
void ToLower(string & s)
// postcondition: s all lower case
{
    int len = s.length();
    for(int k=0; k < len; k++)
    {
        s[k] = tolower(s[k]);
    }}


// Represents an item in a document with its name and count.
struct DocumentItem
{
    string documentName;
    int count;
    DocumentItem(string s = "", int c = 1) : documentName(s), count(c) {}
};


struct WordItem
{
    string word;
    vector<DocumentItem*> documentArray;
    // Constructor with a text parameter.
    WordItem(string s) : word(s) {}
};

// Represents a word and its count in a document.
struct WordCount
{
    string word;
    int count;
    WordCount(string s = "", int c = 0) : word(s), count(c) {}
};

// Represents an output item with a filename and a list of word counts.
struct OutputItem
{
    string filename;
    vector<WordCount*> wordList;
    OutputItem(string f = "") : filename(f) {}
};

// Searches for a document item in an array of documents by filename.
DocumentItem* inArray(const string& filename, const vector<DocumentItem*> documentArray)
{
    if (documentArray.size() != 0)
    {
       for (size_t i = 0; i < documentArray.size(); i++)
        {
            if (documentArray[i]->documentName == filename)
            {
                return documentArray[i];
            }
        }
    }
    return nullptr;}

int inOutput(const string filename, const vector<OutputItem>& out)
{
    int index = -1;
    int i = 0;

    while (i < out.size() && index == -1)
    {
        if (out[i].filename == filename)
        {
            index = i;
        }
        i++;}

    return index;
}

void PrintOut(vector<OutputItem>& out) {
    size_t i = 0;

    while (i < out.size()) {
        cout << "in Document " << out[i].filename << ", ";

        size_t j = 0;
        while (j < out[i].wordList.size()) {
            cout << out[i].wordList[j]->word << " found " << out[i].wordList[j]->count;
            cout << (j != out[i].wordList.size() - 1 ? " times, " : " times.\n");
            j++;}
        i++;}}


bool isAlpha(const string& s) {
    for (size_t i = 0; i < s.size(); ++i) {
        if ('a' > s[i] || s[i] > 'z') {
            return false;}}
    return true;}


void readFile(const string& filename, AvlTree<string, WordItem*>& wordTree) {
    ifstream input(filename);
    string s;
    while (input >> s) {
        ToLower(s);
        if (!isAlpha(s)) {
            continue; // If s is not alphanumeric, continue to the next iteration
        }

        WordItem* w = wordTree.find(s);
        if (w == nullptr) { // If the word is not in the wordTree
            w = new WordItem(s);
            w->documentArray.push_back(new DocumentItem(filename));
            wordTree.insert(s, w);
        } else { // If the word already exists
            DocumentItem* d = inArray(filename, w->documentArray);
            if (d == nullptr) { // If the document name is not in the array
                w->documentArray.push_back(new DocumentItem(filename));
            } else {
                d->count += 1; // If the document name already exists, increment the count
            }}}
    input.close();}


// Function declarations
void ToLower(string & s);
DocumentItem* inArray(const string& filename, const vector<DocumentItem*> documentArray);
int inOutput(const string filename, const vector<OutputItem>& out);
void PrintOut(vector<OutputItem>& out);
bool isAlpha(const string& s);
void readFile(const string& filename, AvlTree<string, WordItem*>& wordTree);



int main() {
    int n;
    AvlTree<string, WordItem*> wordTree;
    // Get the file names from the user
    cout << "Enter number of input files: ";
    cin >> n;
    vector<string> files(n);

    for (size_t i = 0; i < n; i++) {
        cout << "Enter file name " << i + 1 << ": ";
        cin >> files[i];
    }

    // Read files and build the word tree
    for (const auto& filename : files) {
        readFile(filename, wordTree);
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Process queries
    string line;
    while (true) {
        vector<string> queries;
        cout << "Enter queried words in one line: ";
        getline(cin, line);
        istringstream iss(line);
        string word;
        iss >> word;
        ToLower(word);

        if (word == "endofinput") {
            break; // Exit the loop when "endofinput" is entered
        } else if (word == "remove") {
            while (iss >> word) {
                ToLower(word);
                wordTree.remove(word);
                cout << word << " has been REMOVED" << endl; //feedback a word has been removed
            }}
            else {
            queries.push_back(word);
            while (iss >> word) {
                ToLower(word);
                queries.push_back(word);
            }

            // Process and print query results
            vector<OutputItem> out;
            for (const auto& filename : files) {
                OutputItem output(filename);
                bool allWordsInFile = true;
                for (const auto& query : queries) {
                    WordItem* w = wordTree.find(query);
                    if (w == nullptr || inArray(filename, w->documentArray) == nullptr) {
                        allWordsInFile = false;
                        break;
                    }
                    int count = inArray(filename, w->documentArray)->count;
                    output.wordList.push_back(new WordCount(query, count));
                }
                if (allWordsInFile) {
                    out.push_back(output);}}// Store results for each document

            if (out.empty()) {
                cout << "No document contains the given query" << endl;// if no documents contain the query
            } else {
                PrintOut(out);}}}// Print the query results

    wordTree.makeEmpty(); // Free memory used by the word tree

    return 0;
}
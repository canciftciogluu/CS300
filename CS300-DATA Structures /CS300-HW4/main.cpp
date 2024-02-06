#include <iostream>
#include "Hash_header.h"
#include "Bst_header.h"
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <chrono>
#include "Header_sort.h"




using namespace std;

struct DocumentItem {
    string documentName;
    int count;
};
struct WordItem  {
    string word;
    vector <DocumentItem> document_list ;
    // List of DocumentItemís. In order to keep the documents
    //you can again use the BST that you are going to implement.
};

WordItem  ITEM_NOT_FOUND;
string my_string="";


//BST IMPLEMENTATION
BstTree<string, WordItem > myTree(my_string,ITEM_NOT_FOUND);

//vector created for sorting later
vector <WordItem> myvector ;

bool if_number(string s) //this function checks if the string contains only numerical characters
{
    int i = 0;
    while (i < s.length())
    {
        char c = s.at(i);
        if ((c >= 48 && c <= 57)) // Check if the character is between '0' and '9'
        {
            return true;
        }
        i++;
    }
    return false;
}



bool if_exist(string &str,BstTree<string, WordItem > &myTree)
{
    if(myTree.find(str)=="")
        return false;
    else
        return true;
}


string toLower(string word) {
    int i = 0;
    while (i < word.length()) {
        if (word[i] >= 65 && word[i] <= 90) {
            word[i] += 32;
        }
        i++;
    }
    return word;
}


string only_alpha(string & word)
{
    int i = 0;
    while (i < word.length())
    {
        if (word.at(i) < 97 || word.at(i) > 122) // if not a letter replace to space
            word.at(i) = ' ';

        i++;
    }
    return word;
}

int counter1=0 ;

// Function to read a file and store its words in a binary search tree.
// file_name: Name of the file to read.
// myTree: Reference to the binary search tree where words will be stored.
void file_read_tree(string file_name, BstTree<string, WordItem > &myTree)
{
    ifstream inputfile; // Stream for reading from file
    string line; // String to hold each line of the file

    inputfile.open(file_name); // Open the file

    // Read the file line by line
    for (string line; getline(inputfile, line); )
    {
        istringstream iss(line); // Stream to process each line
        string word; // String to hold each word

        // Extract words from the line
        for (string word; iss >> word; )
        {
            word = toLower(word); // Convert word to lowercase
            word = only_alpha(word); // Remove non-alphabetic characters
            istringstream iss4(word); // Stream to process each processed word

            // Process each word
            for (string word; iss4 >> word; )
            {
                // Check if the word is not a number
                if (if_number(word) == false)
                {
                    // Check if the word already exists in the tree
                    if (if_exist(word, myTree) == true)
                    {
                        bool flag = false; // Flag to indicate if the word was found in the same document
                        WordItem *current_node = myTree.getNode(word); // Get the node for the word
                        
                        // Check if the word has already been used in the same document
                        for (int i = 0; i < current_node->document_list.size(); i++)
                        {
                            if (current_node->document_list[i].documentName == file_name)
                            {
                                flag = true; // Word found in the document, update count
                                current_node->document_list[i].count++;
                            }
                        }

                        // If the word has not occurred in this document before
                        if (flag == false)
                        {
                            DocumentItem newdoc; // Create a new document item
                            newdoc.count = 1; // Initialize count
                            newdoc.documentName = file_name; // Set document name
                            current_node->document_list.push_back(newdoc); // Add to the document list
                        }
                    }
                    else // If the word doesn't exist in the tree
                    {
                        WordItem item; // Create a new word item
                        item.word = toLower(word); // Set the word
                        DocumentItem details; // Create a new document item
                        details.documentName = file_name; // Set document name
                        details.count = 1; // Initialize count

                        item.document_list.push_back(details); // Add to the document list

                        myTree.insert(word, item); // Insert the new word item into the tree
                        counter1++; // Increment the unique word counter
                    }
                }
                else
                {
                    // If the word is a number, do nothing
                }
            }
        }
    }
    // You could uncomment the following line to output the count of unique words
    // cout << counter1 << " unique" << endl;
}

// Hash Table create
bool if_exist_table(string &str,HashTable<string, WordItem > &myTable)
{
    if(myTable.find(str).word== "")
        return false;
    else
        return true;
}





HashTable<string, WordItem> myTable(my_string, ITEM_NOT_FOUND);
int counter2=0 ;

void create_table(string file_name, HashTable<string, WordItem> &myTable)
{
    ifstream file;
    file.open(file_name.c_str()); // File is opened

    for (string line; getline(file, line); ) // Replacing while with for
    {
        istringstream iss(line);
        string word;

        for (string word; iss >> word; ) // Replacing while with for
        {
            word = toLower(word);
            string templine = only_alpha(word);

            istringstream iss2(templine);
            string tempword;

            for (string tempword; iss2 >> tempword; ) // Replacing while with for
            {
                if (if_number(tempword) == false) // if it's a string
                {
                    bool existence = if_exist_table(tempword, myTable);
                    if (existence) // word exists
                    {
                        bool flag = false;
                        WordItem *current_node = myTable.getNode_Tree(tempword);

                        for (int i = 0; i < current_node->document_list.size(); i++) // Existing for loop
                        {
                            if (current_node->document_list[i].documentName == file_name)
                            {
                                flag = true; // occurred before, just need to update count
                                current_node->document_list[i].count++;
                            }
                        }

                        if (!flag) // we are on a different txt
                        {
                            DocumentItem newdoc;
                            newdoc.count = 1;
                            newdoc.documentName = file_name;
                            current_node->document_list.push_back(newdoc);
                        }
                    }
                    else // word doesn't exist
                    {
                        WordItem item;
                        item.word = toLower(tempword);
                        DocumentItem docitem;
                        docitem.count = 1;
                        docitem.documentName = file_name;
                        item.document_list.push_back(docitem);
                        myTable.insert(tempword, item);
                        counter2++;
                    }
                }
                else
                {
                    // No action for numbers
                }
            }
        }
    }

    // cout << counter2 << " unique table" << endl;
}



void findtable(vector <WordItem*>outputs)
{
    int counter=0 ;
    for (int i=0 ; i< outputs.size() ;i++)
    {
        if (if_exist_table(outputs[i]->word,myTable)==true) //if (myTable.find(outputs[i]->word).word !="")
            counter++ ;

    }

}

void findtree(vector <WordItem*>outputs)
{
    int counter=0 ;
    for (int i=0 ; i< outputs.size() ;i++)
    {
        if (if_exist(outputs[i]->word,myTree)==true) //if (myTree.find(outputs[i]->word) !="")
            counter++ ;

    }

}

//VECTOR FUNCTS


bool vector_exist(string word, vector<WordItem> &vec)
{
    int i = 0;
    while (i < vec.size())
    {
        if (vec[i].word == word)
        {
            return true;
        }
        i++;
    }

    return false;
}


WordItem* return_index(string word, vector<WordItem> &vec)
{
    int i = 0;
    while (i < vec.size())
    {
        if (vec[i].word == word)
        {
            return &(vec[i]);
        }
        i++;
    }

    return NULL;
}


//---------------------------------------------
void file_read_vector (string file_name, vector<WordItem> &vec)
{
    ifstream file;
    file.open(file_name.c_str()); // File is opened

    string line;
    for (; getline(file, line); ) // Read each line, then process it
    {
        istringstream iss(line);
        string word;

        for (; iss >> word; )
        {
            word = toLower(word);
            // Separator
            string templine = only_alpha(word);

            istringstream iss2(templine);
            string tempword;

            for (; iss2 >> tempword; )
            {
                if (!if_number(tempword)) // If it's a string
                {
                    bool existence = vector_exist(tempword, vec);
                    if (existence) // The word exists
                    {
                        bool flag = false;
                        WordItem *current_node = return_index(tempword, vec);

                        for (int i = 0; i < current_node->document_list.size(); i++)
                        {
                            if (current_node->document_list[i].documentName == file_name)
                            {
                                flag = true; 
                                current_node->document_list[i].count++;
                            }
                        }

                        if (!flag) // We are on a different txt
                        {
                            DocumentItem newdoc;
                            newdoc.count = 1;
                            newdoc.documentName = file_name;
                            current_node->document_list.push_back(newdoc);
                        }
                    }
                    else 
                    {
                        WordItem item;
                        item.word = toLower(tempword);
                        DocumentItem docitem;
                        docitem.count = 1;
                        docitem.documentName = file_name;
                        item.document_list.push_back(docitem);
                        vec.push_back(item);
                        counter2++;
                    }
                }
               
                else
                {
                    
                }
            }
        }
    }
    file.close();
}







double speed_func(double a, double b)
{
    

    double speed= a/b ;
    return  speed ;
    

}





void time_func(vector <WordItem*> outputs, vector <WordItem*> outputs_table)
{
    int k = 10000;
    auto start = std::chrono::high_resolution_clock::now();

    int i = 0;
    while (i < k)
    {
        findtree(outputs);

        // QueryDocuments(with BST);
        ++i;
    }

    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
        (std::chrono::high_resolution_clock::now() - start);
    cout << "Binary Search Tree Time:: " << BSTTime.count() / k << "\n";

    start = std::chrono::high_resolution_clock::now();
    i = 0; // Resetting i for the next loop
    while (i < k)
    {
        // QueryDocuments (with hashtable);

        findtable(outputs_table);

        ++i;
    }

    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
        (std::chrono::high_resolution_clock::now() - start);
    cout << "Hash Table Time: " << HTTime.count() / k << "\n";

    double bst_time_double = BSTTime.count() / k;

    /*double speed= bst_time_double/(HTTime.count()/k);
    cout<< "Speed Up: "<< speed ;*/
}


int binarySearch(vector<WordItem> &myvec, int l, int r, string &some_word)
{
    while (l <= r) {
        int m = l + (r - l) / 2;
  
        // Check if x is present at mid
        if (myvec[m].word == some_word)
            return m;
  
        // If x greater, ignore left half
        if (myvec[m].word < some_word)
            l = m + 1;
  
        // If x is smaller, ignore right half
        else
            r = m - 1;
    }
  
    // if we reach here, then element was
    // not present
    return -1;
}

void binary_2(vector<WordItem> &smt, vector<string> &query_lines, int n)
{
    int count = 0;
    int i = 0;
    while (i < query_lines.size())
    {
        int a = binarySearch(smt, 0, n - 1, query_lines[i]);
        if (a != -1)
            count++;

        i++;
    }
}

vector<double> time_2 (vector <WordItem*> outputs,vector <WordItem*>outputs_table) //int n,vector<WordItem> &heapvec,vector<string> &query_words)
{
    vector <double> numbers;
    int k = 10000;
    clock_t begin_time = clock();
    for (int i = 0; i < k; i++)
        findtree(outputs) ;
        // QueryDocuments with Binary Search Tree
    double endBST = float(clock() - begin_time);

    cout << "\nBinary Search Tree Time: " << endBST / k << "\n";
    numbers.push_back(endBST/k) ;


    begin_time = clock();
    for (int i = 0; i < k; i++)
        findtable(outputs_table) ;
        // QueryDocuments with Hash Table
    double endHT = float(clock() - begin_time);

    cout << "\nHash Table Time: " << endHT / k << "\n";
   numbers.push_back(endHT/k) ;


   return numbers;

}

int main() {

    int nu_files;
    string file_name,query_line;
    vector<string> file_name_list;


    cout<< "Enter number of input files:";
    cin>>nu_files;
    for (int i=1; i<=nu_files;i++){
        cout<<"Enter "<<i <<". file name:";
        cin>>file_name;
        file_name_list.push_back(file_name);
        file_read_tree(file_name,myTree);
        //create_table(file_name, myTable);

    }


    for (int j=0; j<nu_files; j++)
    {
        create_table(file_name_list[j],myTable);

    }

    myTable.resulting();



    for (int j=0; j<nu_files; j++)
    {
        file_read_vector(file_name_list[j],myvector);

    }

    vector <WordItem>insertionvec (myvector);
    vector <WordItem>quickvec1 (myvector);
    vector <WordItem>quickvec2 (myvector);
    vector <WordItem>quickvec3 (myvector);
    vector <WordItem>mergevec (myvector);
    vector <WordItem>heapvec (myvector);

    cout<< "Enter queried words in one line: " ;
    //cin>>query_line;


    cin.ignore();
    getline(cin,query_line);
    query_line= toLower(query_line);
    vector <WordItem *> outputs; //tree
    vector <WordItem *> outputs_table;
    query_line=only_alpha(query_line);
    istringstream iss2(query_line) ;
    string myword;
    
    
//    ---------------------------------------------------------------------------------------------------------------------bu
vector<string> query_words;
for (; iss2 >> myword; ) // Read words from the input stream
{
    // Check if the word exists in either the tree or the table
    if (!if_exist(myword, myTree)) // && !if_exist_table(mystring, myTable))
    {
        cout << "No document contains the given query" << endl;
        cout << "No document contains the given query" << endl;
        cout << "No document contains the given query" << endl;
        time_2(outputs, outputs_table); // Call the time function

        return 10; // Return with error code 10
    }
    else
    {
        // If word exists, get the node from the tree and the table
        WordItem *smt = myTree.getNode(myword);
        outputs.push_back(smt);
        WordItem *smt2 = myTable.getNode_Tree(myword);
        outputs_table.push_back(smt2);
    }

    query_words.push_back(myword); // Add the word to query_words
}

for (int b = 1; b <= 3; b++) // Iterate three times
{
    bool flag = true;
    if (!outputs.empty()) // Check if outputs is not empty
    {
        for (size_t i = 0; i < file_name_list.size(); i++) // Iterate through file names
        {
            for (size_t k = 0; k < outputs.size(); k++) // Iterate through outputs
            {
                for (size_t j = 0; j < outputs[k]->document_list.size(); j++) // Iterate through document list of each output
                {
                    if (outputs[k]->document_list[j].documentName == file_name_list[i])
                    {
                        if (flag)
                        {
                            cout << "In Document " << outputs[k]->document_list[j].documentName << ",";
                            flag = false;
                        }

                        cout << outputs[k]->word << " found ";
                        cout << outputs[k]->document_list[j].count << " times";

                        if (k != outputs.size() - 1)
                        {
                            cout << ",";
                        }
                        else
                        {
                            cout << "." << endl;
                            flag = true; // Reset flag for the next document
                        }
                    }
                }
            }
        }
    }
}

    vector<double>timesof= time_2(outputs,outputs_table) ;
    

    int t=10000;
    
    int k = 100;
    //int k = 10;
    clock_t begin_time = clock();

    begin_time = clock();
    for (int i = 0; i < k; i++)
    {
        heapvec=myvector;
        heapsort(heapvec) ;
        //heapvec= heapsort(myvector) ;
    }
        //heapvec= heapsort(myvector) ;
    double endHT = float(clock() - begin_time);


    
    int n=heapvec.size() ;


    begin_time = clock();
    for (int i = 0; i < t; i++)
         binary_2(heapvec,query_words,n);
        // QueryDocuments with Binary Search
    double endBS1 = float(clock() - begin_time);
    cout << "\nBinary Search Time: " << endBS1 / t << "\n";



    cout<< endl ;


   
    begin_time = clock();
    for (int i = 0; i < k; i++){
        quickvec1=myvector ;
        quicksort(quickvec1, 0, myvector.size( ) - 1); //,"median");
        
    }
    
    double endBS = float(clock() - begin_time);
    cout << "Quick Sort(median) Time: " << endBS / k << "\n";
    

    begin_time = clock();
    for (int i = 0; i < k; i++){
        quickvec2=myvector ;
        quicksortR(quickvec2, 0, myvector.size( ) - 1 ) ;
        //quickSortrandom(quickvec2, 0, myvector.size( ) - 1);
        
    
    }
    double endBS2 = float(clock() - begin_time);
    cout << "Quick Sort(random) Time: " << endBS2 / k << "\n";

    
    
    begin_time = clock();
    for (int i = 0; i < k; i++){
         
        quickvec3=myvector ;
        quicksortF(quickvec3, 0, myvector.size( ) - 1) ;//,"first");
        
    }
        
      
    double endBS3 = float(clock() - begin_time);
    cout << "Quick Sort(first) Time: " << endBS3 / k << "\n";


    begin_time = clock();
    for (int i = 0; i < k; i++)
    {
        mergevec=myvector;
        mergeSort(mergevec) ;
    }

    double endBS4 = float(clock() - begin_time);
    cout << "Merge Sort Time: " << endBS4 / k << "\n";
    cout << "Heap Sort Time: " << endHT / k << "\n";

    begin_time = clock();
    for (int i = 0; i < k; i++){
        insertionvec=myvector;
        insertionSort(insertionvec) ;
        
    }
            //insertionvec=insertionSort(myvector) ;
    double endBST = float(clock() - begin_time);

    cout << "Insertion Sort Time: " << endBST / k << "\n";


cout<< endl ;
cout<< "Speed Up BST/HST:"<<speed_func(timesof[0],timesof[1])<<endl;
cout<< "Speed Up Merge Sort/Quick Sort(Median):"<< speed_func(endBS4 / k,endBS / k)<<endl;
cout<< "Speed Up Heap Sort/Quick Sort(Median):"<< speed_func(endHT/ k,endBS / k)<<endl;
cout<< "Speed Up Insertion Sort/Quick Sort(Median):"<< speed_func(endBST / k,endBS / k)<<endl;

cout<< endl ;
cout << "Speed Up Binary Search / Binary Search Tree Time: " << (endBS1/t)/timesof[0]<<endl;
cout << "Speed Up Binary Search / Hash Table Time: "<< (endBS1/t)/timesof[1] <<endl;

    return 0;
}

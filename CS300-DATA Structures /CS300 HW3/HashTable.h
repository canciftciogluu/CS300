#ifndef HASH_H
#define HASH_h

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
using namespace std;



struct DocumentItem {
	string documentName;
	int count;
};
struct WordItem {
	string word;
	vector<DocumentItem> documentList; /*WordItem includes the list of documents.*/
	WordItem(string word):word(word){}
	WordItem(){}
	bool operator < (const WordItem & rhs) const{
		return word < rhs.word;
	}
	bool operator > (const WordItem& rhs) const{
		return word > rhs.word;
	}
	bool operator == (const WordItem& rhs) const{
		return word == rhs.word;
	}
	bool operator != (const WordItem& rhs) const{
		return word != rhs.word;
	}
	friend ostream & operator <<( ostream & of, const WordItem &s ){
		of << s.word;
		return of;
	}
};

 
template <class HashedObj>
	class HashTable
	{
	public:
		explicit HashTable( const HashedObj & notFound, int size = 101 );
		HashTable();
		HashTable( const HashTable & rhs )
                   : ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
                     hashArray( rhs.hashArray ), currentSize( rhs.currentSize ) { }
		const HashedObj & find(const HashedObj & x );
		void insert(const HashedObj & x );
		void makeEmpty();
		HashedObj * updateHash(HashedObj element);
		void remove( const HashedObj & x );
		void showInfo();
		const HashTable & operator=( const HashTable & rhs );
		enum EntryType { ACTIVE, EMPTY, DELETED };
	private:
		struct HashEntry
		{
			HashedObj element;
			EntryType info;
			HashEntry( const HashedObj & e = HashedObj( ), 
							EntryType i = EMPTY )
						  : element( e ), info( i ) { }
			};
			vector<HashEntry> hashArray;
			int currentSize;
			const HashedObj ITEM_NOT_FOUND;
			bool isActive( int currentPos ) const;
			int findPos( const HashedObj & x );
			void rehash();
			bool isPrime( int n ){ /*This function will be used to arrange size of the table.*/
				if ( n == 2 || n == 3 )
					return true;
				if ( n == 1 || n % 2 == 0 )
					return false;
				for ( int i = 3; i * i <= n; i += 2 )
					if ( n % i == 0 )
						return false;
				return true;
			}

		    int nextPrime( int n ){ /*This function will be used to arrange size of the table.*/
			if ( n % 2 == 0 )
				n++;
              for ( ; ! isPrime( n ); n += 2 )
                   ;
              return n;
			}

 };

/*This function will be used to destruct table. */
template <class HashedObj>
void HashTable<HashedObj>::makeEmpty()
{
	for(unsigned int i = 0; i < hashArray.size(); i++ )
		hashArray[i].info = EMPTY;
	currentSize = 0;
}

/*This function hashes the wordItem. */
int hashFunction(const WordItem & key, int tableSize ){
	int hashVal = 0;
	string keyEle = key.word;
	for( unsigned int i = 0; i < keyEle.length(); i++ ){
		hashVal = hashVal * 37 + keyEle[ i ];
	}
	hashVal = hashVal % tableSize;
	if( hashVal < 0 )
		hashVal = hashVal + tableSize;
	return hashVal;
}


/**
* Construct the hash table.*/
template <class HashedObj>
HashTable<HashedObj>::HashTable( const HashedObj & notFound, int size ): ITEM_NOT_FOUND( notFound ), hashArray( nextPrime( size ) ){
	makeEmpty();
}
/**
* Construct the hash table.*/
template <class HashedObj>
HashTable<HashedObj>::HashTable(){}

template <class HashedObj>
HashedObj* HashTable<HashedObj>::updateHash(HashedObj element){
	return &hashArray[findPos(element)].element;
}

/**
* Method that performs quadratic probing resolution.
* Return the position where the search for x terminates.*/
template <class HashedObj>
int HashTable<HashedObj>::findPos(const HashedObj & x ){
	int collisionNum = 0;
	int currentPos = hashFunction( x, hashArray.size( ) );
	while ( hashArray[ currentPos ].info != EMPTY && hashArray[ currentPos ].element != x ){
		currentPos += pow(++collisionNum, 2) ;  //add the difference
		while ( currentPos >= hashArray.size( ) )              // perform the mod
			currentPos = currentPos - hashArray.size();                // if necessary
		}
		return currentPos;
}
/**
* Return true if currentPos exists and is active.
*/
template <class HashedObj>
bool HashTable<HashedObj>::isActive( int currentPos ) const{
	return hashArray[ currentPos ].info == ACTIVE;
}
/**
* Remove item x from the hash table.
*  x has to be in the table
*/
template <class HashedObj>
void HashTable<HashedObj>::remove( const HashedObj & x ){
	int currentPos = findPos( x );
	if ( isActive( currentPos ) )
		hashArray[ currentPos ].info = DELETED;
}
/**
* Find item x in the hash table.
* Return the matching item, or ITEM_NOT_FOUND, if not found.
*/
template <class HashedObj>
const HashedObj & HashTable<HashedObj>::find( const HashedObj & x ){
	int currentPos = findPos( x );
	if (isActive( currentPos )) 
		return hashArray[ currentPos ].element; 
	return   ITEM_NOT_FOUND;
}
/**
* Expand the hash table.
*/
template <class HashedObj>
void HashTable<HashedObj>::rehash(){
	vector<HashEntry> oldArray = hashArray;
	// Create new double-sized, empty table
	hashArray.resize( nextPrime( 2 * oldArray.size( ) ) );
	for (unsigned int j = 0; j < hashArray.size( ); j++ )
		hashArray[ j ].info = EMPTY;
	// Copy table over
	currentSize = 0;
	for ( unsigned int i = 0; i < oldArray.size( ); i++ )
		if ( oldArray[ i ].info == ACTIVE )
			insert( oldArray[ i ].element );
	cout << "rehashed..." << endl;
	cout << "previous table size:" << oldArray.size() << ", new table size: " << hashArray.size() << ", current unique word count: " << currentSize << ", current load factor: " << double(currentSize)/double(hashArray.size()) << endl;
}

/**
* Insert item x into the hash table. If the item is
* already present, then do nothing.
*/
template <class HashedObj>
void HashTable<HashedObj>::insert( const HashedObj & x ){
	// Insert x as active
	int currentPos = findPos( x );
	if ( isActive( currentPos ) )
		return;
	hashArray[ currentPos ] = HashEntry( x, ACTIVE );
	// enlarge the hash table if necessary 
	float lf = 1/0.9;
	if ( ++currentSize >= hashArray.size() / lf){
		rehash();
	}
}

/*Show the current information of the table. */
template <class HashedObj>
void HashTable<HashedObj>::showInfo(){
	cout <<"After preprocessing, the unique word count is " << currentSize << ". current load ratio is " << double(currentSize)/hashArray.size() << endl;
}


#endif
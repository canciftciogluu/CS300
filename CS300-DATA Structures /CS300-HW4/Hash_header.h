

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;


#ifndef HT_h
#define HT_h

template <class Key, class HashedObj>

class HashTable

{
public:

    explicit HashTable( const Key &t,const HashedObj & notFound, int size = 53 );

    HashTable( const HashTable & rhs ): ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
        array( rhs.array ), currentSize( rhs.currentSize ) { }
    const HashedObj & find( const Key  & x ) const;
    //const HashEntry  find( const Key  & x ) const;
    void makeEmpty( );
    void insert( const Key &k,const HashedObj & x );
    void remove( const HashedObj & x );
    const HashTable & operator=( const HashTable & rhs );
    enum EntryType { ACTIVE, EMPTY, DELETED };
    int hash ( const string & key, int tableSize ) const;

    HashedObj* findBen( Key &x);
    HashedObj * getNode_Tree(Key & element);

    void resulting ();


private:


    //template <class Key,class HashedObj>
    struct HashEntry
    {
        Key x;
        HashedObj element; //wordıtem
        EntryType info;
        HashEntry( const HashedObj & e,const Key &k,EntryType i = EMPTY ): element( e ),x(k), info( i ){ }
        HashEntry (){}
    };

    vector<HashEntry> array;
    int currentSize;
    const HashedObj ITEM_NOT_FOUND;
    bool isActive( int currentPos ) const;
    int findPos( const Key & x )  const;
    void rehash( );

    const  Key not_found_key ;
    HashedObj* notfound_ptr;
};

/*This function will be used to arrange size of the table.*/
bool isPrime( int n )
{
    if ( n == 2 || n == 3 )
        return true;
    if ( n == 1 || n % 2 == 0 )
        return false;
    for ( int i = 3; i * i <= n; i += 2 )
        if ( n % i == 0 )
            return false;
    return true;

}
/*This function will be used to arrange size of the table.*/

int nextPrime( int n )
{
    if ( n % 2 == 0 )
        n++;
    for ( ; ! isPrime( n ); n += 2 )
        ;
    return n;
}

/*  Construct the hash table.   */
template <class Key,class HashedObj>
HashTable<Key,HashedObj>::HashTable( const Key &k, const HashedObj & notFound,int size ): ITEM_NOT_FOUND( notFound ), array( nextPrime( size ) )
{

    makeEmpty( );

}
/*This function will be used to destruct table. */
template <class Key, class HashedObj>void HashTable<Key, HashedObj>::makeEmpty( )
{
    for( int i = 0; i < array.size( ); i++ )
        array[ i ].info=EMPTY;
    currentSize=0 ;
    // destroy the lists but not the vector!

}

/*  Show the current information of the table.  */
template <class Key, class HashedObj>void HashTable<Key, HashedObj>::resulting( )
{
    double d_current=currentSize ;
    cout<< endl ;
    cout<< "After preprocessing, the unique word count is " <<  currentSize<<". Current load ratio is "<< d_current/array.size() <<endl ;
    // destroy the lists but not the vector!

}

template<class Key,class HashedObj>
int HashTable<Key,HashedObj>::hash (const string & key, int tableSize) const

{
    int sum = 0;

    for (int i = 0; i < key.length(); i++) // add all bytes in a loop

        sum = sum + key[ i ];

    return (sum % tableSize);

}

/*  Method that performs quadratic probing resolution.
    Return the position where the search for x terminates.  */
template <class Key,class HashedObj>int HashTable<Key,HashedObj>::findPos( const Key & t )  const
{
    int collisionNum = 0;
    int currentPos = hash( t, array.size( ) ); //dogru mu ? hash before
    //YUKARIYA BAK HASH FONK YAZ

    while ( array[ currentPos ].info != EMPTY && array[ currentPos ].x != t )
    {
        currentPos += 2 * ++collisionNum - 1; // add the difference
        if ( currentPos >= array.size( ) ) // perform the mod
            currentPos -= array.size( ); // if necessary
    }

    return currentPos;

}


/*  Return true if currentPos exists and is active. */
template <class Key, class HashedObj>bool HashTable<Key,HashedObj>::isActive( int currentPos ) const
{
    return array[ currentPos ].info == ACTIVE;
}


/*  Remove item x from the hash table.
    x has to be in the table    */
template <class Key,class HashedObj>void HashTable<Key,HashedObj>::remove( const HashedObj & x )
{
    int currentPos = findPos( x );
    if ( isActive( currentPos ) )
        array[ currentPos ].info = DELETED;

}

//find hash entry dondurmeli

template <class Key,class HashedObj>const HashedObj& HashTable<Key,HashedObj>::find( const Key & x )const
{
    int currentPos = findPos( x );
    if (isActive( currentPos ))
        return array[ currentPos].element;
    return ITEM_NOT_FOUND; //sen yazdın cunku ıtemnotfıund value turundendi

}


template <class Key,class HashedObj> HashedObj*  HashTable<Key,HashedObj>::findBen(  Key & x )
{
    int currentPos = findPos( x );
    HashedObj *temp;
    if (isActive( currentPos ))
    {
        temp = &array[ currentPos].element ;
        return temp;
    }

    else
        return notfound_ptr;

}

/*  Insert item x into the hash table. If the item is already present, then do nothing. */
template <class Key,class HashedObj>void HashTable<Key,HashedObj>::insert( const Key &k,const HashedObj & x )
{

    // Insert x as active
    int currentPos = findPos( k );
    if ( isActive( currentPos ) )
        return;
    //count arttır
    array[ currentPos ] = HashEntry( x,k,ACTIVE );

    // enlarge the hash table if necessary
    if ( ++currentSize >= array.size( )*0.5 ){
        rehash( );
        //cout<< "rehashed..." <<endl;

    }
}

/*  Expand the hash table.    */
template <class Key,class HashedObj>void HashTable<Key,HashedObj>::rehash( )
{
    vector<HashEntry> oldArray = array;

    // Create new double-sized, empty table
    array.resize( nextPrime( 2 * oldArray.size( ) ) );
    for ( int j = 0; j < array.size( ); j++ )
        array[ j ].info = EMPTY;

    // Copy table over
    currentSize = 0;
    for ( int i = 0; i < oldArray.size( ); i++ )
    {
        if ( oldArray[ i ].info == ACTIVE )
            insert(oldArray[ i ].x,oldArray[ i ].element );
    }

}

//getnode
template<class Key,class HashedObj>HashedObj* HashTable<Key,HashedObj>::getNode_Tree(Key & element)
{
    int i ;
    i = findPos(element) ;
    return (&(array[i].element)) ;
    //return &find(element).element;
}

#endif

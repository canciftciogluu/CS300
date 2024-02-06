
#ifndef BST_h
#define BST_h


#include <string>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;


template <class Key,class Value>
class BstTree;

template <class Key,class Value>
class BstNode
{
    Key     word;
    Value  val ;
    BstNode           *left;
    BstNode           *right;
    int                height;

    BstNode( const Key & key, const Value & word_item,
        BstNode *lt, BstNode *rt, int h = 0 )
        : word( key ),val(word_item) ,left( lt ), right( rt ), height( h ) { }

    friend class BstTree<Key,Value>;
};


//template <class Comparable>
template <class Key, class Value>
class BstTree
{
public:
    explicit BstTree( const Key & notFound ,const Value &value);

    const Key & findMin( ) const;
    const Key & findMax( ) const;
    const Key & find( const Key & x ) const;
    bool isEmpty( ) const;
    void printTree( ) const;

    void makeEmpty( );
    void insert( const Key & x ,const Value &theval);
    void remove( const Key & x );
    Value * getNode(Key & element);

private:
    BstNode<Key,Value> *root;
    const Key ITEM_NOT_FOUND;

    const Key & elementAt( BstNode<Key,Value> *t ) const;

    void insert( const Key & x, const Value &theval ,BstNode<Key,Value> * & t ) const;
    void remove(const Key & x, BstNode<Key,Value> * & t ) const;

    BstNode<Key,Value> * findMin( BstNode<Key,Value> *t ) const;
    BstNode<Key,Value> * findMax( BstNode<Key,Value> *t ) const;
    BstNode<Key,Value> * find( const Key & x, BstNode<Key,Value> *t ) const;

    void makeEmpty( BstNode<Key,Value> * & t ) const;
    void printTree( BstNode<Key,Value> *t ) const;
    BstNode<Key,Value> * clone( BstNode<Key,Value> *t ) const;

    int height( BstNode<Key,Value> *t ) const;
    int max( int lhs, int rhs ) const;

};


template <class Key,class Value>void BstTree<Key,Value> :: insert (const Key &x, const Value &theval) //insert public
{
    
    insert (x,theval,root) ;
}






template <class Key,class Value>void BstTree<Key,Value> :: insert (const Key &x, const Value  &theval, BstNode<Key,Value> *&t)const //insert priv
{
    if (t==NULL)
        t=new BstNode<Key,Value>(x,theval,NULL,NULL) ;
    else if (x<t->word)
        insert (x,theval,t->left);
    else if (t->word<x)
        insert (x,theval,t->right) ;
    else
        ;
    
    
}




template <class Key,class Value>BstNode<Key,Value> *BstTree<Key,Value>::find( const Key & x, BstNode<Key,Value> * t ) const //find priv
{

    if ( t == NULL )
        return NULL;

    else if( x < t->word )
        return find( x, t->left );

    else if( t->word < x )
        return find( x, t->right );

    else
        return t; // Match
    //
}


template <class Key,class Value>const Key & BstTree<Key,Value>::find( const Key & x ) const //find public

{
    return elementAt( find( x, root ) );

}


template <class Key,class Value>
int BstTree<Key,Value>::max( int lhs, int rhs ) const
{
    if (lhs > rhs)
        return lhs;

    return rhs;
}



template <class Key,class Value>
int BstTree<Key,Value>::height( BstNode<Key,Value> *t ) const
{
    if (t == NULL)
        return -1;

    return t->height;
}


template <class Key,class Value>
BstNode<Key,Value> *BstTree<Key,Value>::findMin( BstNode<Key,Value> *t )const //find min priv
{

    if( t == NULL )

        return NULL;

    if( t->left == NULL )

        return t;

    return findMin( t->left );
}


template <class Key,class Value>const Key & BstTree<Key,Value>::findMin( ) const //find min public
{
    return elementAt( findMin( root ) );
}







template<class Key,class Value> BstNode<Key,Value>*BstTree<Key,Value>::findMax( BstNode<Key,Value> *t )const // find max priv
{
    if( t != NULL )

        while( t->right != NULL )

            t = t->right;

    return t;

}



template <class Key,class Value>const Key & BstTree<Key,Value>::findMax( ) const //find max public

{
    return elementAt( findMax( root ) );

}






template <class Key,class Value >void BstTree<Key,Value>::printTree( ) const //public print tree
{

    if( isEmpty( ) )

        cout << "Empty tree" << endl;

    else

        printTree( root );
}


template<class Key,class Value>void BstTree<Key,Value>::printTree( BstNode<Key,Value> * t ) const // private print tree
{
    if ( t != NULL )
    {
        printTree( t->left );
        cout << t->word << endl;
        printTree( t->right );
    }
}




template <class Key,class Value> void BstTree<Key,Value>::makeEmpty( ) //make empty public
{
    makeEmpty( root );

}



template <class Key,class Value> void BstTree<Key,Value>::makeEmpty( BstNode<Key,Value> * & t ) const //make empty priv

{
    if( t != NULL )
    {

        makeEmpty( t->left );

        makeEmpty( t->right );

        delete t;

    }

    t = NULL;
}




template<class Key,class Value> bool BstTree<Key,Value>::isEmpty()const  //ben yazdım isempty
{

    if (root==NULL)
        return true;
    else
        return false;
}


template <class Key,class Value> BstNode<Key,Value> *BstTree<Key,Value>::clone( BstNode<Key,Value> * t ) const
{
    if ( t == NULL )
        return NULL;
    else
        return new BstNode<Key,Value>
        ( t->word,
        clone( t->left ),
        clone( t->right ) );
}




template <class Key,class Value> BstTree<Key,Value>::BstTree(const Key & notFound, const Value &value ) :
ITEM_NOT_FOUND( notFound ), root( NULL )
{

}


template <class Key,class Value> const Key & BstTree<Key,Value>::elementAt( BstNode<Key,Value> *t ) const
{
    return t == NULL ? ITEM_NOT_FOUND  : t->word;
}




template<class Key,class Value>
Value * BstTree<Key,Value>::getNode(Key & element)
{

    return &find(element,root)->val;
}



#endif

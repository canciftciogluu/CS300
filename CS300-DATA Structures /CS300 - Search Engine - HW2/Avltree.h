/*  NAME: Eda nur demir
    ID: 28916
    COURSE: CS300
    ASS: HW2
 */

#ifndef AVLTREE_H
#define AVLTREE_H
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

// Template class for AVL Tree
template <class Key, class Value> class AvlTree;

template <class Key, class Value>
class AvlNode{
    Key key;
    Value value;
    AvlNode* left;
    AvlNode* right;
    int height;
    AvlNode(Key k=Key(), Value v=Value(), AvlNode<Key, Value>* l=nullptr, AvlNode<Key, Value>* r=nullptr, int h=0 ): key(k), value(v), left(l), right(r), height(h){};

    friend class AvlTree<Key, Value>;
};

template <class Key, class Value>
class AvlTree
{
public:
    AvlTree();
    AvlTree(const AvlTree &rhs);
    ~AvlTree();
    
    const Value & findMin() const;
    const Value & findMax() const;
    // Method to find a node in the AVL Tree
    const Value & find(const Key &x) const;
    
    bool isEmpty() const;
    void printTree() const;
    
    void makeEmpty();
    // Method to insert a node into the AVL Tree
    void insert(const Key &x, const Value &y);
    // Method to remove a node from the AVL Tree
    void remove(const Key &x);
    
    const AvlTree & operator=(const AvlTree & rhs);

private:
    AvlNode<Key, Value> *root;
    
    const Value & elementAt(AvlNode<Key, Value>* t) const;
    
    void insert(const Key &x, const Value &y, AvlNode<Key, Value>* &t) const;
    void remove(const Key &x, AvlNode<Key, Value>* &t) const;
    
    AvlNode<Key, Value>* findMin( AvlNode<Key, Value>* t) const;
    AvlNode<Key, Value>* findMax( AvlNode<Key, Value>* t) const;
    AvlNode<Key, Value>* find(const Key &x, AvlNode<Key, Value> *t) const;
    
    void makeEmpty(AvlNode<Key, Value>* &t) const;
    void printTree(AvlNode<Key, Value>* t) const;
    AvlNode<Key, Value>* clone(AvlNode<Key, Value>* t) const;
    void balance( AvlNode<Key, Value>*& )const ;
    int height(AvlNode<Key, Value>* t) const;
    int max(int lhs, int rhs) const;
    // Rotate node with left child to balance the tree
    void rotateWithLeftChild(AvlNode<Key, Value>* &k2) const;
    // Rotate node with right child to balance the tree
    void rotateWithRightChild(AvlNode<Key, Value>* &k1) const;
    // Double rotation with left child
    void doubleWithLeftChild(AvlNode<Key, Value>* &k3) const;
    // Double rotation with right child
    void doubleWithRightChild(AvlNode<Key, Value>* &k1) const;
};


static const int ALLOWED_BALANCE = 1;

//construct the tree
template <class Key, class Value>
AvlTree<Key, Value>::AvlTree() :root(nullptr){}

//internal method to get element field in node t (private)
//return the element field or ITEM_NOT_FOUND if t is NULL
template <class Key, class Value>
const Value & AvlTree<Key, Value>::elementAt(AvlNode<Key, Value>* t) const
{
    return t == nullptr ? nullptr: t->value;
    //if t == nullptr, return ITEM_NOT_FOUND, else return t->element
}

//find item x in the tree (public)
// Method to find a node in the AVL Tree
template <class Key, class Value>
const Value & AvlTree<Key, Value>::find(const Key &x) const
{
    return elementAt(find(x, root)); //these are private methods
}

// find function: Searches for a node with a given key in the AVL Tree
template <class Key, class Value>
AvlNode<Key, Value>* AvlTree<Key, Value>::find(const Key &x, AvlNode<Key, Value>* t) const
{
    if (t == nullptr)  // If the tree is empty, return nullptr
        return nullptr;

    while (true) {
        if (x < t->key) { // If x is less than current node's key, move to left subtree
            t = t->left;
        }
        else if (t->key < x) { // If x is greater than current node's key, move to right subtree
            t = t->right;
        }
        else { // Node found, return the node
            return t; // match
        }

        if (t == nullptr)  // Return nullptr if no match is found
            break; // If we reach a leaf node, exit the loop
    }

    return nullptr; // no match
}

// findMin public function: Finds the node with the minimum key in the AVL Tree
template <class Key, class Value>
const Value & AvlTree<Key, Value>::findMin() const
{
    return findMin(root)->value;  // Call the private findMin function starting from the root
}

// findMin private function: Recursive utility to find the minimum node starting from a given node
template <class Key, class Value>
AvlNode<Key, Value>* AvlTree<Key, Value>::findMin(AvlNode<Key, Value>* t) const
{
    if (t == nullptr) // If the subtree is empty, return nullptr
        return nullptr;

    if (t->left == nullptr)
        return t; // If there is no left child, the current node has the minimum key

    return findMin(t->left); // Recursively find the minimum in the left subtree
}

// findMax public function: Finds the node with the maximum key in the AVL Tree
template <class Key, class Value>
const Value & AvlTree<Key, Value>::findMax() const
{
    return findMax(root)->value; // Call the private findMax function starting from the root
}

// findMax private function: Iteratively finds the maximum node starting from a given node
template <class Key, class Value>
AvlNode<Key, Value>* AvlTree<Key, Value>::findMax(AvlNode<Key, Value>* t) const
{
    if (t == nullptr)
        return nullptr;  // If the subtree is empty, return nullptr

    while (t->right != nullptr) // Continue to the rightmost node
        t = t->right;

    return t;  // The rightmost node has the maximum key
}


// height function: Determines the height of a node in the AVL Tree
template <class Key, class Value>
int AvlTree<Key, Value>::height(AvlNode<Key, Value>* t) const
{
    return (t == nullptr) ? -1 : t->height; // Returns -1 for nullptr (indicating no node)
}

// max function: Returns the greater of two integers
template <class Key, class Value>
int AvlTree<Key, Value>::max(int lhs, int rhs) const
{
    return (lhs > rhs) ? lhs : rhs;  // Returns the larger of lhs and rhs
}

// insert function: Inserts a new node with key x and value y into the AVL Tree
template <class Key, class Value>
void AvlTree<Key, Value>::insert(const Key &x, const Value &y, AvlNode<Key, Value>* &t) const {
    if (t == nullptr) {
        // If the current node is null, create a new node with the given key and value
        t = new AvlNode<Key, Value>(x, y, nullptr, nullptr);
    }
    else if (x < t->key) {
        // If the key to be inserted is less than the current node's key, insert in the left subtree
        // x should be inserted into the left subtree;
        insert(x, y, t->left);
        
        // Check for and correct any left side imbalance
        if (height(t->left) - height(t->right) == 2) {
            if (x < t->left->key) {
                rotateWithLeftChild(t);
            }
            else {
                doubleWithLeftChild(t);
            }
        }
    }
    // If the key to be inserted is greater than the current node's key, insert in the right subtree
    else if (x > t->key) {
        // x should be inserted into the right subtree;
        insert(x, y, t->right);
        // Check for and correct any right side imbalance
        if (height(t->right) - height(t->left) == 2) {
            if (x > t->right->key) {
                rotateWithRightChild(t);
            }
            else {
                doubleWithRightChild(t);
            }
        }
    }
    // Update the height of the node after insertion
    t->height = 1 + std::max(height(t->left), height(t->right));
    
    // Balance the tree
    balance(t);
}


// Method to insert a node into the AVL Tree
template <class Key, class Value>
void AvlTree<Key, Value>::insert(const Key &x, const Value &y)
{
    insert(x, y, root);
}

// Method to remove a node from the AVL Tree
template <class Key, class Value>
void AvlTree<Key, Value>::remove(const Key &x){
    remove(x, root);
}

//remove x from the tree (private)
template <class Key, class Value>
void AvlTree<Key, Value>::remove(const Key &x, AvlNode<Key, Value>* &t) const {
    if (t == nullptr) {
        // The item was not found; do nothing
        return;
    }

    if (x < t->key) {
        // The item to be deleted is in the left subtree
        remove(x, t->left);
    } else if (x > t->key) {
        // The item to be deleted is in the right subtree
        remove(x, t->right);
    } else if (t->left != nullptr && t->right != nullptr) {
        // The node to be deleted has two children
        AvlNode<Key, Value>* minNode = findMin(t->right);
        t->key = minNode->key;
        t->value = minNode->value;
        remove(t->key, t->right); // Remove the minimum node in the right subtree
    } else {
        // The node to be deleted has one or no children
        AvlNode<Key, Value>* oldNode = t;
        t = (t->left != nullptr) ? t->left : t->right;
        delete oldNode;
    }
    // Rebalance the tree after deletion
    balance(t);
}

// Check if the tree is empty by examining the root pointer.
template <class Key, class Value>
bool AvlTree<Key, Value>::isEmpty() const{
    return root==nullptr;
}

// Public function to make the tree empty.
template <class Key, class Value>
void AvlTree<Key, Value>::makeEmpty(){
    makeEmpty(root);
}

// Recursive function to make the tree empty by deleting all nodes.
template <class Key, class Value>
void AvlTree<Key, Value>::makeEmpty(AvlNode<Key, Value>* &t) const{
    if (t!=nullptr){
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t=nullptr; // Set the root pointer to nullptr to indicate an empty tree.
}

template <class Key, class Value>
void AvlTree<Key, Value>::printTree() const {
    // Public function to print the elements of the tree.
    if (isEmpty()) {
        cout<<"Empty tree"<<endl;}
    else{
        printTree(root);}
}

// Recursive function to print the elements of the tree in-order.
template <class Key, class Value>
void AvlTree<Key, Value>::printTree(AvlNode<Key, Value>* t) const{
    
    if (t!=nullptr){
        
        printTree(t->left);
        
        cout<<t->key<<" "<<t->value<< endl;;
        
        printTree(t->right);}
}

// Destructor to release the memory occupied by the tree.
template <class Key, class Value>
AvlTree<Key, Value>::~AvlTree() {
    makeEmpty();
}

// Copy constructor to create a new tree as a copy of another tree.
template <class Key, class Value>
AvlTree<Key, Value>::AvlTree(const AvlTree<Key, Value> & rhs): root(nullptr) {
    *this=rhs;

}



// Assignment operator to copy the contents of another tree.
template <class Key, class Value>
const AvlTree<Key, Value>& AvlTree<Key, Value>::operator=(const AvlTree<Key, Value>& rhs){
    if (this !=&rhs){
        makeEmpty(); // Clear the current tree
        this->root=clone(rhs.root); // Clone the other tree's nodes
    }
    return *this;
}

// Recursive function to clone a node and its subtrees.
template <class Key, class Value>
AvlNode<Key, Value>* AvlTree<Key, Value>::clone(AvlNode<Key, Value>* t) const{
    if (t==nullptr){
        return nullptr;
    }
    AvlNode<Key, Value>* tmp=new AvlNode<Key, Value>(t->key, t->value, clone(t->left), clone(t->right));
    return tmp;
    
}

// Function to balance the AVL tree.
template<class Key, class Value>
void AvlTree<Key, Value>::balance(AvlNode<Key, Value> *& t)const {

    if (t == nullptr) {
        return;}

    if ( height( t->left ) - height( t->right ) > ALLOWED_BALANCE ) {
        if ( height( t->left->left ) >= height( t->left->right ) ) {
            rotateWithLeftChild(t);}
        else {
            doubleWithLeftChild(t);}}

    else if ( height(t->right) - height(t->left) > ALLOWED_BALANCE ){
        if (height(t->right->right) >= height(t->right->left)) {
            rotateWithRightChild(t);}
        else {
            doubleWithRightChild(t);}}

    t->height = max( height( t->left ), height( t->right ) ) + 1;}


// Rotate a node with its left child.
template <class Key, class Value>
void AvlTree<Key, Value>::rotateWithLeftChild(AvlNode<Key, Value>* &k) const {
    // Assign k's left child to a temporary node
    AvlNode<Key, Value>* leftChild = k->left;
    // Replace k's left child with the right child of k's left child
    k->left = k->left->right;
    // Set the right child of k's old left child to k
    leftChild->right = k;
    // Update the height of k
    k->height = max(height(k->left), height(k->right)) + 1;
    // Update the height of k's old left child
    leftChild->height = max(height(leftChild->left), height(k)) + 1;
    // Update k
    k = leftChild;
}

template <class Key, class Value>
void AvlTree<Key, Value>::rotateWithRightChild(AvlNode<Key, Value>* &k) const {
// Rotate the given node with its right child in a single rotation.
// Store a reference to the right child in a temporary variable.
    AvlNode<Key, Value>* rightChild = k->right;
// Update the right child of the current node to be the left child of the former right child.
    k->right = k->right->left;
// Update the left child of the former right child to be the current node.
    rightChild->left = k;
// Update the height of the current node based on the heights of its left and right subtrees.
    k->height = max(height(k->left), height(k->right)) + 1;
// Update the height of the former right child based on the heights of its right subtree and the updated current node.
  
    rightChild->height = max(height(rightChild->right), height(k)) + 1;
// Update the current node to point to the former right child, which is now the new root of this subtree.
    k = rightChild;
}

template <class Key, class Value>

void AvlTree<Key, Value>::doubleWithLeftChild(AvlNode<Key, Value>* &k) const {
    // Perform a double rotation with a left child node.
    // First, perform a single right rotation on the left child.
    rotateWithRightChild(k->left);
    // Then, perform a single left rotation on the current node.
    rotateWithLeftChild(k);
}

template <class Key, class Value>

void AvlTree<Key, Value>::doubleWithRightChild(AvlNode<Key, Value>* &k) const {
    // Perform a double rotation with a right child node.
    // First, perform a single left rotation on the right child.
    rotateWithLeftChild(k->right);
    // Then, perform a single right rotation on the current node.
    rotateWithRightChild(k);
}

#endif
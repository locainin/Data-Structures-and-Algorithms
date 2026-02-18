//  AVLTree.hpp
//  AVL-Tree
//  File contains class for the Node and the AVLTree

#ifndef AVLTree_hpp
#define AVLTree_hpp
#include <string>
using std::string;
 
// Node class represents a node in the AVL tree
class Node {
public:
    string key;        // The value or key of the node
    Node *left;     // Pointer to the left child node
    Node *right;    // Pointer to the right child node
    int height;     // Height of the node, used for balancing

    Node(const string &keyValue);    // Constructor for creating a new node with a key
};

// AVLTree class defines the AVL tree and its operations
class AVLTree {
public:
    Node *insert(Node *rootNode, const string &keyValue);  // Function to insert a new key into the tree

private:
    int height(Node *nodePointer);                // Function to get the height of a node
    Node *rightRotate(Node *subtreeRoot);         // Function to perform right rotation
    Node *leftRotate(Node *subtreeRoot);          // Function to perform left rotation
    int getBalance(Node *nodePointer);            // Function to get the balance factor of a node
};

#endif /* AVLTree_hpp */

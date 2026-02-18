//  AVLTree.cpp
//  Implementation of the AVL Tree
#include "AVLTree.hpp" // Include the header file for AVL tree
#include <algorithm>   // Include the algorithm library for max

using std::max;
// Node constructor to initialize a new node
// Initializes a new node with the given key, and sets left and right children to nullptr, height to 1
Node::Node(const string &initialKey) : key(initialKey), left(nullptr), right(nullptr), height(1) {}
// Function to get the height of a node in the AVL tree
int AVLTree::height(Node *nodePointer) {
  if (nodePointer == nullptr) {
    // If the node is null, its height is 0
    return 0;
  }
  // Return the height of the node
  return nodePointer->height;
}
// Function to perform right rotation on a subtree rooted with y
Node *AVLTree::rightRotate(Node *subtreeRoot) {
  Node *newRoot = subtreeRoot->left;     // new root is left child
  Node *movedSubtree = newRoot->right;   // subtree moved during rotation
  // Perform rotation
  newRoot->right = subtreeRoot;
  subtreeRoot->left = movedSubtree;
  // Update heights after rotation
  subtreeRoot->height = max(height(subtreeRoot->left), height(subtreeRoot->right)) + 1;
  newRoot->height = max(height(newRoot->left), height(newRoot->right)) + 1;
  // Return new root after rotation
  return newRoot;
}
// Function to perform left rotation on a subtree rooted with x
Node *AVLTree::leftRotate(Node *subtreeRoot) {
  Node *newRoot = subtreeRoot->right;  // new root is right child
  Node *movedSubtree = newRoot->left;  // subtree moved during rotation
  // Perform rotation
  newRoot->left = subtreeRoot;
  subtreeRoot->right = movedSubtree;
  // Update heights after rotation
  subtreeRoot->height = max(height(subtreeRoot->left), height(subtreeRoot->right)) + 1;
  newRoot->height = max(height(newRoot->left), height(newRoot->right)) + 1;
  // Return new root after rotation
  return newRoot;
}
// Function to get the balance factor of a node N
int AVLTree::getBalance(Node *nodePointer) {
  if (nodePointer == nullptr) {
    // If the node is null, balance factor is 0
    return 0;
  }
  // Balance factor is the difference in heights between left and right subtrees
  return height(nodePointer->left) - height(nodePointer->right);
}
// Function to insert a new key into the AVL tree
Node *AVLTree::insert(Node *rootNode, const string &keyValue) {
  // Perform normal BST insertion
  if (rootNode == nullptr) {
    return new Node(keyValue);
  }
  if (keyValue < rootNode->key) {
    rootNode->left = insert(rootNode->left, keyValue);
  } else if (keyValue > rootNode->key) {
    rootNode->right = insert(rootNode->right, keyValue);
  } else {
    // Duplicate keys are not allowed, return the existing node
    return rootNode;
  }
  // Update height of this ancestor node
  rootNode->height = 1 + max(height(rootNode->left), height(rootNode->right));
  // Get the balance factor of this node to check whether it's balanced
  int balanceFactor = getBalance(rootNode);
  // Perform rotations to balance the tree if necessary
  // Left Left Case
  if (balanceFactor > 1 && keyValue < rootNode->left->key) {
    // Perform a right rotation
    return rightRotate(rootNode);
  }
  // Right Right Case
  if (balanceFactor < -1 && keyValue > rootNode->right->key) {
    // Perform a left rotation
    return leftRotate(rootNode);
  }
  // Left Right Case
  if (balanceFactor > 1 && keyValue > rootNode->left->key) {
    // First, perform a left rotation on the left child
    rootNode->left = leftRotate(rootNode->left);
    // Then, perform a right rotation on the current node
    return rightRotate(rootNode);
  }
  // Right Left Case
  if (balanceFactor < -1 && keyValue < rootNode->right->key) {
    // First, perform a right rotation on the right child
    rootNode->right = rightRotate(rootNode->right);
    // Then, perform a left rotation on the current node
    return leftRotate(rootNode);
  }
  // Return the node pointer (unchanged if no rotations were necessary)
  return rootNode;
}

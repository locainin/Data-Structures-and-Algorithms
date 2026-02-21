//  main.cpp
//  AVL-Tree
// Demo of AVL Tree
#include "AVLTree.hpp"
#include <iostream>
#include <vector>

using std::cout;
using std::size_t;
using std::vector;
void inorderTraversal(Node *rootNode);
void printTree(const Node *rootNode, int depthLevel = 0);
void destroyTree(Node *rootNode);

int main() {
  Node *ptrRoot = nullptr;
  AVLTree avlTree;

  const vector<string> insertionOrder{"Mary", "Sam", "Bart", "Sue", "Joe", "Kevin"};

  cout << "================ AVL Tree Build ================\n";
  for (const string &name : insertionOrder) {
    ptrRoot = avlTree.insert(ptrRoot, name);
    cout << "[insert] " << name << "  ->  root now: " << ptrRoot->key << '\n';
  }

  cout << "\nInsertion order: ";
  for (size_t index = 0; index < insertionOrder.size(); index++) {
    cout << insertionOrder[index];
    if (index + 1 < insertionOrder.size()) {
      cout << " -> ";
    }
  }

  cout << "\n\nTree shape (right branch shown above left branch)\n";
  cout << "-------------------------------------------------\n";
  printTree(ptrRoot);

  cout << "\nInorder traversal (sorted keys): ";
  inorderTraversal(ptrRoot);
  cout << '\n';

  destroyTree(ptrRoot);
  return 0;
}
/* This function recursively traverses the tree in an inorder manner, meaning it visits the left child, then
 * the node itself, and then the right child. This traversal method ensures that the nodes are visited in
 * ascending order of their keys for a binary search tree.
 */
void inorderTraversal(Node *rootNode) {
  // Check if the current node is not null
  if (rootNode != nullptr) {
    // Recursively traverse the left subtree first
    inorderTraversal(rootNode->left);
    // Visit/Print the root node of the current subtree
    cout << rootNode->key << " ";
    // Recursively traverse the right subtree
    inorderTraversal(rootNode->right);
  }
  // If the current node is null, the function returns,
  // effectively ending the traversal in this path of the tree
}

void printTree(const Node *rootNode, int depthLevel) {
  if (rootNode == nullptr) {
    return;
  }

  printTree(rootNode->right, depthLevel + 1);

  cout << string(static_cast<size_t>(depthLevel * 4), ' ') << "- " << rootNode->key << " (h="
       << rootNode->height << ")\n";

  printTree(rootNode->left, depthLevel + 1);
}

void destroyTree(Node *rootNode) {
  if (rootNode == nullptr) {
    return;
  }
  destroyTree(rootNode->left);
  destroyTree(rootNode->right);
  delete rootNode;
}

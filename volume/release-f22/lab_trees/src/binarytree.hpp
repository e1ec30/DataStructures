/**
 * @file binarytree.hpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */
#include "InorderTraversal.h"
#include <iostream>

/**
 * @return The height of the binary tree. Recall that the height of a binary
 *  tree is just the length of the longest path from the root to a leaf, and
 *  that the height of an empty tree is -1.
 */
template <typename T> int BinaryTree<T>::height() const {
  // Call recursive helper function on root
  return height(root);
}

/**
 * Private helper function for the public height function.
 * @param subRoot
 * @return The height of the subtree
 */
template <typename T> int BinaryTree<T>::height(const Node *subRoot) const {
  // Base case
  if (subRoot == NULL)
    return -1;

  // Recursive definition
  return 1 + std::max(height(subRoot->left), height(subRoot->right));
}

/**
 * Prints out the values of the nodes of a binary tree in order.
 * That is, everything to the left of a node will be printed out before that
 * node itself, and everything to the right of a node will be printed out after
 * that node.
 */
template <typename T> void BinaryTree<T>::printLeftToRight() const {
  // Call recursive helper function on the root
  printLeftToRight(root);

  // Finish the line
  std::cout << std::endl;
}

/**
 * Private helper function for the public printLeftToRight function.
 * @param subRoot
 */
template <typename T>
void BinaryTree<T>::printLeftToRight(const Node *subRoot) const {
  // Base case - null node
  if (subRoot == NULL)
    return;

  // Print left subtree
  printLeftToRight(subRoot->left);

  // Print this node
  std::cout << subRoot->elem << ' ';

  // Print right subtree
  printLeftToRight(subRoot->right);
}

/**
 * Flips the tree over a vertical axis, modifying the tree itself
 *  (not creating a flipped copy).
 */
template <typename T> void BinaryTree<T>::mirror() {
  // your code here
  mirror(root);
}

template <typename T> void BinaryTree<T>::mirror(Node *subroot) {
  // e1ec30: base case
  if (subroot == NULL)
    return;

  // e1ec30: mirror the children
  mirror(subroot->left);
  mirror(subroot->right);

  // e1ec30: swap the pointers
  Node *tmp = subroot->left;
  subroot->left = subroot->right;
  subroot->right = tmp;
}

/**
 * isOrdered() function iterative version
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T> bool BinaryTree<T>::isOrderedIterative() const {
  // your code here
  //
  InorderTraversal<T> it(root);
  auto i = it.begin();
  T prev = (*i)->elem;

  while (++i != it.end()) {
    T cur = (*i)->elem;
    if (prev > cur)
      return false;
    prev = cur;
  }

  return true;
}

/**
 * isOrdered() function recursive version
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T> bool BinaryTree<T>::isOrderedRecursive() const {
  // your code here

  printLeftToRight();

  T l = isOrderedRecursive(root->left, root->elem, true);
  T r = isOrderedRecursive(root->right, root->elem, false);
  printf("comparing %d <= %d <= %d\n", l, root->elem, r);
  return (l <= root->elem && root->elem <= r);
}

template <typename T>
T BinaryTree<T>::isOrderedRecursive(Node *n, T cmp, bool gt) const {
  // e1ec30: there has to be a better way to do this, but it works.
  printf("comparing: %d, ", cmp);
  if (n == NULL) {
    printf("%d won\n", cmp);
    return cmp;
  }

  if (gt) {
    T cmp_n = (n->elem >= cmp) ? n->elem : cmp;
    T l = isOrderedRecursive(n->left, cmp_n, true);
    T r = isOrderedRecursive(n->right, cmp_n, true);

    if (l >= r)
      return l;
    else
      return r;

  } else {
    T cmp_n = (n->elem < cmp) ? n->elem : cmp;
    T l = isOrderedRecursive(n->left, cmp_n, false);
    T r = isOrderedRecursive(n->right, cmp_n, false);

    if (l < r)
      return l;
    return r;
  }
}

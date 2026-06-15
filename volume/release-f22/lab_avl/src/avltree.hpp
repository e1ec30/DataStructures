/**
 * @file avltree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

#include "avltree.h"
template <class K, class V> V AVLTree<K, V>::find(const K &key) const {
  return find(root, key);
}

template <class K, class V>
struct AVLTree<K, V>::Node *&AVLTree<K, V>::rightMost(Node *&subtree) {
  if (subtree->right == NULL)
    return subtree;
  else
    return rightMost(subtree->right);
}

template <class K, class V>
V AVLTree<K, V>::find(Node *subtree, const K &key) const {
  if (subtree == NULL)
    return V();
  else if (key == subtree->key)
    return subtree->value;
  else {
    if (key < subtree->key)
      return find(subtree->left, key);
    else
      return find(subtree->right, key);
  }
}

template <class K, class V> void AVLTree<K, V>::rotateLeft(Node *&t) {
  functionCalls.push_back(
      "rotateLeft"); // Stores the rotation name (don't remove this)
  // your code here

  Node *r = t->right;
  Node *l = r->left;

  r->left = t;
  t->right = l;
  updateHeight(t);
  updateHeight(r);
  t = r;
}

template <class K, class V> void AVLTree<K, V>::rotateLeftRight(Node *&t) {
  functionCalls.push_back(
      "rotateLeftRight"); // Stores the rotation name (don't remove this)
  // Implemented for you:
  rotateLeft(t->left);
  rotateRight(t);
}

template <class K, class V> void AVLTree<K, V>::rotateRightLeft(Node *&t) {
  functionCalls.push_back("rotateRightLeft"); // Stores the rotation name (don't
                                              // remove this) your code here
  rotateRight(t->right);
  rotateLeft(t);
}

template <class K, class V> void AVLTree<K, V>::rotateRight(Node *&t) {
  functionCalls.push_back(
      "rotateRight"); // Stores the rotation name (don't remove this)
  // your code here

  Node *l = t->left;
  Node *r = l->right;

  l->right = t;
  t->left = r;
  updateHeight(t);
  updateHeight(r);
  t = l;
}

// e1ec30: I shouldn't compute the height on every time, but let's do one thing
// at a time.
template <class K, class V> void AVLTree<K, V>::updateHeight(Node *&subtree) {
  if (subtree == NULL)
    return;
  subtree->height =
      (1 + std::max(heightOrNeg1(subtree->left), heightOrNeg1(subtree->right)));
}
template <class K, class V> int AVLTree<K, V>::balance(Node *&subtree) {
  return (heightOrNeg1(subtree->left) - heightOrNeg1(subtree->right));
}

template <class K, class V> void AVLTree<K, V>::rebalance(Node *&subtree) {
  // your code here
  //
  int b = balance(subtree);

  if (b > 1) {
    if (balance(subtree->left) >= 0) {
      rotateRight(subtree); // rotateRight for a LL imbalance
    } else {
      rotateLeftRight(subtree); // rotateLeftRight for a LR imbalance
    }
  } else if (b < -1) {
    if (balance(subtree->right) <= 0) {
      rotateLeft(subtree); // rotateLeft for a RR imbalance
    } else {
      rotateRightLeft(subtree); // rotateRightLeft for a RL imbalance
    }
  }
  updateHeight(subtree);
}

template <class K, class V>
void AVLTree<K, V>::insert(const K &key, const V &value) {
  insert(root, key, value);
  updateHeight(root);
}

template <class K, class V>
void AVLTree<K, V>::insert(Node *&subtree, const K &key, const V &value) {
  // your code here
  if (!subtree) {
    subtree = new Node(key, value);
    return;
  }
  if (subtree->key == key) {
    return;
  }

  if (subtree->key > key) {
    if (subtree->left != NULL)
      insert(subtree->left, key, value);
    else {
      subtree->left = new Node(key, value);
    }
  } else if (subtree->key < key) {
    if (subtree->right != NULL)
      insert(subtree->right, key, value);
    else {
      subtree->right = new Node(key, value);
    }
  }
  updateHeight(subtree);
  rebalance(subtree);
}

template <class K, class V> void AVLTree<K, V>::remove(const K &key) {
  remove(root, key);
}

template <class K, class V>
void AVLTree<K, V>::remove(Node *&subtree, const K &key) {
  if (subtree == NULL)
    return;

  if (key < subtree->key) {
    // your code here
    remove(subtree->left, key);
    updateHeight(subtree);
    rebalance(subtree);
  } else if (key > subtree->key) {
    // your code here
    remove(subtree->right, key);
    updateHeight(subtree);
    rebalance(subtree);
  } else {
    if (subtree->left == NULL && subtree->right == NULL) {
      /* no-child remove */
      // your code here
      delete subtree;
      subtree = NULL;
      return; // go back one level;
    } else if (subtree->left != NULL && subtree->right != NULL) {
      /* two-child remove */
      // your code here
      Node *&r = rightMost(subtree->left);
      swap(subtree, r);
      remove(r, key);
    } else {
      /* one-child remove */
      // your code here
      if (subtree->left != NULL) {
        // only left child
        Node *l = subtree->left;
        swap(subtree, subtree->left);
        subtree->right = subtree->left->right;
        subtree->left = subtree->left->left;
        delete l;
        l = NULL;
      } else {
        // only right child
        Node *r = subtree->right;
        swap(subtree, subtree->right);
        subtree->left = subtree->right->left;
        subtree->right = subtree->right->right;
        delete r;
        r = NULL;
      }
    }
    // your code here
    updateHeight(subtree);
    rebalance(subtree);
  }
}

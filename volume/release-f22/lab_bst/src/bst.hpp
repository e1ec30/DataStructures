/**
 * @file bst.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */

#include "bst.h"
#include <algorithm>
template <class K, class V> V BST<K, V>::find(const K &key) {
  // your code here
  BST<K, V>::Node *&found = find(root, key);
  if (found && found->key == key)
    return found->value;
  else
    return {};
}

template <class K, class V>
struct BST<K, V>::Node *&BST<K, V>::find(Node *&subtree, const K &key) {
  // Your code here
  // e1ec30: This will traverse the tree in search of key, if it's found the
  // node is returned, otherwise it returns the node which will be the parent of
  // key if it were to be inserted

  if (subtree == NULL || subtree->key == key)
    return subtree;

  if (subtree->key > key) {
    // e1ec30: go left

    if (subtree->left != NULL)
      return find(subtree->left, key);
    else
      return subtree;
  }

  else /*if (subtree->key < key)*/ {
    // e1ec30: go right

    if (subtree->right != NULL)
      return find(subtree->right, key);
    else
      return subtree;
  }
}

template <class K, class V>
void BST<K, V>::insert(const K &key, const V &value) {
  // your code here

  if (root == NULL) {
    root = new Node(key, value);
    return;
  }
  insert(root, key, value);
}

template <class K, class V>
void BST<K, V>::insert(Node *&subtree, const K &key, const V &value) {
  // your code here

  Node *&found = find(subtree, key);
  if (found->key == key)
    return; // the key alread exists, do nothing.

  else {
    if (found->key > key) {
      found->left = new Node(key, value);
      return;
    } else {
      found->right = new Node(key, value);
    }
  }
}

template <class K, class V> void BST<K, V>::swap(Node *&first, Node *&second) {
  // your code here

  K fk = first->key;
  V fv = first->value;

  // e1ec30: swap values.
  first->key = second->key;
  first->value = second->value;
  second->key = fk;
  second->value = fv;
}

template <class K, class V> void BST<K, V>::remove(const K &key) {
  // your code here
  remove(root, key);
}

template <class K, class V>
struct BST<K, V>::Node *&BST<K, V>::rightmost(Node *&subtree) {
  if (subtree->right == NULL)
    return subtree;
  else
    return rightmost(subtree->right);
}

template <class K, class V>
void BST<K, V>::remove(Node *&subtree, const K &key) {
  // your code here
  Node *&found = find(subtree, key);

  if (!found || found->key != key)
    return; // do nothing if key does not exist.

  // if no children
  if (found->left == NULL && found->right == NULL) {
    if (found != NULL) {
      delete found;
      found = NULL;
    }
    return;
  }

  // if only right child
  else if (found->left == NULL) {
    Node *r = found->right;
    swap(found, found->right);
    found->left = found->right->left;
    found->right = found->right->right;
    delete r;
    return;
  }

  // if only left child
  else if (found->right == NULL) {
    Node *l = found->left;
    swap(found, found->left);
    found->right = found->left->right;
    found->left = found->left->left;
    delete l;
    return;
  }

  // if both children
  else {
    // find the InOrderPredecessor
    Node *&pred = rightmost(found->left);

    swap(found, pred);
    remove(pred, pred->key);
    return;
  }
}

template <class K, class V>
BST<K, V> listBuild(std::vector<std::pair<K, V>> inList) {
  // your code here
  BST<K, V> ret{};
  for (auto [key, value] : inList) {
    // printf("(%s, %d)\n", key.c_str(), value);
    ret.insert(key, value);
  }
  return ret;
}

template <class K, class V>
std::vector<int> allBuild(std::vector<std::pair<K, V>> inList) {
  // your code here
	std::vector<int> results;
	results.resize(inList.size(), 0);
	
	std::sort(inList.begin(), inList.end());

	do {
		int h = listBuild(inList).height();
		// printf("height: %d\n", h);
		results[h]++;
	} while (std::next_permutation(inList.begin(), inList.end()));
	
  return results;
}

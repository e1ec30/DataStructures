/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

#include "kdtree.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iterator>
#include <utility>
#include <vector>

using namespace std;

template <int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim> &first,
                                const Point<Dim> &second, int curDim) const {
  /**
   * @todo Implement this function!
   */
  if ((first[curDim]) < (second[curDim]))
    return true;
  if (first[curDim] == second[curDim])
    return first < second;
  return false;
}

template <int Dim> int distance(const Point<Dim> &p1, const Point<Dim> &p2) {
  int result = 0;
  for (int i = 0; i < Dim; i++)
    result += std::pow((p1[i] - p2[i]), 2);
  return result;
}

template <int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim> &target,
                                const Point<Dim> &currentBest,
                                const Point<Dim> &potential) const {
  /**
   * @todo Implement this function!
   */
  if (distance(potential, target) < distance(currentBest, target))
    return true;
  if (distance(potential, target) == distance(currentBest, target))
    return potential < currentBest;
  return false;
}

template <int Dim>
void KDTree<Dim>::partition(int left, int right, int dimension, int idx) {
  // e1ec30: This function will swap things around so that element[idx] is in
  // its final position
  //  I'm just taking points[right] as my pivot

  // printf("Partition: left: %d, right:%d, dimension: %d, idx: %d\n", left, right,
         // dimension, idx);

  // printf("{ ");

  for (auto p : points_) {
    // std::cout << p << ", ";
  }

  // printf(" }\n");

  auto swap = [this](int l, int r) {
    if (l == r) {
      // puts("");
      return;
    }
    // printf("swap(%d, %d)\n", l, r);
    Point<Dim> p = points_[l];
    points_[l] = points_[r];
    points_[r] = p;
  };

  int l = left;

  Point pivot_val = points_[right];
  for (int i = left; i < right; i++) {
    if (smallerDimVal(points_[i], pivot_val, dimension)) {
      swap(l, i);
      l++;
    }
  }

  // printf("final swap: ");
  swap(l, right);

  if (l == idx) {
    // printf("done with this\n");
    return;
  }
  if (l < idx)
    return partition(l + 1, right, dimension, idx);
  if (l > idx)
    return partition(left, l - 1, dimension, idx);

  return;
}

template <int Dim>
KDTree<Dim>::KDTree(const vector<Point<Dim>> &newPoints) : points_(newPoints) {
  /**
   * @todo Implement this function!
   */
  if (points_.size() == 0)
    return;
  // printf("size: %zu\n", points_.size());
  root = makeTree(0, points_.size() - 1, 0);
  // printf("root: %p\n", (void *)root);
}

template <int Dim>
typename KDTree<Dim>::KDTreeNode *KDTree<Dim>::makeTree(int left, int right,
                                                        int splitting_dim) {

  // printf("Tree: left: %d, right: %d\n", left, right);
  if (left > right)
    return NULL;
  int median = (left + right) / 2;
  partition(left, right, splitting_dim, median);

  // e1ec30: construct the tree and it's left and right children
  KDTreeNode *subroot = new KDTreeNode(points_[median]);

  subroot->left = makeTree(left, median - 1, (splitting_dim + 1) % Dim);
  subroot->right = makeTree(median + 1, right, (splitting_dim + 1) % Dim);
  return subroot;
}
template <int Dim> KDTree<Dim>::KDTree(const KDTree<Dim> &other) {
  /**
   * @todo Implement this function!
   */
}

template <int Dim>
const KDTree<Dim> &KDTree<Dim>::operator=(const KDTree<Dim> &rhs) {
  /**
   * @todo Implement this function!
   */

  return *this;
}

template <int Dim> KDTree<Dim>::~KDTree() {
  /**
   * @todo Implement this function!
   */
  if (root)
    delete root;
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim> &query) const {
  /**
   * @todo Implement this function!
   */

  // let's start small
  return findNearest(root, query, root->point, 0);
}

template <int Dim>
Point<Dim> KDTree<Dim>::findNearest(KDTree<Dim>::KDTreeNode *root,
                                    const Point<Dim> query, const Point<Dim> best, int dimension) const {
  Point<Dim> new_best = best;
  bool left = false;
  (void)left;

  if (!root) return best;

  if (smallerDimVal(query, root->point, dimension)) {
    // e1ec30: if we're smaller go left
    new_best = findNearest(root->left, query, new_best, (dimension + 1) % Dim);
    left = true;
  }
  else {
    new_best = findNearest(root->right, query, new_best, (dimension + 1) % Dim);
  }

  // e1ec30: if the this node is closer than what we found at the child
  if (shouldReplace(query, new_best, root->point)) new_best = root->point;


  // e1ec30: if the distance of this node to the query (on the splitting dimension) is smaller than the total distance of the current best point to the query, there's a chance there might be a better node on the other child.
  // e1ec30: in effect, we're checking if the query is closer to the hyperrectangle than the best point we have, if so, then perhaps there's another point just across the plane that's closer.
  int current_distance = distance(new_best, query);
  int splitting = std::pow(((root->point)[dimension] - query[dimension]), 2);

  if (splitting <= current_distance) {
    if (left){
      Point<Dim> other = findNearest(root->right, query, new_best, (dimension + 1) % Dim);
      if (shouldReplace(query, new_best, other)) new_best = other;
    } 
    else {
      Point<Dim> other = findNearest(root->left, query, new_best, (dimension + 1) % Dim);
      if (shouldReplace(query, new_best, other)) new_best = other;
    }
  }

  return new_best;
}

/**
 * @file NimLearner.cpp
 * CS 225: Data Structures
 */

#include "nim_learner.h"
#include "nim_graph/edge.h"
#include <ctime>
#include <sstream>
#include <string>
#include <unordered_map>

/**
 * Constructor to create a game of Nim with `startingTokens` starting tokens.
 *
 * This function creates a graph, `g_` representing all of the states of a
 * game of Nim with vertex labels "p#-X", where:
 * - # is the current player's turn; p1 for Player 1, p2 for Player2
 * - X is the tokens remaining at the start of a player's turn
 *
 * For example:
 *   "p1-4" is Player 1's turn with four (4) tokens remaining
 *   "p2-8" is Player 2's turn with eight (8) tokens remaining
 *
 * All legal moves between states are created as edges with initial weights
 * of 0.
 *
 * @param startingTokens The number of starting tokens in the game of Nim.
 */
NimLearner::NimLearner(unsigned startingTokens) : g_(true, true) {
  /* Your code goes here! */
  int remaining_tokens = startingTokens;

  while (remaining_tokens >= 0) {
    Vertex p1_rem("p1-" + to_string(remaining_tokens));
    Vertex p2_rem("p2-" + to_string(remaining_tokens));
    Vertex p1_1("p2-" + to_string(remaining_tokens - 1));
    Vertex p1_2("p2-" + to_string(remaining_tokens - 2));
    Vertex p2_1("p1-" + to_string(remaining_tokens - 1));
    Vertex p2_2("p1-" + to_string(remaining_tokens - 2));
    if (remaining_tokens == 0) {
      g_.insertVertex(p1_rem);
      g_.insertVertex(p2_rem);
    } else {
      if (remaining_tokens >= 1) {
        g_.insertEdge(p1_rem, p1_1);
        g_.insertEdge(p2_rem, p2_1);
        g_.setEdgeWeight(p1_rem, p1_1, 0);
        g_.setEdgeWeight(p2_rem, p2_1, 0);
      }
      if (remaining_tokens >= 2) {
        g_.insertEdge(p1_rem, p1_2);
        g_.insertEdge(p2_rem, p2_2);
        g_.setEdgeWeight(p1_rem, p1_2, 0);
        g_.setEdgeWeight(p2_rem, p2_2, 0);
      }
    }
    remaining_tokens--;
  }
  startingVertex_ = Vertex("p1-" + to_string(startingTokens));
}

/**
 * Plays a random game of Nim, returning the path through the state graph
 * as a vector of `Edge` classes.  The `origin` of the first `Edge` must be
 * the vertex with the label "p1-#", where # is the number of starting
 * tokens.  (For example, in a 10 token game, result[0].origin must be the
 * vertex "p1-10".)
 *
 * @returns A random path through the state space graph.
 */
std::vector<Edge> NimLearner::playRandomGame() const {
  vector<Edge> path;
  /* Your code goes here! */
  vector<Edge> edges = g_.getEdges();
  Edge curr;

  // get the starting edge
  for (auto edge : edges) {
    if (edge.source == startingVertex_) {
      curr = edge;
      break;
    }
  }

  while (curr.dest.back() != '0') {
    vector<Edge> possible;
    possible.reserve(2);
    for (auto edge : edges) {
      if (edge.source == curr.dest) {
        possible.push_back(edge);
      }
    }
    int r = rand() % possible.size();
    path.push_back(curr);
    curr = possible[r];
  }
  path.push_back(curr);

  return path;
}

/*
 * Updates the edge weights on the graph based on a path through the state
 * tree.
 *
 * If the `path` has Player 1 winning (eg: the last vertex in the path goes
 * to Player 2 with no tokens remaining, or "p2-0", meaning that Player 1
 * took the last token), then all choices made by Player 1 (edges where
 * Player 1 is the source vertex) are rewarded by increasing the edge weight
 * by 1 and all choices made by Player 2 are punished by changing the edge
 * weight by -1.
 *
 * Likewise, if the `path` has Player 2 winning, Player 2 choices are
 * rewarded and Player 1 choices are punished.
 *
 * @param path A path through the a game of Nim to learn.
 */
void NimLearner::updateEdgeWeights(const std::vector<Edge> &path) {
  /* Your code goes here! */
  Vertex v = path.back().dest;
  if (v.back() == '0') {
    string winner = path.back().source.substr(0, 2);
    for (auto edge: path) {
      // printf("(%s, %s, %d)\n", edge.source.c_str(), edge.dest.c_str(), edge.getWeight());
      int oldweight = g_.getEdgeWeight(edge.source, edge.dest);
      if (edge.source.substr(0, 2) == winner) {
        g_.setEdgeWeight(edge.source, edge.dest, oldweight+1);
      }
      else {
        g_.setEdgeWeight(edge.source, edge.dest, oldweight-1);
      }
      // printf("(%s, %s, %d)\n", edge.source.c_str(), edge.dest.c_str(), edge.getWeight());
    }
  }
}

/**
 * Label the edges as "WIN" or "LOSE" based on a threshold.
 */
void NimLearner::labelEdgesFromThreshold(int threshold) {
  for (const Vertex &v : g_.getVertices()) {
    for (const Vertex &w : g_.getAdjacent(v)) {
      int weight = g_.getEdgeWeight(v, w);

      // Label all edges with positve weights as "WINPATH"
      if (weight > threshold) {
        g_.setEdgeLabel(v, w, "WIN");
      } else if (weight < -1 * threshold) {
        g_.setEdgeLabel(v, w, "LOSE");
      }
    }
  }
}

/**
 * Returns a constant reference to the state space graph.
 *
 * @returns A constant reference to the state space graph.
 */
const Graph &NimLearner::getGraph() const { return g_; }

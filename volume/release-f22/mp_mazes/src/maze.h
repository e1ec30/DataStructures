/* Your code here! */
#pragma once
#include "cs225/PNG.h"
#include "dsets.h"
#include <vector>

enum { RIGHT = 0, DOWN = 1, LEFT = 2, UP = 3 };
enum { NOWALL = 0, RIGHTWALL = 1, DOWNWALL = 2, BOTHWALLS = 3 };

class SquareMaze {
public:
  SquareMaze();
  bool canTravel(int, int, int) const;

  void makeMaze(int, int);

  void setWall(int, int, int, bool);

  std::vector<int> solveMaze();

  cs225::PNG *drawMaze() const;

  cs225::PNG *drawMazeWithSolution();

  void printSolution(const std::vector<int> &solution);

private:
  int width_;
  int height_;
  std::vector<int> squares; // Index is the square, value is the walls. Didn't
                            // come up with this, found it on the internet.
  DisjointSets sets;

  // e1ec30: Helpers to make it easy to make it easy to convert between
  // coordinates and an index;
  int getIndex(int x, int y) const { return (y * width_) + x; }
  int getX(int index) const { return (index % width_); }
  int getY(int index) const { return (index / width_); }
};

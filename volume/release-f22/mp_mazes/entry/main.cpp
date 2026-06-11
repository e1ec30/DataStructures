#include "cs225/PNG.h"
#include "dsets.h"
#include "maze.h"
#include <cstddef>
#include <iostream>

using namespace std;

int main() {
  // Write your own main here
  // std::srand(time(nullptr));
  // cout << "Add your own tests here! Modify main.cpp" << endl;
  int x = 2, y = 2;
  cs225::PNG *plain = new cs225::PNG(x * 10, y * 10);
  plain->writeToFile("plain2x2.png");
  delete plain;
  SquareMaze sq{};
  sq.makeMaze(x, y);

  cs225::PNG *png = sq.drawMaze();
  png->writeToFile("my2x2.png");
  delete png;
  std::vector<int> solution = sq.solveMaze();
  // sq.printSolution(solution);
  png = sq.drawMazeWithSolution();
  png->writeToFile("my2x2solved.png");
  delete png;

  return 0;
}

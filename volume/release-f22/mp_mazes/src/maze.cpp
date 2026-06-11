/* Your code here! */
#include "maze.h"
#include "cs225/HSLAPixel.h"
#include <algorithm>
#include <cassert>
#include <queue>
#include <set>

using namespace cs225;

void SquareMaze::printSolution(const std::vector<int> &solution) {
  printf("{ ");
  for (auto move : solution) {
    switch (move) {
    case RIGHT:
      printf("RIGHT, ");
      break;

    case LEFT:
      printf("LEFT, ");
      break;

    case UP:
      printf("UP, ");
      break;

    case DOWN:
      printf("DOWN, ");
      break;
    }
  }
  puts(" }");
}

SquareMaze::SquareMaze() {
  width_ = 0;
  height_ = 0;
}

bool SquareMaze::canTravel(int x, int y, int dir) const {
  // printf("canTravel(%d, %d, %d), width_: %d, height_: %d\n", x, y, dir,
  // width_, height_);
  // if (x >= width_ || y >= height_)
  //   return false;
  //
  int sqr = squares.at(getIndex(x, y));
  // printf("sqr: %d\n", sqr);
  if (dir == RIGHT) {
    if ((x < width_ - 1) && (sqr != RIGHTWALL) && (sqr != BOTHWALLS)) {
      // puts("can go RIGHT");
      return true;
    }
  }
  if (dir == DOWN) {
    if ((y < height_ - 1) && (sqr != DOWNWALL) && (sqr != BOTHWALLS)) {
      // puts("can go DOWN");
      return true;
    }
  }

  if (dir == UP) {
    if ((y != 0) && (squares.at(getIndex(x, y - 1)) != DOWNWALL) &&
        (squares.at(getIndex(x, y - 1)) != BOTHWALLS)) {
      // puts("can go UP");
      return true;
    }
  }

  if (dir == LEFT) {
    if ((x != 0) && (squares.at(getIndex(x - 1, y)) != RIGHTWALL) &&
        (squares.at(getIndex(x - 1, y)) != BOTHWALLS)) {
      // puts("can go DOWN");
      return true;
    }
  }

  return false;
}

void SquareMaze::makeMaze(int width, int height) {
  int unions = 0;
  width_ = width;
  height_ = height;

  squares.resize(width_ * height_, BOTHWALLS);
  sets.addelements(width_ * height_);

  // e1ec30: while everything is not in the same set
  while (sets.size(0) != (width_ * height_)) {
    // e1ec30: This is just Kruskal's algorithm, which I came across here:
    // https://www.math.umd.edu/~immortal/CMSC420/notes/disjointset.pdf e1ec30:
    // randomly pick a square, and randomly try to remove one of it's walls

    int sqr = std::rand() % (width_ * height_);
    int wall = std::rand() % 2; // right or down

    if (wall == 0) { // The right wall
      if ((sqr + 1) % width_ == 0 || sets.find(sqr) == sets.find(sqr + 1))
        continue;
      else {
        setWall(getX(sqr), getY(sqr), 0, false); // remove the right wall
        sets.setunion(sqr, sqr + 1); // since we're sure this won't "overflow"
        unions++;
        assert(canTravel(getX(sqr), getY(sqr), RIGHT));
      }
    } else {
      if ((sqr + width_) >= (width_ * height_) ||
          sets.find(sqr) == sets.find(sqr + width_))
        continue;
      else {
        setWall(getX(sqr), getY(sqr), 1, false);
        sets.setunion(sqr, sqr + width_);
        unions++;
        assert(canTravel(getX(sqr), getY(sqr), DOWN));
      }
    }
  }
  assert(unions = (width_ * height_) - 1);
  // printf("unions: %d\n", unions);
}

void SquareMaze::setWall(int x, int y, int dir, bool exists) {
  int wall = squares.at(getIndex(x, y));

  if (dir == RIGHT && exists) {
    if (wall == DOWNWALL)
      squares.at(getIndex(x, y)) = BOTHWALLS;
    else if (wall == NOWALL)
      squares.at(getIndex(x, y)) = RIGHTWALL;
  }

  if (dir == DOWN && exists) {
    if (wall == NOWALL)
      squares.at(getIndex(x, y)) = DOWNWALL;
    else if (wall == RIGHTWALL)
      squares.at(getIndex(x, y)) = BOTHWALLS;
  }

  if (dir == RIGHT && !exists) {
    if (wall == RIGHTWALL)
      squares.at(getIndex(x, y)) = NOWALL;
    else if (wall == BOTHWALLS)
      squares.at(getIndex(x, y)) = DOWNWALL;
  }

  if (dir == DOWN && !exists) {
    if (wall == DOWNWALL)
      squares.at(getIndex(x, y)) = NOWALL;
    else if (wall == BOTHWALLS)
      squares.at(getIndex(x, y)) = RIGHTWALL;
  }
}

std::vector<int> SquareMaze::solveMaze() {
  std::vector<int> sol;

  std::vector<int> path;
  std::queue<int> q;
  std::vector<int> length(width_ * height_, -1);
  std::vector<int> parent(width_ * height_, -1);
  std::vector<int> direction(width_ * height_, -1);

  q.push(0);

  while (!q.empty()) {
    auto c = q.front();
    if (canTravel(getX(c), getY(c), RIGHT)) {
      // puts("going right");
      if (length[c + 1] < 0) {
        length[c + 1] = length[c] + 1;
        parent[c + 1] = c;
        direction[c + 1] = RIGHT;
        q.push(c + 1);
      }
    }

    if (canTravel(getX(c), getY(c), DOWN)) {
      // puts("going down");
      if (length[c + width_] < 0) {
        length[c + width_] = length[c] + 1;
        parent[c + width_] = c;
        direction[c + width_] = DOWN;
        q.push(c + width_);
      }
    }

    if (canTravel(getX(c), getY(c), LEFT)) {
      // puts("going left");
      if (length[c - 1] < 0) {
        length[c - 1] = length[c] + 1;
        parent[c - 1] = c;
        direction[c - 1] = LEFT;
        q.push(c - 1);
      }
    }

    if (canTravel(getX(c), getY(c), UP)) {
      // puts("going up");
      if (length[c - width_] < 0) {
        length[c - width_] = length[c] + 1;
        parent[c - width_] = c;
        direction[c - width_] = UP;
        q.push(c - width_);
      }
    }
    
    q.pop();
  }

  int longest = -1;
  int longest_length = -1;
  
  // e1ec30: find the square with the longest path (starting from the right end) 
  for (int i = getIndex(0, height_ - 1); i < width_ * height_; i++) {
    if (length[i] > longest_length) {
      longest = i;
      longest_length = length[i];
    }
  }

  // e1ec30: follow the directions from the path to the origin, then reverse it to start from the origin.
  while (longest > 0) {
    path.push_back(direction[longest]);
    longest = parent[longest];
  }
  std::reverse(path.begin(), path.end());
  return path;
}

PNG *SquareMaze::drawMaze() const {
  PNG *png = new PNG(width_ * 10 + 1, height_ * 10 + 1);

  // e1ec30: Blacken entire topmost row, except for the entrance
  for (unsigned i = 0; i < png->width(); i++) {
    if ((i < 1) || (i > 9)) {
      HSLAPixel &p = png->getPixel(i, 0);
      p.l = 0;
    }
  }

  // e1ec30: Blacken the leftmost column
  for (unsigned i = 0; i < png->height(); i++) {
    HSLAPixel &p = png->getPixel(0, i);
    p.l = 0;
  }

  // e1ec30: for each square in the maze, call its coordinates
  for (int x = 0; x < width_; x++) {
    for (int y = 0; y < height_; y++) {
      int sqr = squares.at(getIndex(x, y));
      
      // e1ec30: if the rightwall exists
      if (sqr == RIGHTWALL || sqr == BOTHWALLS) {
        // e1ec30: blacken all pixels with ((x+1)*10, (y*10)+k) for k in [0, 10]
        for (int k = 0; k <= 10; k++) {
          HSLAPixel &p = png->getPixel(((x + 1) * 10), (y * 10) + k);
          p.l = 0;
        }
      }

      // e1ec30: if the downwall exists
      if (sqr == DOWNWALL || sqr == BOTHWALLS) {
        // printf("blackening downwall: (%d, %d)\n", x, y);
        // e1ec30: blacken all pixels with (x*10+k, (y+1)*10) for k from 0 to 10
        for (int k = 0; k <= 10; k++) {
          HSLAPixel &p = png->getPixel((x * 10) + k, (y + 1) * 10);
          p.l = 0;
        }
      }
    }
  }

  return png;
}

PNG *SquareMaze::drawMazeWithSolution() {
  PNG *maze_img = drawMaze();
  std::vector<int> solution = solveMaze();

  int x = 5, y = 5, i=0;

  auto make_red = [&maze_img](int x, int y) {
    HSLAPixel &p = maze_img->getPixel(x, y);
    p.h = 0;
    p.s = 1;
    p.l = 0.5;
    p.a = 1;
  };

  int last_cell = 0;

  for (auto dir : solution) {
    if (dir == RIGHT) {
      for (i = 0; i < 11; i++) {
        make_red(x + i, y);
      }
      last_cell += 1;
      x += 10;
    }
    else if (dir == LEFT) {
      for (i = 0; i < 11; i++) {
        make_red(x - i, y);
      }
      last_cell -= 1;
      x -= 10;
    }
    else if (dir == UP) {
      for (i = 0; i < 11; i++) {
        make_red(x, y - i);
      }
      last_cell -= width_;
      y -= 10;
    }
    else if (dir == DOWN) {
      for (i = 0; i < 11; i++) {
        make_red(x, y + i);
      }
      last_cell += width_;
      y += 10;
    }
  }

  // e1ec30: make the exit by whitening (x*10+k, (y+1)*10) for k from 1 to 9
  // where (x, y) are the coords of the destination maze
  int dx = getX(last_cell);
  int dy = getY(last_cell);

  for (int k = 1; k < 10; k++) {
    HSLAPixel &p = maze_img->getPixel((dx * 10) + k, (dy + 1) * 10);
    p.h = 0;
    p.s = 0;
    p.l = 1;
    p.a = 1;
  }
  return maze_img;
}


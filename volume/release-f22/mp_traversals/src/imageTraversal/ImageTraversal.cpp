#include <cmath>
#include <iterator>
#include <iostream>

#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"
#include "../Point.h"

#include "ImageTraversal.h"

/**
 * Calculates a metric for the difference between two pixels, used to
 * calculate if a pixel is within a tolerance.
 *
 * @param p1 First pixel
 * @param p2 Second pixel
 * @return the difference between two HSLAPixels
 */
double ImageTraversal::calculateDelta(const HSLAPixel & p1, const HSLAPixel & p2) {
  double h = fabs(p1.h - p2.h);
  double s = p1.s - p2.s;
  double l = p1.l - p2.l;

  // Handle the case where we found the bigger angle between two hues:
  if (h > 180) { h = 360 - h; }
  h /= 360;

  return sqrt( (h*h) + (s*s) + (l*l) );
}


double ImageTraversal::calculateDelta(const PNG &png, const Point &p1, const Point &p2)
{
  const HSLAPixel &pixel1 = png.getPixel(p1.x, p1.y);
  const HSLAPixel &pixel2 = png.getPixel(p2.x, p2.y);

  return calculateDelta(pixel1, pixel2);
}

/**
 * Default iterator constructor.
 */
ImageTraversal::Iterator::Iterator(ImageTraversal *traversal, bool empty) {
  /** @todo [Part 1] */
  traversal_ = traversal;
  empty_ = empty;
}

ImageTraversal::Iterator::~Iterator() {
  delete traversal_;
}

/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
  /** @todo [Part 1] */
  if (!traversal_->empty()) {
    Point p = traversal_->peek();
    traversal_->pop();
    visited_.insert(p);

    if(visited_.find(Point(p.x+1, p.y)) == visited_.end()) traversal_->add(Point(p.x+1, p.y));
    if(visited_.find(Point(p.x, p.y+1)) == visited_.end()) traversal_->add(Point(p.x, p.y+1));
    if(visited_.find(Point(p.x-1, p.y)) == visited_.end()) traversal_->add(Point(p.x-1, p.y));
    if(visited_.find(Point(p.x, p.y-1)) == visited_.end()) traversal_->add(Point(p.x, p.y-1));

    if (traversal_->empty()) empty_ = true;
  }
  else {
    empty_ = true;
  }
  return *this;
}

/**
 * Iterator accessor opreator.
 *
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*() {
  /** @todo [Part 1] */
  return traversal_->peek();
}

/**
 * Iterator inequality operator.
 *
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
  /** @todo [Part 1] */
  return this->empty_ != other.empty_;
}


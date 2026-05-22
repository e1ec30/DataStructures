#include <cmath>

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
ImageTraversal::Iterator::Iterator(ImageTraversal *traversal, Point start, bool empty) {
  /** @todo [Part 1] */
  traversal_ = traversal;
  empty_ = empty;
  start_ = start;

  if (!empty) current_ = traversal_->peek();

}


ImageTraversal::Iterator::~Iterator() {
  delete traversal_;
}

bool ImageTraversal::Iterator::isVisited(Point p) {
  return (visited_.find(p) != visited_.end());
}

/**
 * Iterator increment opreator.
 *
 * Advances the traversal of the image.
 */
ImageTraversal::Iterator & ImageTraversal::Iterator::operator++() {
  /** @todo [Part 1] */

  // e1ec30: current_ is always traversal_->peek, so pop it, mark it as visited, pop everything that has been visited, and insert it's neighbours.
  if (!traversal_->empty()) {

    traversal_->pop();
    Point p = current_;

    visited_.insert(p);

    Point p1 = Point(p.x+1, p.y);
    Point p2 = Point(p.x, p.y+1);
    Point p3 = Point(p.x-1, p.y);
    Point p4 = Point(p.x, p.y-1);

    while(!traversal_->empty() && isVisited(traversal_->peek())) traversal_->pop();

    if (!isVisited(p1)) traversal_->add(p1);
    if (!isVisited(p2)) traversal_->add(p2);
    if (!isVisited(p3)) traversal_->add(p3);
    if (!isVisited(p4)) traversal_->add(p4);

    if (!traversal_->empty()) current_ = traversal_->peek();
    else empty_ = true;

 }
  else empty_ = true;
  return *this;
}

/**
 * Iterator accessor opreator.
 *
 * Accesses the current Point in the ImageTraversal.
 */
Point ImageTraversal::Iterator::operator*() {
  /** @todo [Part 1] */
  return Point(current()->x, current()->y);
}

const Point* ImageTraversal::Iterator::current() const{
  if (this->empty_) return NULL;
  return &current_;
}

/**
 * Iterator inequality operator.
 *
 * Determines if two iterators are not equal.
 */
bool ImageTraversal::Iterator::operator!=(const ImageTraversal::Iterator &other) {
  /** @todo [Part 1] */
  return this->current() != other.current();
}


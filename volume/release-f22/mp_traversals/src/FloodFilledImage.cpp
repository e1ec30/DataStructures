#include "cs225/HSLAPixel.h"
#include "cs225/PNG.h"

#include "colorPicker/ColorPicker.h"
#include "imageTraversal/ImageTraversal.h"

#include "Animation.h"
#include <utility>
#include "FloodFilledImage.h"

using namespace cs225;

/**
 * Constructs a new instance of a FloodFilledImage with a image `png`.
 * 
 * @param png The starting image of a FloodFilledImage
 */
FloodFilledImage::FloodFilledImage(const PNG & png): png_(png), operations_() {
  /** @todo [Part 2] */
}

/**
 * Adds a FloodFill operation to the FloodFillImage.  This function must store the operation,
 * which will be used by `animate`.
 * 
 * @param traversal ImageTraversal used for this FloodFill operation.
 * @param colorPicker ColorPicker used for this FloodFill operation.
 */
void FloodFilledImage::addFloodFill(ImageTraversal & traversal, ColorPicker & colorPicker) {
  /** @todo [Part 2] */
  operations_.push_back({&traversal, &colorPicker});
}

/**
 * Creates an Animation of frames from the FloodFill operations added to this object.
 * 
 * Each FloodFill operation added by `addFloodFill` is executed based on the order
 * the operation was added.  This is done by:
 * 1. Visiting pixels within the image based on the order provided by the ImageTraversal iterator and
 * 2. Updating each pixel to a new color based on the ColorPicker
 * 
 * While applying the FloodFill to the image, an Animation is created by saving the image
 * after every `frameInterval` pixels are filled.  To ensure a smooth Animation, the first
 * frame is always the starting image and the final frame is always the finished image.
 * 
 * (For example, if `frameInterval` is `4` the frames are:
 *   - The initial frame
 *   - Then after the 4th pixel has been filled
 *   - Then after the 8th pixel has been filled
 *   - ...
 *   - The final frame, after all pixels have been filed)
 */ 
Animation FloodFilledImage::animate(unsigned frameInterval) {
  Animation animation;

  for (auto op: operations_) {
    ImageTraversal *trav = op.first;
    ColorPicker *picker = op.second;

    //e1ec30: all I need now is how to edit my pixels

    int i = 1;
    animation.addFrame(png_);
    for (auto it = trav->begin(); it != trav->end(); ++it, ++i) {
      HSLAPixel pixel = picker->getColor((*it).x, (*it).y);
      png_.getPixel((*it).x, (*it).y).h = pixel.h;
      png_.getPixel((*it).x, (*it).y).s = pixel.s;
      png_.getPixel((*it).x, (*it).y).l = pixel.l;
      png_.getPixel((*it).x, (*it).y).a = pixel.a;

      if (i % frameInterval == 0) animation.addFrame(png_);
    }
    animation.addFrame(png_);
  }
  
  /** @todo [Part 2] */
  return animation;
}

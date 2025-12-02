#include "Image.h"
#include "StickerSheet.h"

int main() {
  //
  // Reminder:
  //   Before exiting main, save your creation to disk as myImage.png
  //
  Image a;
  a.readFromFile("../alma.png");
  StickerSheet s(a, 20);
  s.addSticker(a, 2, 2);
  Image f = s.render();
  f.writeToFile("../myImage.png");

  return 0;
}

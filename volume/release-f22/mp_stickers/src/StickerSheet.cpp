#include <StickerSheet.h>

StickerSheet::StickerSheet(const Image &picture, unsigned max) : sheet(max), base(picture) {} 

void StickerSheet::changeMaxStickers(unsigned max) {
	sheet.resize(max);
}

int StickerSheet::addSticker(Image &sticker, unsigned x, unsigned y) {
	if (sheet.size() == sheet.capacity()) return -1;

	Sticker *s = new Sticker {
		&sticker,
		x,
		y
	};

	sheet.push_back(std::move(*s));
	delete s;
	return sheet.size();
}

bool StickerSheet::translate(unsigned index, unsigned x, unsigned y) {

	try {
		auto &s = sheet.at(index);
		s.x = x;
		s.y = y;
		return true;
	}

	catch(const std::out_of_range &e) {
		return false;
	}

}

void StickerSheet::removeSticker(unsigned index) {
	if (index < sheet.size()) {
		sheet.erase(sheet.begin() + index);
	}
}

Image* StickerSheet::getSticker(unsigned index) {
	if (index < sheet.size()) {
		auto &s = sheet.at(index);
		return s.sticker;
	}
	return NULL;
}

StickerSheet::StickerSheet(const StickerSheet &other) : sheet(other.sheet), base(other.base) {}

const StickerSheet& StickerSheet::operator=(const StickerSheet &other) {
	sheet = other.sheet;
	base = other.base;
	return *this;
}

Image StickerSheet::render() const {
	// First, an image as big as the base Image
	Image ret(base);

	// Next, go through each sticker and copy it on the base if it will fit, otherwise resize the base

	return ret;
}

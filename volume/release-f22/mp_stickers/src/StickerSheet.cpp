#include <StickerSheet.h>

StickerSheet::StickerSheet(const Image &picture, unsigned m) : base(picture), max(m) {} 

void StickerSheet::changeMaxStickers(unsigned max) {
	if (sheet.size() > max) sheet.resize(max);
	else (sheet.reserve(max));
	this->max = max;
}

int StickerSheet::addSticker(Image &sticker, unsigned x, unsigned y) {
	if (sheet.size() == max) return -1;

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

void copy_onto(Image *base, Image *sticker, unsigned x, unsigned y) {
	unsigned new_width = base->width(), new_height = base->height();
	if (base->width() < (sticker->width() + x)) {
		new_width = (x + sticker->width());
	}

	if (base->height() < (sticker->height() + y)) {
		new_height = (y + sticker->height());
	}

	if ((new_width != base->width()) || (new_height != base->height())) {
		base->resize(new_width, new_height);
	}

	for (unsigned xs = 0; xs < sticker->width(); xs++) {
		for (unsigned ys = 0; ys < sticker->height(); ys++) {
			auto &src_p = sticker->getPixel(xs, ys);
			auto &dst_p = base->getPixel(x + xs, y + ys);

			if (src_p.a != 0) {
				dst_p.h = src_p.h;
				dst_p.s = src_p.s;
				dst_p.l = src_p.l;
				dst_p.a = src_p.a;
			}
		}
	}

}
Image StickerSheet::render() const {
	// First, an image as big as the base Image
	Image ret(base);

	// Next, go through each sticker and copy it on the base if it will fit, otherwise resize the base first
	for (auto s : sheet) {
		copy_onto(&ret, s.sticker, s.x, s.y);
	}

	return ret;
}

#include "Image.h"
#include "cs225/HSLAPixel.h"
#include <algorithm>
#include <cmath>

void Image::lighten(double amount) {
	for (unsigned x = 0; x < width(); x++) {
		for (unsigned y = 0; y < height(); y++) {
			auto &p = getPixel(x, y);
			p.l = std::clamp(p.l+amount, 0.0, 1.0);
		}
	}
}

void Image::saturate(double amount) {
	for (unsigned x = 0; x < width(); x++) {
		for (unsigned y = 0; y < height(); y++) {
			auto &p = getPixel(x, y);
			p.s = std::clamp(p.s+amount, 0.0, 1.0);
		}
	}
}

void Image::scale(double factor) {
	if (factor == 1.0) return;
	unsigned new_w = std::round(width()*factor);
	unsigned new_h = std::round(height()*factor);

	Image old_img(*this);
	resize(new_w, new_h);

	for (unsigned x = 0; x < new_w; x++) {
		for (unsigned y = 0; y < new_h; y++) {
			double fsrc_x = x / factor;
			double fsrc_y = y / factor;

			unsigned src_x = std::round(fsrc_x);
			unsigned src_y = std::round(fsrc_y);

			src_x = std::clamp(src_x, (unsigned)0, old_img.width() - 1);
			src_y = std::clamp(src_y, (unsigned)0, old_img.height() - 1);

			auto &new_p = getPixel(x, y);
			auto &old_p = old_img.getPixel(src_x, src_y);

			new_p.h = old_p.h;
			new_p.l = old_p.l;
			new_p.s = old_p.s;
			new_p.a = old_p.a;
		}
	}
}

/**
 * @file Image.h
 * Contains your declaration of the interface for the Image class.
 */

#pragma once
#include "../lib/cs225/PNG.h"

class Image: public cs225::PNG {
public:
	Image(): cs225::PNG() {};
	Image(unsigned w, unsigned h) : cs225::PNG(w, h) {};
	Image(const Image& other) : cs225::PNG(other) {};
	void lighten();
	void lighten(double amount);

	void darken();
	void darken(double amount);

	void saturate();
	void saturate(double amount);

	void desaturate();
	void desaturate(double amount);

	void grayscale();
	void rotateColor(double degrees);

	void illinify();
	void scale(double factor);
	void scale(unsigned w, unsigned h);
};

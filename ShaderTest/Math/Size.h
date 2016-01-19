#ifndef __SIZE__
#define __SIZE__
#include "Point.h"

struct Size{
	float width, height;

	Size();
	Size(float width, float height);
	Size(float _size[2]);

	const Size& operator=(const Size& _size);
	bool operator==(const Size& _size) const;

	const Size& operator+=(const Size& _size);
	const Size& operator-=(const Size& _size);

	Size operator+(const Size& _size) const;
	Size operator-(const Size& _size) const;
};
#endif
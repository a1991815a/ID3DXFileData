#ifndef __RECT2D__
#define __RECT2D__
#include "Point.h"
#include "Size.h"

struct Rect2D{
	union
	{
		struct {
			float x, y;
		};
		float position[2];
	};
	union
	{
		struct {
			float width, height;
		};
		float size[2];
	};

	Rect2D();
	Rect2D(float x, float y, float width, float height);
	Rect2D(const Point& point, const Size& _size);
	Rect2D(float point[2], float _size[2]);

	void SetPoint(const Point& point);
	void SetSize(const Size& _size);
	bool containsPoint(float x, float y) const;
	bool containsPoint(const Point& point) const;
};
#endif
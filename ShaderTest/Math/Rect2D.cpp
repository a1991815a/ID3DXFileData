#include "Rect2D.h"


Rect2D::Rect2D()
	:x(0), y(0), width(0), height(0)
{

}

Rect2D::Rect2D(float x, float y, float width, float height)
	: x(x), y(y), width(width), height(height)
{

}

Rect2D::Rect2D(const Point& point, const Size& _size)
	: x(point.x), y(point.y), width(_size.width), height(_size.height)
{

}

Rect2D::Rect2D(float point[2], float _size[2])
	: x(point[0]), y(point[1]), width(_size[0]), height(_size[1])
{

}

void Rect2D::SetPoint(const Point& point)
{
	x = point.x;
	y = point.y;
}

void Rect2D::SetSize(const Size& _size)
{
	width = _size.width;
	height = _size.height;
}

bool Rect2D::containsPoint(float x, float y) const
{
	return x >= this->x && x <= this->x + this->width &&
		y >= this->y && y <= this->y + this->height;
}

bool Rect2D::containsPoint(const Point& point) const
{
	return containsPoint(point.x, point.y);
}

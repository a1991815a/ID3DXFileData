#ifndef __POINT__
#define __POINT__

struct Point{
	float x, y;
	
	Point() 
		:x(0), y(0)
	{};
	Point(float x, float y)
		:x(x), y(y)
	{};
	Point(float pos[2])
		:x(pos[0]), y(pos[1])
	{};

	const Point& operator=(const Point& _Point) {
		x = _Point.x;
		y = _Point.y;
		return *this;
	};
	bool operator==(const Point& _Point) const {
		return x == _Point.x && y == _Point.y;
	};
	bool operator!=(const Point& _Point) const {
		return !(*this == _Point);
	}

	const Point& operator+=(const Point& _Point) {
		x += _Point.x;
		y += _Point.y;
		return *this;
	};
	const Point& operator-=(const Point& _Point) {
		x -= _Point.x;
		y -= _Point.y;
		return *this;
	};

	Point operator+(const Point& _Point) const {
		Point point;
		point += _Point;
		return point;
	};
	Point operator-(const Point& _Point) const {
		Point point;
		point -= _Point;
		return point;
	};
};
#endif
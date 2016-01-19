#include "Size.h"

Size::Size()
	:width(0), height(0)
{

}

Size::Size(float width, float height)
	:width(width), height(height)
{

}

Size::Size(float _size[2])
	:width(_size[0]), height(_size[1])
{

}

const Size& Size::operator-=(const Size& _size)
{
	width -= _size.width;
	height -= _size.height;
	return *this;
}

Size Size::operator-(const Size& _size) const
{
	Size ret = *this;
	ret -= _size;
	return ret;
}

const Size& Size::operator+=(const Size& _size)
{
	width += _size.width;
	height += _size.height;
	return *this;
}

Size Size::operator+(const Size& _size) const
{
	Size ret = *this;
	ret += _size;
	return ret;
}

bool Size::operator==(const Size& _size) const
{
	return width == _size.width && height == _size.height;
}

const Size& Size::operator=(const Size& _size)
{
	width = _size.width;
	height = _size.height;
	return *this;
}

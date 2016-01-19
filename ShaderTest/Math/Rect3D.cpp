#include "Rect3D.h"

Rect3D::Rect3D()
	:x(0), y(0), z(0), width(0), height(0), depth(0)
{

}

Rect3D::Rect3D(float x, float y, float z)
	: x(x), y(y), z(z), width(0), height(0), depth(0)
{

}

Rect3D::Rect3D(float x, float y, float z, float width, float height, float depth)
	: x(x), y(y), z(z), width(width), height(height), depth(depth)
{

}

bool Rect3D::containsPoint(const D3DXVECTOR3& pos)
{
	return pos.x >= x && pos.x <= x + width &&
		pos.y >= y && pos.y <= y + height &&
		pos.z >= z && pos.z <= z + depth;
}

bool Rect3D::operator!=(const Rect3D& _rect) const
{
	return !(*this == _rect);
}

bool Rect3D::operator==(const Rect3D& _rect) const
{
	return x == _rect.x && y == _rect.y && z == _rect.z &&
		width == _rect.width && height == _rect.height && depth == _rect.depth;
}

const Rect3D& Rect3D::operator=(const Rect3D& _rect)
{
	x = _rect.x;
	y = _rect.y;
	z = _rect.z;
	width = _rect.width;
	height = _rect.height;
	depth = _rect.depth;

	return *this;
}

D3DXVECTOR3 Rect3D::NearBottomLeft()
{
	return D3DXVECTOR3(x, y, z);
}

D3DXVECTOR3 Rect3D::NearBottomRight()
{
	return D3DXVECTOR3(x + width, y, z);
}

D3DXVECTOR3 Rect3D::NearTopLeft()
{
	return D3DXVECTOR3(x, y + height, z);
}

D3DXVECTOR3 Rect3D::NearTopRight()
{
	return D3DXVECTOR3(x + width, y + height, z);
}

D3DXVECTOR3 Rect3D::FarBottomLeft()
{
	return D3DXVECTOR3(x, y, z + depth);
}

D3DXVECTOR3 Rect3D::FarBottomRight()
{
	return D3DXVECTOR3(x + width, y, z + depth);
}

D3DXVECTOR3 Rect3D::FarTopLeft()
{
	return D3DXVECTOR3(x, y + height, z + depth);
}

D3DXVECTOR3 Rect3D::FarTopRight()
{
	return D3DXVECTOR3(x + width, y + height, z + depth);
}

D3DXVECTOR3 Rect3D::MinPoint()
{
	return D3DXVECTOR3(x, y, z);
}

D3DXVECTOR3 Rect3D::MaxPoint()
{
	return D3DXVECTOR3(x + width, y + height, z + depth);
}

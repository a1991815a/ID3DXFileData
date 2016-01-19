#ifndef __RECT3D__
#define __RECT3D__
#include <d3d9.h>
#include <d3dx9.h>

struct Rect3D{
	float x, y, z;
	float width, height, depth;
	Rect3D();
	Rect3D(float x, float y, float z);
	Rect3D(float x, float y, float z, float width, float height, float depth);

	const Rect3D& operator=(const Rect3D& _rect);
	bool operator==(const Rect3D& _rect) const;
	bool operator!=(const Rect3D& _rect) const;

	bool containsPoint(const D3DXVECTOR3& pos);

	D3DXVECTOR3 NearBottomLeft();
	D3DXVECTOR3 NearBottomRight();
	D3DXVECTOR3 NearTopLeft();
	D3DXVECTOR3 NearTopRight();
	D3DXVECTOR3 FarBottomLeft();
	D3DXVECTOR3 FarBottomRight();
	D3DXVECTOR3 FarTopLeft();
	D3DXVECTOR3 FarTopRight();

	D3DXVECTOR3 MinPoint();
	D3DXVECTOR3 MaxPoint();
};
#endif
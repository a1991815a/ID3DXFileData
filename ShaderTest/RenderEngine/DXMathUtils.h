#ifndef __DXMATHUTILS__
#define __DXMATHUTILS__
#include <d3d9.h>
#include <d3dx9.h>

void		dxMatrixLookAtLHInverse(
							D3DXMATRIX* pOut, 
							const D3DXVECTOR3* pos,
							const D3DXVECTOR3* dstPos, 
							const D3DXVECTOR3* up);

float		dxVec3Angle(const D3DXVECTOR3* v1, const D3DXVECTOR3* v2);
float		dxVec3AngleSmall(const D3DXVECTOR3* v1, const D3DXVECTOR3* v2);
float		dxVec3AngleLarge(const D3DXVECTOR3* v1, const D3DXVECTOR3* v2);

#endif
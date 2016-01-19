#include "DXMathUtils.h"



void dxMatrixLookAtLHInverse(
	D3DXMATRIX* pOut, 
	const D3DXVECTOR3* pos, 
	const D3DXVECTOR3* dstPos, 
	const D3DXVECTOR3* up)
{
	D3DXMatrixIdentity(pOut);
	D3DXVECTOR3 asixX(0, 0, 0);
	D3DXVECTOR3 asixY(*up);
	D3DXVECTOR3 asixZ(*dstPos - *pos);

	D3DXVec3Normalize(&asixZ, &asixZ);
	
	D3DXVec3Cross(&asixX, &asixY, &asixZ);
	D3DXVec3Normalize(&asixX, &asixX);
	D3DXVec3Cross(&asixY, &asixZ, &asixX);

	*(D3DXVECTOR3*)pOut->m[0] = asixX;
	*(D3DXVECTOR3*)pOut->m[1] = asixY;
	*(D3DXVECTOR3*)pOut->m[2] = asixZ;
	*(D3DXVECTOR3*)pOut->m[3] = *pos;
}

float dxVec3Angle(const D3DXVECTOR3* v1, const D3DXVECTOR3* v2)
{
	D3DXVECTOR3 vv1, vv2;
	D3DXVec3Normalize(&vv1, v1);
	D3DXVec3Normalize(&vv2, v2);

	float dot_val = D3DXVec3Dot(&vv1, &vv2);
	return acosf(dot_val);
}

float dxVec3AngleSmall(const D3DXVECTOR3* v1, const D3DXVECTOR3* v2)
{
	float angle = dxVec3Angle(v1, v2);
	if(angle > D3DX_PI)
		angle = 2 * D3DX_PI - angle;
	return angle;
}

float dxVec3AngleLarge(const D3DXVECTOR3* v1, const D3DXVECTOR3* v2)
{
	float angle = dxVec3Angle(v1, v2);
	if(angle < D3DX_PI)
		angle = 2 * D3DX_PI - angle;
	return angle;
}

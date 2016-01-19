#ifndef __DXUTILS__
#define __DXUTILS__
#include "DXMathUtils.h"


//�õ��豸
IDirect3DDevice9*	dxGetDevice();

//�õ���λ����
const D3DXMATRIX*	dxGetUnitMatrix();

//�õ�Ĭ�ϲ���
const D3DMATERIAL9& dxGetDefaultMaterial();
//�޸�Ĭ�ϲ���
void				dxSetDefaultMaterial(
						D3DXCOLOR	Diffuse,
						D3DXCOLOR	Ambient,
						D3DXCOLOR	Specular,
						D3DXCOLOR	Emissive,
						float		Power
					);

//��ʼ���� ��ͼͶӰ��������
bool				dxInit(HWND hwnd);
void				dxSetWorld(const D3DXMATRIX* matrix);
void				dxSetView(const D3DXMATRIX* matrix);
void				dxSetProjection(const D3DXMATRIX* matrix);
void				dxSetView(float x, float y, float z,
							float dx, float dy, float dz,
							float upx, float upy, float upz);
void				dxSetProjection(float radians, float aspect, float _near, float _far);
void				dxEnableDepthBuffer(bool onoff);
void				dxSetFillMode(D3DFILLMODE mode);
void				dxSetCullMode(D3DCULL mode);
void				dxSetRenderState(D3DRENDERSTATETYPE State, DWORD Value);

//���ƺ���
bool				dxBeginScene();
bool				dxEndScene();

//���������������
void				dxSetLight(DWORD index, const D3DLIGHT9* light);
void				dxLightEnable(DWORD index, bool onoff);
void				dxSetMaterial(const D3DMATERIAL9* material);
void				dxSetTexture(DWORD index, IDirect3DTexture9* texutre);

//��ɫ��Alpha������ú���
void				dxSetTextureColorBlend(DWORD index, DWORD op, DWORD arg1, DWORD arg2);
void				dxSetTextureAlphaBlend(DWORD index, DWORD op, DWORD arg1, DWORD arg2);
void				dxSetTextureBlend(DWORD index, DWORD op, DWORD arg1, DWORD arg2);
void				dxSetBlend(D3DBLEND srcblend, D3DBLEND dstblend);
void				dxSetBlendOp(D3DBLENDOP op);
void				dxSetBlendAlpha(D3DBLEND srcblend, D3DBLEND dstblend);
void				dxSetBlendAlphaOp(D3DBLENDOP op);

void				dxGetWorldMatrix(D3DXMATRIX* matrix);
void				dxGetViewMatrix(D3DXMATRIX* matrix);
void				dxGetProjMatrix(D3DXMATRIX* matrix);

//�������
void				dxCopyVertex(
						void* destVertex, void* resVertex, 
						size_t stride, size_t copy_count,
						const D3DXMATRIX* matrix
					);

#endif
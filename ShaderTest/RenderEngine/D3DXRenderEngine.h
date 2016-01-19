#ifndef __D3DXRENDERENGINE__
#define __D3DXRENDERENGINE__
#include "../GameHeader.h"
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define _dxRenderEngine	D3DXRenderEngine::getInstance()

class D3DXRenderEngine{
public:
	static D3DXRenderEngine* getInstance();
	static void destroyInstance();
private:
	static D3DXRenderEngine* m_instance;
public:
	D3DXRenderEngine();
	~D3DXRenderEngine();

	friend bool					dxInit(HWND hwnd);
	friend IDirect3DDevice9*	dxGetDevice();
	friend void					dxSetWorld(const D3DXMATRIX* matrix);
	friend void					dxSetView(const D3DXMATRIX* matrix);
	friend void					dxSetProjection(const D3DXMATRIX* matrix);
	friend void					dxEnableDepthBuffer(bool onoff);
	friend void					dxSetFillMode(D3DFILLMODE mode);
	friend void					dxSetCullMode(D3DCULL mode);
	friend void					dxSetRenderState(D3DRENDERSTATETYPE State, DWORD Value);

	friend bool					dxBeginScene();
	friend bool					dxEndScene();
	friend void					dxSetLight(DWORD index, const D3DLIGHT9* light);
	friend void					dxLightEnable(DWORD index, bool onoff);
	friend void					dxSetMaterial(const D3DMATERIAL9* material);
	friend void					dxSetTexture(DWORD index, IDirect3DTexture9* texutre);
	friend void					dxSetTextureColorBlend(DWORD index, DWORD op, DWORD arg1, DWORD arg2);
	friend void					dxSetTextureAlphaBlend(DWORD index, DWORD op, DWORD arg1, DWORD arg2);
	friend void					dxSetTextureBlend(DWORD index, DWORD op, DWORD arg1, DWORD arg2);
	friend void					dxSetBlend(D3DBLEND srcblend, D3DBLEND dstblend);
	friend void					dxSetBlendOp(D3DBLENDOP op);
	friend void					dxSetBlendAlpha(D3DBLEND srcblend, D3DBLEND dstblend);
	friend void					dxSetBlendAlphaOp(D3DBLENDOP op);

private:
	IDirect3D9*				m_pD3pp;
	IDirect3DDevice9*		m_pDevice;
};
#endif
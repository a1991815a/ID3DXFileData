#include "DXUtils.h"
#include "D3DXRenderEngine.h"

#pragma warning(disable:4244)

#define MOVE_BYTES(p, bytes) \
p = (decltype(p))((char*)p + bytes)

static const D3DXMATRIX s_mUnitMatrix(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
	);

static D3DMATERIAL9 s_DefaultMaterial;

bool dxInit(HWND hwnd)
{
	//默认材质初始化
	{
		s_DefaultMaterial.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 0.7f);
		s_DefaultMaterial.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
		s_DefaultMaterial.Emissive = D3DXCOLOR(0, 0, 0, 0);
		s_DefaultMaterial.Specular = D3DXCOLOR(0, 0, 0, 0);
		s_DefaultMaterial.Power = 0;
	}

	_dxRenderEngine->m_pD3pp = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS parameters = { 0 };
	parameters.BackBufferCount = 1;
	parameters.BackBufferFormat = D3DFMT_X8R8G8B8;
	parameters.EnableAutoDepthStencil = TRUE;
	parameters.AutoDepthStencilFormat = D3DFMT_D24S8;
	parameters.hDeviceWindow = hwnd;
	parameters.Windowed = TRUE;
	parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	parameters.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;


	HRESULT result = _dxRenderEngine->m_pD3pp->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		nullptr,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&parameters,
		&_dxRenderEngine->m_pDevice
		);
	if (FAILED(result))
		return false;

	//打开深度缓冲和光影
	_dxRenderEngine->m_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	_dxRenderEngine->m_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	RECT rect;
	GetClientRect(hwnd, &rect);
	float width = rect.right - rect.left;
	float height = rect.bottom - rect.top;

	//设置视图和投影矩阵
	dxSetView(0, 0, -10, 0, 0, 0, 0, 1, 0);
	dxSetProjection(D3DX_PI / 4, width / height, 1, 500);

	return SUCCEEDED(result);
}

IDirect3DDevice9* dxGetDevice()
{
	return _dxRenderEngine->m_pDevice;
}

void dxSetView(const D3DXMATRIX* matrix)
{
	_dxRenderEngine->m_pDevice->SetTransform(D3DTS_VIEW, matrix);
}

void dxSetView(
	float x, float y, float z,
	float dx, float dy, float dz, 
	float upx, float upy, float upz)
{
	D3DXMATRIX matrix;
	D3DXMatrixLookAtLH(&matrix, 
		&D3DXVECTOR3(x, y, z), 
		&D3DXVECTOR3(dx, dy, dz), 
		&D3DXVECTOR3(upx, upy, upz));
	dxSetView(&matrix);
}

void dxSetProjection(const D3DXMATRIX* matrix)
{
	_dxRenderEngine->m_pDevice->SetTransform(D3DTS_PROJECTION, matrix);
}

void dxSetProjection(float radians, float aspect, float _near, float _far)
{
	D3DXMATRIX matrix;
	D3DXMatrixPerspectiveFovLH(&matrix, radians, aspect, _near, _far);
	dxSetProjection(&matrix);
}

bool dxBeginScene()
{
	HRESULT result = _dxRenderEngine->m_pDevice->Clear(
		0, nullptr, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
	if (FAILED(result))
		return false;
	result = _dxRenderEngine->m_pDevice->BeginScene();
	return SUCCEEDED(result);
}

bool dxEndScene()
{
	HRESULT result = _dxRenderEngine->m_pDevice->EndScene();
	if (FAILED(result))
		return false;
	result = _dxRenderEngine->m_pDevice->Present(0, 0, 0, 0);
	return SUCCEEDED(result);
}

void dxSetLight(DWORD index, const D3DLIGHT9* light)
{
	_dxRenderEngine->m_pDevice->SetLight(index, light);
}

void dxLightEnable(DWORD index, bool onoff)
{
	_dxRenderEngine->m_pDevice->LightEnable(index, onoff);
}

void dxSetMaterial(const D3DMATERIAL9* material)
{
	_dxRenderEngine->m_pDevice->SetMaterial(material);
}

void dxSetTexture(DWORD index, IDirect3DTexture9* texutre)
{
	_dxRenderEngine->m_pDevice->SetTexture(index, texutre);
}

void dxSetTextureColorBlend(DWORD index, DWORD op, DWORD arg1, DWORD arg2)
{
	_dxRenderEngine->m_pDevice->SetTextureStageState(index, D3DTSS_COLOROP, op);
	_dxRenderEngine->m_pDevice->SetTextureStageState(index, D3DTSS_COLORARG1, arg1);
	_dxRenderEngine->m_pDevice->SetTextureStageState(index, D3DTSS_COLORARG2, arg2);
}

void dxSetTextureAlphaBlend(DWORD index, DWORD op, DWORD arg1, DWORD arg2)
{
	_dxRenderEngine->m_pDevice->SetTextureStageState(index, D3DTSS_ALPHAOP, op);
	_dxRenderEngine->m_pDevice->SetTextureStageState(index, D3DTSS_ALPHAARG1, arg1);
	_dxRenderEngine->m_pDevice->SetTextureStageState(index, D3DTSS_ALPHAARG2, arg2);
}

void dxSetTextureBlend(DWORD index, DWORD op, DWORD arg1, DWORD arg2)
{
	dxSetTextureColorBlend(index, op, arg1, arg2);
	dxSetTextureAlphaBlend(index, op, arg1, arg2);
}

void dxSetBlend(D3DBLEND srcblend, D3DBLEND dstblend)
{
	_dxRenderEngine->m_pDevice->SetRenderState(D3DRS_SRCBLEND, srcblend);
	_dxRenderEngine->m_pDevice->SetRenderState(D3DRS_DESTBLEND, dstblend);
}

void dxSetBlendOp(D3DBLENDOP op)
{
	_dxRenderEngine->m_pDevice->SetRenderState(D3DRS_BLENDOP, op);
}

void dxSetBlendAlpha(D3DBLEND srcblend, D3DBLEND dstblend)
{
	_dxRenderEngine->m_pDevice->SetRenderState(D3DRS_SRCBLENDALPHA, srcblend);
	_dxRenderEngine->m_pDevice->SetRenderState(D3DRS_DESTBLENDALPHA, dstblend);
}

void dxSetBlendAlphaOp(D3DBLENDOP op)
{
	_dxRenderEngine->m_pDevice->SetRenderState(D3DRS_BLENDOPALPHA, op);
}

void dxEnableDepthBuffer(bool onoff)
{
	_dxRenderEngine->m_pDevice->SetRenderState(D3DRS_ZENABLE, onoff);
}

void dxSetFillMode(D3DFILLMODE mode)
{
	_dxRenderEngine->m_pDevice->SetRenderState(D3DRS_FILLMODE, mode);
}

void dxSetCullMode(D3DCULL mode)
{
	_dxRenderEngine->m_pDevice->SetRenderState(D3DRS_CULLMODE, mode);
}

void dxGetWorldMatrix(D3DXMATRIX* matrix)
{
	dxGetDevice()->GetTransform(D3DTS_WORLD, matrix);
}

void dxGetViewMatrix(D3DXMATRIX* matrix)
{
	dxGetDevice()->GetTransform(D3DTS_VIEW, matrix);
}

void dxGetProjMatrix(D3DXMATRIX* matrix)
{
	dxGetDevice()->GetTransform(D3DTS_PROJECTION, matrix);
}

const D3DXMATRIX* dxGetUnitMatrix()
{
	return &s_mUnitMatrix;
}

void dxSetWorld(const D3DXMATRIX* matrix)
{
	_dxRenderEngine->m_pDevice->SetTransform(D3DTS_WORLD, matrix);
}

void dxSetRenderState( D3DRENDERSTATETYPE State, DWORD Value )
{
	_dxRenderEngine->m_pDevice->SetRenderState(State, Value);
}

void dxCopyVertex( void* destVertex, void* resVertex, size_t stride, size_t copy_count, const D3DXMATRIX* matrix )
{
	D3DXVECTOR3* resPos = (D3DXVECTOR3*) resVertex;
	D3DXVECTOR3* dstPos = (D3DXVECTOR3*) destVertex;

	for (size_t i = 0; i < copy_count; ++i)
	{
		memcpy_s(dstPos, stride, resPos, stride);
		D3DXVec3TransformCoord(dstPos, dstPos, matrix);
		MOVE_BYTES(resPos, stride);
		MOVE_BYTES(dstPos, stride);
	}
}

const D3DMATERIAL9& dxGetDefaultMaterial()
{
	return s_DefaultMaterial;
}

void dxSetDefaultMaterial(
	D3DXCOLOR Diffuse, 
	D3DXCOLOR Ambient,
	D3DXCOLOR Specular, 
	D3DXCOLOR Emissive, 
	float Power)
{
	s_DefaultMaterial.Diffuse = Diffuse;
	s_DefaultMaterial.Ambient = Ambient;
	s_DefaultMaterial.Specular = Specular;
	s_DefaultMaterial.Emissive = Emissive;
	s_DefaultMaterial.Power = Power;
}

#pragma warning(default:4244)
#include "D3DXRenderEngine.h"

D3DXRenderEngine* D3DXRenderEngine::m_instance = nullptr;

D3DXRenderEngine* D3DXRenderEngine::getInstance()
{
	if (m_instance == nullptr)
		m_instance = new D3DXRenderEngine();
	return m_instance;
}

void D3DXRenderEngine::destroyInstance()
{
	SAFE_DELETE(m_instance);
}

D3DXRenderEngine::D3DXRenderEngine()
	:m_pD3pp(nullptr), m_pDevice(nullptr)
{

}

D3DXRenderEngine::~D3DXRenderEngine()
{
	SAFE_RELEASE_COM(m_pDevice);
	SAFE_RELEASE_COM(m_pD3pp);
}

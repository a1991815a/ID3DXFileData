#include "GameHeader.h"
#include "RenderEngine\DXUtils.h"
#include <iostream>
#include <stdexcept>
using namespace std;
#include "XFileUtils.h"
#include "XFileStruct.h"
#include <fstream>

D3DXMATRIX mMatrix = *dxGetUnitMatrix();
D3DXMATRIX vMatrix = *dxGetUnitMatrix();
D3DXMATRIX pMatrix = *dxGetUnitMatrix();
D3DVIEWPORT9 gViewport = { 0 };

ID3DXEffect*		pEffect = nullptr;
ID3DXMesh*			pMesh = nullptr;

DWORD		numAttribute = 0;

GSkinInfo*	pSkininfo = nullptr;


D3DXVECTOR3* Normals = nullptr;
ID3DXEffect*  pDrawEffect = nullptr;
IDirect3DTexture9** pTextureList = nullptr;


GAnimationController* pController = nullptr;

bool WinCreate(HWND hwnd)
{
	//初始化DX9
	dxInit(hwnd);
	auto device = dxGetDevice();
	device->GetViewport(&gViewport);

	//矩阵初始化
	{
		D3DXMatrixLookAtLH(
			&vMatrix,
			&D3DXVECTOR3(0, 0 , -20),
			&D3DXVECTOR3(0, 0, 0),
			&D3DXVECTOR3(0, 1, 0)
			);
		D3DXMatrixPerspectiveFovLH(
			&pMatrix,
			D3DX_PI / 4,
			(float)gViewport.Width / gViewport.Height,
			1,
			500
			);
	}


	D3DXCreateEffectFromFileA(
		device,
		"Normal.fx",
		nullptr, nullptr, D3DXSHADER_DEBUG, nullptr, &pEffect, nullptr);

	DWORD time = GetTickCount();
	ID3DXFileEnumObject* pObj = nullptr;
	_XFIleUtils->LoadXFile("role.x", &pObj);

	ID3DXBuffer* pBuf = nullptr;
	GLoadCallback call;
	call.RegisterCall(_XTemplate("Mesh"), [&](
		const GUID& guid, ID3DXFileData* pFileData)
	{
		static int i = 0;
		if(i != 0)
			return;
		i++;
		_XFIleUtils->LoadMesh(pFileData, device, &pMesh, &pBuf, &numAttribute, &pSkininfo);


	});

	DWORD timee = GetTickCount();

	_XFIleUtils->LoadChild(call, pObj);

	timee = GetTickCount() - timee;
	cout << (float)timee / 1000.0f << endl;


	pTextureList = new IDirect3DTexture9*[numAttribute];
	memset(pTextureList, 0, sizeof(IDirect3DTexture9*) * numAttribute);
	D3DXMATERIAL* materialList = (D3DXMATERIAL*)pBuf->GetBufferPointer();
	//打印材质信息
	for (size_t i = 0; i < numAttribute; ++i)
	{
		D3DMATERIAL9& mm = materialList[i].MatD3D;
		printf("Diffuse: %f, %f, %f, %f \n"
			"Ambient: %f, %f, %f, %f \n"
			"Specular: %f, %f, %f, %f \n"
			"Emissive: %f, %f, %f, %f \n"
			"Power: %f\n"
			"TextureFilename: %s\n",
			mm.Diffuse.r, mm.Diffuse.g, mm.Diffuse.b, mm.Diffuse.a,
			mm.Ambient.r, mm.Ambient.g, mm.Ambient.b, mm.Ambient.a,
			mm.Specular.r, mm.Specular.g, mm.Specular.b, mm.Specular.a,
			mm.Emissive.r, mm.Emissive.g, mm.Emissive.b, mm.Emissive.a,
			mm.Power,
			materialList[i].pTextureFilename
			);
		if(materialList[i].pTextureFilename)
			D3DXCreateTextureFromFileA(device,
				materialList[i].pTextureFilename, &pTextureList[i]);
	}

	{
		Normals = new D3DXVECTOR3[pMesh->GetNumVertices() * 2];
		memset(Normals, 0, pMesh->GetNumVertices() * 2 * sizeof(D3DXVECTOR3));
		XVertex_p3_n3_t1* vertexes = nullptr;
		pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&vertexes);
		size_t array_index = 0;
		for (size_t i = 0; i < pMesh->GetNumVertices(); ++i)
		{
			Normals[array_index++] = vertexes[i].getPosition();
			Normals[array_index++] =
				D3DXVECTOR3(vertexes[i].getPosition() + vertexes[i].getNormal() / 7);
		}
		pMesh->UnlockVertexBuffer();
		D3DXCreateEffectFromFileA(device, "DrawNormal.fx",
			nullptr, nullptr, D3DXSHADER_DEBUG, nullptr,
			&pDrawEffect, nullptr);
	}

	D3DXMatrixRotationY(&mMatrix, D3DX_PI / 2);
	D3DXMATRIX matrix;
	D3DXMatrixScaling(&matrix, 3, 3, 3);
	mMatrix = mMatrix * matrix;

	std::ofstream ofs("debug.txt");
	_XFIleUtils->PrintAll(pObj, ofs);
	ofs.close();


	_XFIleUtils->ParseAnimationController(pObj, &pController);
	
	
// 
// 	std::ofstream ofs2("outAni.txt");
// 	pController->DebugPrint(ofs2);
// 	ofs2.close();
// 
// 	std::ofstream ofs3("outFrame.txt");
 	GFrameContainer* pFrame = nullptr;
	_XFIleUtils->LoadFrameHierarchy(pObj, device, &pFrame);
// 	pFrame->getRootFrame()->DebugPrint(ofs3);
// 
// 	ofs3.close();
// 
// 	std::ofstream ofs4("outSkin.txt");
// 	pSkininfo->DebugPrint(ofs4);
// 	ofs4.close();

	cout << pFrame->getFrameCount() << endl;
	for (
		auto itor = pFrame->begin();
		itor != pFrame->end();
		++itor)
	{
		cout << itor->second->Name.c_str() << endl;
	}

	return true;
};


void WinLoop(){
	IDirect3DDevice9* device = dxGetDevice();

//	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	dxBeginScene();
	UINT numPass = 0;
	pEffect->Begin(&numPass, 0);
	pEffect->SetTechnique("NormalShader");


	pEffect->SetMatrix("worldMatrix", &mMatrix);
	pEffect->SetMatrix("viewMatrix", &vMatrix);
	pEffect->SetMatrix("projMatrix", &pMatrix);

	for (size_t i = 0; i < numAttribute; ++i)
	{
		pEffect->SetTexture("texture0", pTextureList[i]);
		pEffect->BeginPass(0);
		pMesh->DrawSubset(i);
		pEffect->EndPass();
	}
	pEffect->End();


// 	pDrawEffect->SetTechnique("DrawNormalShader");
// 	pDrawEffect->Begin(&numPass, 0);
// 	pDrawEffect->SetMatrix("worldMatrix", &mMatrix);
// 	pDrawEffect->SetMatrix("viewMatrix", &vMatrix);
// 	pDrawEffect->SetMatrix("projMatrix", &pMatrix);
// 	float color[4] = {1, 1, 0, 1};
// 	pDrawEffect->SetFloatArray("normalColor", color, 4);
// 	pDrawEffect->BeginPass(0);
// 	device->DrawPrimitiveUP(D3DPT_LINELIST, pMesh->GetNumVertices(), Normals, sizeof(D3DXVECTOR3));
// 	pDrawEffect->EndPass();
// 	pDrawEffect->End();
	dxEndScene();
};
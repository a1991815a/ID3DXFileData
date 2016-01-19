#include "XFileUtils.h"
#include <stdio.h>
#include <assert.h>														   
#include "XFileStruct.h"

#pragma warning(push)
#pragma warning(disable: 4477)

GUID GuidFromString(const char* sGuid)
{
	GUID retGuid = { 0 };

	char pBuf[32] = { 0 };

	int len = sscanf_s(
		sGuid,
		"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		&(pBuf[0]),
		&(pBuf[4]),
		&(pBuf[6]),
		&(pBuf[8]),
		&(pBuf[9]),
		&(pBuf[10]),
		&(pBuf[11]),
		&(pBuf[12]),
		&(pBuf[13]),
		&(pBuf[14]),
		&(pBuf[15])
		);
	memcpy_s(&retGuid, sizeof(GUID), pBuf, 16);

	return retGuid;
}


XFileUtils* XFileUtils::m_instance = nullptr;

XFileUtils* XFileUtils::getInstance()
{
	if (m_instance == nullptr)
		m_instance = new XFileUtils();
	return m_instance;
}

void XFileUtils::destroyInstance()
{
	if (m_instance)
	{
		delete m_instance;
		m_instance = nullptr;
	}
}

XFileUtils::XFileUtils()
	:m_GuidMap(), m_pFile(nullptr), m_UnknownString("Unknown")
{
	{
		m_GuidMap.insert(container_type::value_type("AnimationOptions",GuidFromString("E2BF56C0-840F-11cf-8F52-0040333594A3") ));
		m_GuidMap.insert(container_type::value_type("AnimTicksPerSecond",GuidFromString("9E415A43-7BA6-4a73-8743-B73D47E88476") ));
		m_GuidMap.insert(container_type::value_type("Boolean",GuidFromString("537da6a0-ca37-11d0-941c-0080c80cfa7b") ));
		m_GuidMap.insert(container_type::value_type("Boolean2d",GuidFromString("4885AE63-78E8-11cf-8F52-0040333594A3") ));
		m_GuidMap.insert(container_type::value_type("ColorRGB",GuidFromString("D3E16E81-7835-11cf-8F52-0040333594A3") ));
		m_GuidMap.insert(container_type::value_type("ColorRGBA",GuidFromString("35FF44E0-6C7C-11cf-8F52-0040333594A3") ));
		m_GuidMap.insert(container_type::value_type("IndexedColor",GuidFromString("1630B820-7842-11cf-8F52-0040333594A3") ));
		m_GuidMap.insert(container_type::value_type("CompressedAnimationSet",GuidFromString("7F9B00B3-F125-4890-876E-1C42BF697C4D") ));
		m_GuidMap.insert(container_type::value_type("Coords2d",GuidFromString("F6F23F44-7686-11cf-8F52-0040333594A3") ));
		m_GuidMap.insert(container_type::value_type("EffectDWord",GuidFromString("622C0ED0-956E-4da9-908A-2AF94F3CE716") ));
		m_GuidMap.insert(container_type::value_type("EffectFloats",GuidFromString("F1CFE2B3-0DE3-4e28-AFA1-155A750A282D") ));
		m_GuidMap.insert(container_type::value_type("Animation",GuidFromString("3D82AB4F-62DA-11cf-AB39-0020AF71E433") ));
		m_GuidMap.insert(container_type::value_type("EffectParamDWord",GuidFromString("E13963BC-AE51-4c5d-B00F-CFA3A9D97CE5") ));
		m_GuidMap.insert(container_type::value_type("EffectParamFloats",GuidFromString("3014B9A0-62F5-478c-9B86-E4AC9F4E418B") ));
		m_GuidMap.insert(container_type::value_type("EffectParamString",GuidFromString("1DBC4C88-94C1-46ee-9076-2C28818C9481") ));
		m_GuidMap.insert(container_type::value_type("EffectString",GuidFromString("D55B097E-BDB6-4c52-B03D-6051C89D0E42") ));
		m_GuidMap.insert(container_type::value_type("EffectInstance",GuidFromString("E331F7E4-0559-4cc2-8E99-1CEC1657928F") ));
		m_GuidMap.insert(container_type::value_type("FaceAdjacency",GuidFromString("A64C844A-E282-4756-8B80-250CDE04398C") ));
		m_GuidMap.insert(container_type::value_type("FloatKeys",GuidFromString("10DD46A9-775B-11cf-8F52-0040333594A3") ));
		m_GuidMap.insert(container_type::value_type("FVFData",GuidFromString("B6E70A0E-8EF9-4e83-94AD-ECC8B0C04897") ));
		m_GuidMap.insert(container_type::value_type("Guid",GuidFromString("a42790e0-7810-11cf-8f52-0040333594a3") ));
		m_GuidMap.insert(container_type::value_type("MaterialWrap",GuidFromString("4885ae60-78e8-11cf-8f52-0040333594a3") ));
		m_GuidMap.insert(container_type::value_type("Matrix4x4",GuidFromString("F6F23F45-7686-11cf-8F52-0040333594A3") ));
		m_GuidMap.insert(container_type::value_type("MeshFace",GuidFromString("3D82AB5F-62DA-11cf-AB39-0020AF71E433") ));
		m_GuidMap.insert(container_type::value_type("Patch",GuidFromString("A3EB5D44-FC22-429D-9AFB-3221CB9719A6") ));
		m_GuidMap.insert(container_type::value_type("PMAttributeRange",GuidFromString("917E0427-C61E-4a14-9C64-AFE65F9E9844") ));
		m_GuidMap.insert(container_type::value_type("Vector",GuidFromString("3D82AB5E-62DA-11cf-AB39-0020AF71E433") ));
		m_GuidMap.insert(container_type::value_type("VertexDuplicationIndices",GuidFromString("B8D65549-D7C9-4995-89CF-53A9A8B031E3") ));
		m_GuidMap.insert(container_type::value_type("VertexElement",GuidFromString("F752461C-1E23-48f6-B9F8-8350850F336F") ));
		m_GuidMap.insert(container_type::value_type("XSkinMeshHeader",GuidFromString("3CF169CE-FF7C-44ab-93C0-F78F62D172E2") ));
		m_GuidMap.insert(container_type::value_type("PMVSplitRecord",GuidFromString("574CCC14-F0B3-4333-822D-93E8A8A08E4C") ));
		m_GuidMap.insert(container_type::value_type("TextureFilename",GuidFromString("A42790E1-7810-11cf-8F52-0040333594A3") ));
		m_GuidMap.insert(container_type::value_type("Material",GuidFromString("3D82AB4D-62DA-11CF-AB39-0020AF71E433") ));
		m_GuidMap.insert(container_type::value_type("TimedFloatKeys",GuidFromString("F406B180-7B3B-11cf-8F52-0040333594A3") ));
		m_GuidMap.insert(container_type::value_type("AnimationKey",GuidFromString("10DD46A8-775B-11CF-8F52-0040333594A3") ));
		m_GuidMap.insert(container_type::value_type("FrameTransformMatrix",GuidFromString("F6F23F41-7686-11cf-8F52-0040333594A3") ));
		m_GuidMap.insert(container_type::value_type("AnimationSet",GuidFromString("3D82AB50-62DA-11cf-AB39-0020AF71E433") ));
		m_GuidMap.insert(container_type::value_type("DeclData",GuidFromString("BF22E553-292C-4781-9FEA-62BD554BDD93") ));
		m_GuidMap.insert(container_type::value_type("MeshFaceWraps",GuidFromString("ED1EC5C0-C0A8-11D0-941C-0080C80CFA7B") ));
		m_GuidMap.insert(container_type::value_type("MeshNormals",GuidFromString("F6F23F43-7686-11cf-8F52-0040333594A3") ));
		m_GuidMap.insert(container_type::value_type("MeshTextureCoords",GuidFromString("F6F23F40-7686-11cf-8F52-0040333594A3") ));
		m_GuidMap.insert(container_type::value_type("MeshVertexColors",GuidFromString("1630B821-7842-11cf-8F52-0040333594A3") ));
		m_GuidMap.insert(container_type::value_type("MeshMaterialList",GuidFromString("F6F23F42-7686-11CF-8F52-0040333594A3") ));
		m_GuidMap.insert(container_type::value_type("SkinWeights",GuidFromString("6F0D123B-BAD2-4167-A0D0-80224F25FABB") ));
		m_GuidMap.insert(container_type::value_type("PMInfo",GuidFromString("B6C3E656-EC8B-4b92-9B62-681659522947") ));
		m_GuidMap.insert(container_type::value_type("Mesh",GuidFromString("3D82AB44-62DA-11CF-AB39-0020AF71E433") ));
		m_GuidMap.insert(container_type::value_type("PatchMesh",GuidFromString("D02C95CC-EDBA-4305-9B5D-1820D7704BBF") ));
		m_GuidMap.insert(container_type::value_type("PatchMesh9",GuidFromString("B9EC94E1-B9A6-4251-BA18-94893F02C0EA") ));
		m_GuidMap.insert(container_type::value_type("Frame",GuidFromString("3D82AB46-62DA-11CF-AB39-0020AF71E433") ));
	}

	FILE* f = nullptr;
	errno_t err = fopen_s(&f, "Template.txt", "rb+");
	assert(err == 0);
	char ch = EOF;
	while ((ch = getc(f)) != EOF)
		m_strTemplate.push_back(ch);
	fclose(f);
	D3DXFileCreate(&m_pFile);
	HRESULT result = m_pFile->RegisterTemplates((void*)m_strTemplate.c_str(), (SIZE_T)m_strTemplate.size());
	HRESULT re = D3DXFERR_PARSEERROR;
	assert(SUCCEEDED(result));
}

XFileUtils::~XFileUtils()
{

}

const GUID& XFileUtils::getTemplateGuid(const char* strTemplate) const
{
	return m_GuidMap.at(strTemplate);
}

const std::string& XFileUtils::getStringFromGuid( const GUID& guid ) const
{
	auto itor = m_GuidMap.begin();
	for (; itor != m_GuidMap.end(); ++itor)
		if(itor->second == guid)
			return itor->first;
	return m_UnknownString;
}

void XFileUtils::PrintAll( ID3DXFileEnumObject* pObject, std::ostream& os)
{
	std::vector<size_t>			stack_depth;		//保存节点深度
	{
		SIZE_T numChild = 0;
		pObject->GetChildren(&numChild);
		for (SIZE_T i = numChild - 1; i >= 0 && i < numChild; --i)
		{
			ID3DXFileData* pFileData = nullptr;
			pObject->GetChild(i, &pFileData);
			m_LoadStack.push_back(pFileData);
			stack_depth.push_back(1);
		}
	}

	while (!m_LoadStack.empty())
	{
		ID3DXFileData* pLoadFileData = m_LoadStack.back();
		m_LoadStack.pop_back();
		size_t depth = stack_depth.back();
		stack_depth.pop_back();


		int ss = m_LoadStack.size();
		GUID gGuid = { 0 };
		char name[128] = { 0 };
		SIZE_T nameLen = 128;
		pLoadFileData->GetType(&gGuid);
		pLoadFileData->GetName(name, &nameLen);
		const std::string& typeName = getStringFromGuid(gGuid);

		{
			for (size_t i = 0; i < depth - 1; ++i)
				os.write("  ", 2);

			if (pLoadFileData->IsReference())
			{
				os.write("[", 1);
			}
			os.write(typeName.c_str(), typeName.size());
			os.write(" ", 1);
			os.write(name, strlen(name));
			
			if (pLoadFileData->IsReference())
			{
				os.write("]", 1);
				os.write("\n", 1);
				continue;
			}
			os.write("\n", 1);
		}


		SIZE_T numChild = 0;
		pLoadFileData->GetChildren(&numChild);
		for (SIZE_T i = numChild - 1; i >= 0 && i < numChild; --i)
		{
			ID3DXFileData* pInsertFileData = nullptr;
			pLoadFileData->GetChild(i, &pInsertFileData);
			m_LoadStack.push_back(pInsertFileData);
			stack_depth.push_back(depth + 1);
		}
	}
}

bool XFileUtils::LoadXFile( const char* path, ID3DXFileEnumObject** pNewObject ) const
{
	return SUCCEEDED(
		m_pFile->CreateEnumObject(
		(void*)path,
		D3DXF_FILELOAD_FROMFILE,
		pNewObject
		));
}

void XFileUtils::LoadChild( GLoadCallback& call, ID3DXFileEnumObject* pObject )
{
	{
		SIZE_T numChild = 0;
		pObject->GetChildren(&numChild);
		for (SIZE_T i = numChild - 1; i >= 0 && i < numChild; --i)
		{
			ID3DXFileData* pFileData = nullptr;
			pObject->GetChild(i, &pFileData);
			m_LoadStack.push_back(pFileData);
		}
	}

	while (!m_LoadStack.empty())
	{
		ID3DXFileData* pLoadFileData = m_LoadStack.back();
		m_LoadStack.pop_back();

		int ss = m_LoadStack.size();
		GUID gGuid = { 0 };
		pLoadFileData->GetType(&gGuid);

		call.Excute(gGuid, pLoadFileData);

		SIZE_T numChild = 0;
		pLoadFileData->GetChildren(&numChild);
		for (SIZE_T i = numChild - 1; i >= 0 && i < numChild; --i)
		{
			ID3DXFileData* pInsertFileData = nullptr;
			pLoadFileData->GetChild(i, &pInsertFileData);
			m_LoadStack.push_back(pInsertFileData);
		}
	}
}

ID3DXFileData* XFileUtils::SearchChild(
	const GUID& guid, ID3DXFileData* pFileData, size_t offset /*= 0*/ )
{
	SIZE_T numChild = 0;
	pFileData->GetChildren(&numChild);
	for (size_t i = offset; i < numChild; ++i)
	{
		GUID cguid;
		ID3DXFileData* pChild = nullptr;
		pFileData->GetChild(i, &pChild);
		pChild->GetType(&cguid);
		if(guid == cguid)
			return pChild;
	}
	return nullptr;
}

size_t XFileUtils::GetChildCountByGuid( const GUID& guid, ID3DXFileData* pFileData )
{
	size_t sumChild = 0;
	SIZE_T numChild = 0;
	pFileData->GetChildren(&numChild);
	for (size_t i = 0; i < numChild; ++i)
	{
		ID3DXFileData* pChild = nullptr;
		GUID cguid;
		pFileData->GetChild(i, &pChild);
		pChild->GetType(&cguid);

		if(cguid == guid)
			++sumChild;
	}

	return sumChild;
}

GLoadCallback::GLoadCallback()
	:m_UnknownCall(nullptr)
{

}

void GLoadCallback::RegisterCall( const GUID& guid, GLoadCall_t call )
{
	auto itor = m_CallMap.find(guid);
	if(itor != m_CallMap.end())
		return;
	m_CallMap.insert(container_type::value_type(guid, call));
}

void GLoadCallback::UnregisterCall( const GUID& guid )
{
	auto itor = m_CallMap.find(guid);
	if(itor != m_CallMap.end())
		m_CallMap.erase(itor);
}

void GLoadCallback::RegisterUnknownCall( GLoadCall_t call )
{
	m_UnknownCall = call;
}

void GLoadCallback::UnregisterUnknownCall()
{
	m_UnknownCall = nullptr;
}

void GLoadCallback::Excute(const GUID& guid, ID3DXFileData* pFileData)
{
	auto itor = m_CallMap.find(guid);
	if(itor != m_CallMap.end())
	{
		itor->second(guid, pFileData);
		return;
	}

	if(m_UnknownCall)
		m_UnknownCall(guid, pFileData);
}

bool XFileUtils::LoadMesh(
	ID3DXFileData*		pFileData, 
	IDirect3DDevice9*	pDevice, 
	ID3DXMesh**			pNewMesh, 
	ID3DXBuffer**		pMaterial,
	DWORD*				pNumAttribute,
	GSkinInfo**			pNewSkininfo)
{
	//检查类型
	_XCheckType(pFileData, "Mesh", false);
	DWORD nVertices = 0;
	DWORD nFaces = 0;
	_XCheckExcute(ParseBasemesh(pFileData, &nVertices, &nFaces, 
		nullptr, nullptr, sizeof(XVertex_p3_n3_t1)));
	ID3DXMesh* pMesh = nullptr;
	if (FAILED(D3DXCreateMeshFVF(nFaces, nVertices, 
		D3DXMESH_32BIT | D3DXMESH_MANAGED, XVertex_p3_n3_t1::fvf, pDevice, &pMesh)))
		return false;
	void* pVertices = nullptr;
	DWORD* pIndices = nullptr;
	DWORD* pAttribtues = nullptr;
	pMesh->LockVertexBuffer(0, &pVertices);
	pMesh->LockIndexBuffer(0, (void**)&pIndices);
	pMesh->LockAttributeBuffer(0, &pAttribtues);
	_XCheckExcute(ParseBasemesh(pFileData, &nVertices, &nFaces, 
		pVertices, pIndices, sizeof(XVertex_p3_n3_t1)));

	_XCheckExcute(ParseNormals(pFileData, nVertices, pVertices, pIndices,
		sizeof(XVertex_p3_n3_t1), sizeof(D3DXVECTOR3)));

	_XCheckExcute(ParseTexcoord(pFileData, pVertices,
		sizeof(XVertex_p3_n3_t1), sizeof(D3DXVECTOR3) * 2));

	_XCheckExcute(ParseMaterial(pFileData, pAttribtues, nFaces, pNumAttribute, pMaterial));

	pMesh->UnlockAttributeBuffer();
	pMesh->UnlockIndexBuffer();
	pMesh->UnlockVertexBuffer();

	*pNewMesh = pMesh;
	
	ParseSkinInfo(pFileData, pNewSkininfo);
	return true;
}

bool XFileUtils::CheckType( const GUID& guid, ID3DXFileData* pFileData ) const
{
	GUID cguid;
	pFileData->GetType(&cguid);
	return cguid == guid;
}

bool XFileUtils::LoadMeshContainer(
	ID3DXFileData* pFileData, 
	IDirect3DDevice9* pDevice, 
	GMeshContainer** pNewMeshContainer)
{
	_XCheckType(pFileData, "Mesh", false);

	GMeshContainer* pMeshContainer = new GMeshContainer();

	char name[128] = { 0 };
	SIZE_T nameSize = 128;
	pFileData->GetName(name, &nameSize);
	pMeshContainer->Name = name;

	if (!LoadMesh(
		pFileData, 
		pDevice,
		&pMeshContainer->pMesh,
		&pMeshContainer->pMaterials,
		&pMeshContainer->NumMaterials, 
		&pMeshContainer->pSkinInfo))
		return false;

	*pNewMeshContainer = pMeshContainer;
	return true;
}

bool XFileUtils::LoadFrame(
	ID3DXFileData* pFileData, 
	IDirect3DDevice9* pDevice, 
	GFrame** pNewFrame)
{

	GFrame* pFrame = new GFrame();
	char name[128] = { 0 };
	SIZE_T nameSize = 128;
	pFileData->GetName(name, &nameSize);
	pFrame->Name = name;

	ID3DXFileData* transFileData = SearchChild(_XTemplate("FrameTransformMatrix"), pFileData);
	if (transFileData)
	{
		_XFileDataLock(transFileData, transSize, transData);
		D3DXMATRIX* pInMatrix = _XToValue(D3DXMATRIX*, 0, transData);
// 		memcpy_s(&pFrame->TransformationMatrix, sizeof(D3DXMATRIX),
// 			transData, sizeof(D3DXMATRIX));
		D3DXMatrixTranspose(&pFrame->TransformationMatrix, pInMatrix);
		_XFileDataUnlock(transFileData);
	}

	ID3DXFileData* pMeshFileData = SearchChild(_XTemplate("Mesh"), pFileData);
	if (pMeshFileData)
	{
		if (!LoadMeshContainer(pMeshFileData, pDevice, &pFrame->pMeshContainer))
			return false;
	}

	*pNewFrame = pFrame;
	return true;
}

bool XFileUtils::LoadFrameHierarchy(
	ID3DXFileEnumObject* pFileData, 
	IDirect3DDevice9* pDevice, 
	GFrameContainer** pNewFrameContainer)
{
	GFrameContainer* pFrameContainer = new GFrameContainer();

	SIZE_T numChild = 0;
	pFileData->GetChildren(&numChild);

	struct FrameFD { 
		GFrame*				pFrame;
		ID3DXFileData*		pFileData;
	};

	//读取用栈
	std::vector<FrameFD>	FrameStack;
	//框架GUID
	const GUID& frameGuid = _XTemplate("Frame");

	//创建根框架
	GFrame* pRootFrame = new GFrame();
	pRootFrame->Name = GROOTFRAME;
	pFrameContainer->m_pRootFrames = pRootFrame;
	pFrameContainer->InsertFrame(pRootFrame);

	//得到第一层 框架
	//*********************************
	{
		GFrame* pPreSiblingFrame = nullptr;
		for (size_t i = 0; i < numChild; ++i)
		{
			ID3DXFileData* pTopFileData = nullptr;
			GFrame* pTopChild = nullptr;
			pFileData->GetChild(i, &pTopFileData);
			GUID cguid;
			pTopFileData->GetType(&cguid);

			if (cguid == frameGuid)
			{
				if (!LoadFrame(pTopFileData, pDevice, &pTopChild))
					return false;

				if (pRootFrame->pFrameFirstChild == nullptr)
				{
					pRootFrame->pFrameFirstChild = pTopChild;
					pFrameContainer->InsertFrame(pTopChild);
					pPreSiblingFrame = pTopChild;
				}
				else {
					pPreSiblingFrame->pFrameSibling = pTopChild;
					pPreSiblingFrame = pTopChild;
				}

				FrameFD fd;
				fd.pFileData = pTopFileData;
				fd.pFrame = pTopChild;
				FrameStack.push_back(fd);
			}
		}
	}
	//*********************************
	//得到第一层 框架
	
	while (!FrameStack.empty())
	{
		//上一个兄弟节点
		GFrame* pPreSiblingFrame = nullptr;
		//栈弹出
		FrameFD fd = FrameStack.back();
		FrameStack.pop_back();
		GFrame*			pParentFrame = fd.pFrame;
		ID3DXFileData*	pParentFileData = fd.pFileData;

		//得到孩子个数
		SIZE_T numChild = 0;
		pParentFileData->GetChildren(&numChild);

		//开始解析该节点的孩子
		for (size_t i = 0; i < numChild; ++i)
		{
			//读取孩子
			ID3DXFileData* pChildFileData = nullptr;
			pParentFileData->GetChild(i, &pChildFileData);
			GFrame* pChildFrame = nullptr;
			//解析该 孩子框架,失败进入条件返回 false
			GUID cguid;
			pChildFileData->GetType(&cguid);
			if (cguid != frameGuid)
				continue;
			if (!LoadFrame(pChildFileData, pDevice, &pChildFrame))
			{
				char DebugText[512] = { 0 };
				sprintf_s(DebugText, "LoadFrame Error: %s \n", pParentFrame->Name.c_str());
				OutputDebugStringA(DebugText);
				continue;
			}
			
			if (pPreSiblingFrame == nullptr)
			{
				//如果是第一个孩子节点，则将pFirstChild指向该节点，
				//并将preSiblingFrame指针指向它, 并压入解析用栈
				pParentFrame->pFrameFirstChild = pChildFrame;
				pPreSiblingFrame = pChildFrame;
			}
			else {
				//上一个兄弟节点指针连接, preSiblingFrame指针往下移
				pPreSiblingFrame->pFrameSibling = pChildFrame;
				pPreSiblingFrame = pChildFrame;
			}
			
			FrameFD tmp_fd;
			tmp_fd.pFrame = pChildFrame;
			tmp_fd.pFileData = pChildFileData;
			FrameStack.push_back(tmp_fd);
			pFrameContainer->InsertFrame(pChildFrame);
		}
	}


	*pNewFrameContainer = pFrameContainer;
	return true;
}

bool XFileUtils::ParseBasemesh(
	ID3DXFileData* pFileData,		/*网格文件节点 */ 
	DWORD* pNVertices,				/*顶点个数输出 */
	DWORD* pNFaces,					/*面个数输出 */
	void* pVertices,				/*顶点数据赋值 */ 
	DWORD* pIndex,					/*索引数据输出 */ 
	size_t verStride				/*顶点跨度 */
	)
{
	_XFileDataLock(pFileData, vSize, vData);
	if (pVertices && pIndex)
	{
		char* pCVertices = (char*)pVertices;

		XVector* pXVector = _XToValue(XVector*, sizeof(DWORD), vData);
		XMeshFace* pXFaces = _XToValue(XMeshFace*, sizeof(DWORD) * 2 + sizeof(XVector) * *pNVertices, vData);
		
		//顶点信息赋值
		{
			for (size_t i = 0; i < *pNVertices; ++i)
			{
				memcpy_s(pCVertices, sizeof(XVector), &pXVector[i], sizeof(XVector));
				pCVertices += verStride;
			}
		}

		//索引信息赋值
		{
			size_t array_index = 0;
			for (size_t i = 0; i < *pNFaces; ++i)
			{
				pIndex[array_index++] = pXFaces[i].faceVertexIndices[0];
				pIndex[array_index++] = pXFaces[i].faceVertexIndices[1];
				pIndex[array_index++] = pXFaces[i].faceVertexIndices[2];
			}
		}
	}

	if (pNVertices && pNFaces)
	{
		*pNVertices = *_XToValue(DWORD*, 0, vData);
		*pNFaces = *_XToValue(DWORD*, sizeof(DWORD) + *pNVertices * sizeof(XVector), vData);
	}
	_XFileDataUnlock(pFileData);

	return true;
}


bool XFileUtils::ParseNormals(
	ID3DXFileData* pFileData,			/*网格文件节点 */
	DWORD nVertices,					/*顶点个数 */
	void* pVertices,					/*顶点数据赋值 */ 
	const DWORD* pIndex,				/*索引数据查询 */ 
	size_t verStride,					/*顶点跨度 */ 
	size_t normalOffset				/*法线数据 偏移 */
	)
{
	ID3DXFileData* pNormalFileData = SearchChild(_XTemplate("MeshNormals"), pFileData);
	if (pNormalFileData == nullptr)
		return false;

	_XFileDataLock(pNormalFileData, normalSize, normalData);
	DWORD nNormals = *_XToValue(DWORD*, 0, normalData);
	DWORD nFaceNormals = *_XToValue(DWORD*, sizeof(DWORD) + sizeof(D3DXVECTOR3) * nNormals, normalData);
	D3DXVECTOR3* normals = _XToValue(D3DXVECTOR3*, sizeof(DWORD), normalData);
	XMeshFace* faceNormals = _XToValue(XMeshFace*, sizeof(DWORD) * 2 + sizeof(D3DXVECTOR3) * nNormals, normalData);
	DWORD* normalShared = new DWORD[nVertices];
	char* pNVertices = (char*)pVertices + normalOffset;
	memset(normalShared, 0, sizeof(DWORD) * nVertices);

	for (size_t i = 0; i < nFaceNormals; ++i)
	{
		DWORD& nIndex1 = faceNormals[i].faceVertexIndices[0];
		DWORD& nIndex2 = faceNormals[i].faceVertexIndices[1];
		DWORD& nIndex3 = faceNormals[i].faceVertexIndices[2];

		const DWORD& vIndex1 = pIndex[i * 3];
		const DWORD& vIndex2 = pIndex[i * 3 + 1];
		const DWORD& vIndex3 = pIndex[i * 3 + 2];

		D3DXVECTOR3& normal1 = *(D3DXVECTOR3*)(pNVertices + verStride * vIndex1);
		D3DXVECTOR3& normal2 = *(D3DXVECTOR3*)(pNVertices + verStride * vIndex2);
		D3DXVECTOR3& normal3 = *(D3DXVECTOR3*)(pNVertices + verStride * vIndex3);

		normal1 += normals[nIndex1];
		normal2 += normals[nIndex2];
		normal3 += normals[nIndex3];

		++normalShared[vIndex1];
		++normalShared[vIndex2];
		++normalShared[vIndex3];
	}
		

	for (size_t i = 0; i < nVertices; ++i)
	{
		if (normalShared[i] == 0)
			continue;
		D3DXVECTOR3& normal = *(D3DXVECTOR3*)(pNVertices + verStride * i);
		normal /= (float)normalShared[i];
		D3DXVec3Normalize(&normal, &normal);
	}

	delete[] normalShared;
	return true;
}

bool XFileUtils::ParseTexcoord(
	ID3DXFileData* pFileData,		/*网格文件节点 */
	void* pVertices,				/*顶点数据赋值 */
	size_t verStride,				/*顶点跨度 */ 
	size_t texOffset				/*纹理坐标 偏移 */
	)
{
	ID3DXFileData* pTexFileData = SearchChild(_XTemplate("MeshTextureCoords"), pFileData);
	if (pTexFileData == nullptr)
		return false;

	_XFileDataLock(pTexFileData, texSize, texData);
	DWORD nTextureCoords = *_XToValue(DWORD*, 0, texData);
	XCoords2d* pCoords = _XToValue(XCoords2d*, sizeof(DWORD), texData);
	char* pCVertices = (char*)pVertices + texOffset;
	for (size_t i = 0; i < nTextureCoords; ++i)
	{
		memcpy_s(pCVertices, sizeof(XCoords2d), &pCoords[i], sizeof(XCoords2d));
		pCVertices += verStride;
	}
	_XFileDataUnlock(pTexFileData);
	return true;
}

bool XFileUtils::ParseMaterial(
	ID3DXFileData* pFileData,	/*网格文件节点 */
	DWORD* pAttributes,		/*属性缓冲区赋值 */
	DWORD  nFaces,				/*面个数(属性缓冲区长度) */
	DWORD* nMaterials,			/*属性个数 */ 
	ID3DXBuffer** ppMaterial		/*材质缓冲 */
	)
{
	ID3DXFileData* pMaterialListFileData = SearchChild(_XTemplate("MeshMaterialList"), pFileData);
	if (pMaterialListFileData == nullptr)
	{
		memset(pAttributes, 0, sizeof(DWORD) * nFaces);
		*nMaterials = 0;
		return false;
	}

	//属性缓冲读取
	_XFileDataLock(pMaterialListFileData, materialListSize, materialListData);
	//材质个数
	*nMaterials = *_XToValue(DWORD*, 0, materialListData);
	//面索引个数
	DWORD nFaceIndexes = *_XToValue(DWORD*, 0, materialListData);
	DWORD* faceIndexes = _XToValue(DWORD*, sizeof(DWORD), materialListData);
	memcpy_s(pAttributes, sizeof(DWORD) * nFaces, 
		faceIndexes, sizeof(DWORD) * nFaceIndexes);
	_XFileDataUnlock(pMaterialListFileData);

	if (*nMaterials == 0)
	{
		*ppMaterial = nullptr;
		return true;
	}

	//材质纹理信息读取
	{
		//空间分配
		std::string* TextureFilenameList = new std::string[*nMaterials];
		D3DXMATERIAL* MatList = new D3DXMATERIAL[*nMaterials];
		memset(MatList, 0, sizeof(D3DXMATERIAL) * *nMaterials);
		//得到孩子数量
		SIZE_T nChild = 0;
		pMaterialListFileData->GetChildren(&nChild);
		if (nChild == 0)
			return false;
		//材质计数
		size_t MatCount = 0;
		const GUID& MatGuid = _XTemplate("Material");
		const GUID& TexGuid = _XTemplate("TextureFilename");

		//纹理信息 字节计数
		DWORD nBytes = 0;
		//纹理信息字节偏移
		const DWORD FileDataOffset = sizeof(D3DXMATERIAL) * *nMaterials;

		for (size_t i = 0; i < nChild; ++i)
		{
			ID3DXFileData* pChild = nullptr;
			pMaterialListFileData->GetChild(i, &pChild);
			GUID cguid;
			//类型检查
			pChild->GetType(&cguid);
			if(cguid != MatGuid)
				continue;

			_XFileDataLock(pChild, childSize, childData);

			XMaterial* pXMaterial = _XToValue(XMaterial*, 0, childData);
			pXMaterial->GetD3DMATERIAL9(&MatList[MatCount].MatD3D);

			//纹理读取
			ID3DXFileData* pTextureFilenameFD = SearchChild(TexGuid, pChild);
			if (pTextureFilenameFD != nullptr)
			{
				_XFileDataLock(pTextureFilenameFD, texSize, texData);
				TextureFilenameList[MatCount].resize(texSize);
				memcpy_s((char*)TextureFilenameList[MatCount].data(), texSize,
					texData, texSize);


				MatList[MatCount].pTextureFilename = (char*)(nBytes + FileDataOffset);
				nBytes += texSize;
				_XFileDataUnlock(pTextureFilenameFD);
			}

			_XFileDataUnlock(pChild);
			++MatCount;
		}

		if (MatCount < *nMaterials)
		{
			delete[] TextureFilenameList;
			delete[] MatList;
			return false;
		}

		//开始创建缓冲区
		if (MatCount >= *nMaterials)
		{
			D3DXCreateBuffer(nBytes + sizeof(D3DXMATERIAL) * *nMaterials, ppMaterial);
			char* pData = (char*)(*ppMaterial)->GetBufferPointer();
			D3DXMATERIAL* pMaterialData = (D3DXMATERIAL*)pData;
			memcpy_s(pMaterialData, sizeof(D3DXMATERIAL) * *nMaterials,
				MatList, sizeof(D3DXMATERIAL) * *nMaterials);
			char* pTexOffset = pData + sizeof(D3DXMATERIAL) * *nMaterials;
			size_t pTexIndexOffset = 0;
			for (size_t i = 0; i < *nMaterials; ++i)
			{
				if (TextureFilenameList[i].size() == 0)
				{
					pMaterialData[i].pTextureFilename = nullptr;
					continue;
				}
				pMaterialData[i].pTextureFilename += (int)pData;
				memcpy_s(&pTexOffset[pTexIndexOffset], TextureFilenameList[i].size(),
					TextureFilenameList[i].data(), TextureFilenameList[i].size());
				pTexIndexOffset += TextureFilenameList[i].size();
			}

		}

		delete[] TextureFilenameList;
		delete[] MatList;
	}

	return true;
}

bool XFileUtils::ParseSkinInfo(
	ID3DXFileData* pFileData,	/*网格文件节点 */ 
	GSkinInfo** pNewSkininfo		/*蒙皮信息接口 */)
{
	WORD nMaxSkinWeightsPerVertex = 4;
	WORD nMaxSkinWeightsPerFace = 0;
	WORD nBones = 0;

	ID3DXFileData* pSkinHeaderFileData = SearchChild(_XTemplate("XSkinMeshHeader"), pFileData);
	if (pSkinHeaderFileData == nullptr)
	{
		*pNewSkininfo = nullptr;
		return false;
	}

	_XFileDataLock(pSkinHeaderFileData, skinHeaderSize, skinHeaderData);
	nMaxSkinWeightsPerVertex = *_XToValue(WORD*, 0, skinHeaderData);
	nMaxSkinWeightsPerFace = *_XToValue(WORD*, sizeof(WORD), skinHeaderData);
	nBones = *_XToValue(WORD*, sizeof(WORD) * 2, skinHeaderData);
	_XFileDataUnlock(pSkinHeaderFileData);

	GSkinInfo* pSkinInfo = new GSkinInfo();
	pSkinInfo->m_nBones = nBones;
	pSkinInfo->m_nMaxSkinWeightsPerVertex = nMaxSkinWeightsPerVertex;
	pSkinInfo->m_nMaxSkinWeightsPerFace = nMaxSkinWeightsPerFace;


	SIZE_T numChild = 0;
	pFileData->GetChildren(&numChild);
	
	const GUID& boneGuid = _XTemplate("SkinWeights");
	for (size_t i = 0; i < numChild; ++i)
	{
		ID3DXFileData* pBoneFileData = nullptr;
		pFileData->GetChild(i, &pBoneFileData);
		GUID cguid;
		pBoneFileData->GetType(&cguid);
		if(boneGuid != cguid)
			continue;

		GSkinBone* pBone = new GSkinBone();
		size_t nameLen = 0;
		_XFileDataLock(pBoneFileData, boneSize, BoneData);
		char* boneName = (char*)BoneData;
		pBone->transformNodeName = boneName;
		for (size_t i = 0; i < boneSize; ++i)
		{
			if (boneName[i] == '\0')
			{
				++nameLen;
				break;
			}
			++nameLen;
		}

		DWORD nWeights = *_XToValue(DWORD*, nameLen, BoneData);
		DWORD* vertexIndices = _XToValue(DWORD*, nameLen + sizeof(DWORD), BoneData);
		float* weights = _XToValue(float*, nameLen + sizeof(DWORD) * (nWeights + 1), BoneData);
		D3DXMATRIX* pInMatrixOffset = _XToValue(D3DXMATRIX*, nameLen + sizeof(DWORD) * (nWeights + 1) + sizeof(float) * nWeights, BoneData);
		D3DXMatrixTranspose(&pBone->matrixOffset, pInMatrixOffset);
		pBone->nWeights = nWeights;
		pBone->vertexIndices = new DWORD[nWeights];
		pBone->weights = new float[nWeights];

		memcpy_s(pBone->vertexIndices, sizeof(DWORD) * nWeights,
			vertexIndices, sizeof(DWORD) * nWeights);
		memcpy_s(pBone->weights, sizeof(float) * nWeights,
			weights, sizeof(float) * nWeights);
		_XFileDataUnlock(pBoneFileData);

		pSkinInfo->m_pBoneMap[pBone->transformNodeName] = pBone;
	}
	
	*pNewSkininfo = pSkinInfo;
	return true;
}

bool XFileUtils::ParseAnimationController(
	ID3DXFileEnumObject* pFileData,					/*文件 */ 
	GAnimationController** pNewAnimationController	/*动画控制器 */
	)
{
	SIZE_T numChild = 0;
	pFileData->GetChildren(&numChild);

	GAnimationController* pNewController = new GAnimationController();

	const GUID& anisetGuid = _XTemplate("AnimationSet");
	for (size_t i = 0; i < numChild; ++i)
	{
		GUID cguid;
		ID3DXFileData* pChild = nullptr;
		pFileData->GetChild(i, &pChild);
		pChild->GetType(&cguid);
		if(cguid != anisetGuid)
			continue;
		GAnimationSet* pNewAnimationSet = nullptr;
		if (ParseAnimationSet(pChild, &pNewAnimationSet))
			pNewController->InsertAnimationSet(pNewAnimationSet);
	}

	*pNewAnimationController = pNewController;
	return true;
}

bool XFileUtils::ParseAnimationSet(
	ID3DXFileData* pFileData,			/*文件节点 */
	GAnimationSet** pNewAnimationSet	/*输出动画集 */
	)
{
	char name[128] = { 0 };
	SIZE_T nameLen = 128;
	pFileData->GetName(name, &nameLen);
	GAnimationSet* aniset = new GAnimationSet();
	aniset->m_Name = name;
	
	SIZE_T numChild = 0;

	const GUID& animationGuid = _XTemplate("Animation");

	pFileData->GetChildren(&numChild);
	for (size_t i = 0; i < numChild; ++i)
	{
		GUID cguid;
		ID3DXFileData* pChild = nullptr;
		pFileData->GetChild(i, &pChild);
		pChild->GetType(&cguid);
		if(cguid != animationGuid)
			continue;

		GAnimation* pNewAnimation = nullptr;
		if(ParseAnimation(pChild, &pNewAnimation))
			aniset->m_AnimationVector.push_back(pNewAnimation);
	}

	*pNewAnimationSet = aniset;
	return true;
}

bool XFileUtils::ParseAnimation(
	ID3DXFileData* pFileData,		/*文件节点 */ 
	GAnimation** pNewAnimation		/*动画 */
	)
{
	GAnimation* ani = new GAnimation();
	char name[128] = { 0 };
	SIZE_T nameLen = 128;
	pFileData->GetName(name, &nameLen);
	ani->m_Name = name;
	
	ID3DXFileData* pOptions = SearchChild(_XTemplate("AnimationOptions"), pFileData);
	if (pOptions)
	{
		_XFileDataLock(pOptions, opSize, opData);
		ani->m_AnimationOptions = *_XToValue(GAnimationOptions*, 0, opData);
		_XFileDataUnlock(pOptions);
	}

	ID3DXFileData* pAniKeyFD = SearchChild(_XTemplate("AnimationKey"), pFileData);
	if (pAniKeyFD)
	{
		ani->m_pAnimationKey = new GAnimationKey();
		_XFileDataLock(pAniKeyFD, anikeySize, anikeyData);
		DWORD keyType = *_XToValue(DWORD*, 0, anikeyData);
		DWORD nKeys = *_XToValue(DWORD*, sizeof(DWORD), anikeyData);
		GTimedFloatKeys* pInKeys = _XToValue(GTimedFloatKeys*, sizeof(DWORD) * 2, anikeyData);
		GTimedFloatKeys* pKeys = new GTimedFloatKeys[nKeys];
		memcpy_s(pKeys, sizeof(GTimedFloatKeys) * nKeys,
			pInKeys, sizeof(GTimedFloatKeys) * nKeys);
		D3DXMatrixTranspose(&pKeys->Matrix, &pKeys->Matrix);
		_XFileDataUnlock(pAniKeyFD);

		ani->m_pAnimationKey->m_keyType = keyType;
		ani->m_pAnimationKey->m_nKeys = nKeys;
		ani->m_pAnimationKey->m_keys = pKeys;
	}

	//绑定框架
	{
		ID3DXFileData* pFrameFileData = SearchChild(_XTemplate("Frame"), pFileData);
		if (pFrameFileData)
		{
			char FrameName[128] = { 0 };
			SIZE_T nameLen = 128;
			pFrameFileData->GetName(FrameName, &nameLen);
			ani->m_pBoneName = FrameName;
		}
	}

	*pNewAnimation = ani;
	return true;
}

#pragma warning(pop)
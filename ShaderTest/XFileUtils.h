#ifndef __XFILEUTILS__
#define __XFILEUTILS__
#include <string>
#include <unordered_map>
#include <d3d9.h>
#include <d3dx9.h>
#include <functional>
#include <iostream>
#include "GAnimation.h"

#define _XMeshFVFDefault				D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1
#define _XFIleUtils						XFileUtils::getInstance()
#define _XTemplate(T)					_XFIleUtils->getTemplateGuid(T)
#define _XToValue(TYPE, offset, p)		(TYPE)((char*)p + offset)
#define _XCheckType(_FILEDATA, TEMPLATE, VAL) \
{ \
	GUID gGuid; \
	_FILEDATA->GetType(&gGuid); \
	if (gGuid != _XTemplate(TEMPLATE)) \
	{ \
		OutputDebugStringA("Type Error:"); \
		OutputDebugStringA(getStringFromGuid(gGuid).c_str()); \
		OutputDebugStringA("\n"); \
		return VAL; \
	} \
}

#define _XCheckExcute(func)				if(!func) return false;

#define _XBeginParseChild(Index, numChild, Child, TEMPLATE, FILEDATA) \
for (size_t Index = 0; Index < numChild; ++Index) \
{ \
	ID3DXFileData* Child = nullptr; \
	FILEDATA->GetChild(Index, &Child); \
	if (!CheckType(_XTemplate(TEMPLATE), Child)) \
		continue;

#define _XEndParseChild()				}

#define _XFileDataLock(_FILEDATA, _SIZE, _DATA) \
SIZE_T _SIZE = 0; \
LPCVOID _DATA = nullptr; \
if(_FILEDATA) \
	_FILEDATA->Lock(&_SIZE, &_DATA)

#define _XFileDataUnlock(_FILEDATA) if(_FILEDATA) _FILEDATA->Unlock();

inline size_t XFile_seq(const unsigned char* pData, size_t len)
{
	size_t _Val = 2166136261U;
	for (size_t i = 0; i < len; ++i)
		_Val = 16777619U * _Val ^ (size_t)pData[i];
	return _Val;
};

GUID GuidFromString(const char* sGuid);

struct GuidHasher{
	size_t operator()(const GUID& _left) const{
		return XFile_seq((unsigned char*)&_left, sizeof(GUID));
	};
};

struct GLoadCallback{
public:
	typedef std::function<void(const GUID& guid, ID3DXFileData* pFileData)> GLoadCall_t;
private:
	typedef std::unordered_map<GUID, GLoadCall_t, GuidHasher> container_type;
public:
	GLoadCallback();
	void RegisterCall(const GUID& guid, GLoadCall_t call);
	void UnregisterCall(const GUID& guid);
	void RegisterUnknownCall(GLoadCall_t call);
	void UnregisterUnknownCall();
	void Excute(const GUID& guid, ID3DXFileData* pFileData);

private:
	std::unordered_map<GUID, GLoadCall_t, GuidHasher> m_CallMap;		//�ص�ӳ���
	GLoadCall_t	m_UnknownCall;											//δ֪���ͻص�
};

struct GLoadXFile: public GLoadCallback
{
public:
	GLoadXFile();
	//�����ȡ
	void LoadMesh(ID3DXFileData* pFileData, IDirect3DDevice9* pDevice, ID3DXMesh** pNewMesh);

private:
	std::unordered_map<void*, ID3DXMesh*>		m_pMeshMap;
};


class XFileUtils{
public:
	typedef std::unordered_map<std::string, GUID> container_type;
public:
	static XFileUtils* getInstance();
	static void destroyInstance();
private:
	static XFileUtils* m_instance;
	XFileUtils();
	~XFileUtils();
	XFileUtils(const XFileUtils&);
	XFileUtils(XFileUtils&&);
public:
	//�õ�GUIDֵ
	const GUID& getTemplateGuid(const char* strTemplate) const;
	//ͨ��GUID�õ��ַ���
	const std::string& getStringFromGuid(const GUID& guid) const;
	//��ӡ�����нڵ�
	void PrintAll(ID3DXFileEnumObject* pObject, std::ostream& os);

	//��ȡ�ļ�
	bool LoadXFile(const char* path, ID3DXFileEnumObject** pNewObject) const;

	//�����ڵ�
	void LoadChild(GLoadCallback& call, ID3DXFileEnumObject* pObject);

	//��GUID�����ڵ�
	ID3DXFileData* SearchChild(const GUID& guid, ID3DXFileData* pFileData, size_t offset = 0);

	//��GUIDͳ�Ƹ����ͺ�������
	size_t GetChildCountByGuid(const GUID& guid, ID3DXFileData* pFileData);

	//��ȡһ����̬ģ��
	bool LoadMesh(
		ID3DXFileData*		pFileData, 
		IDirect3DDevice9*	pDevice, 
		ID3DXMesh**			pNewMesh, 
		ID3DXBuffer**		pMaterial,
		DWORD*				pNumAttribute,
		GSkinInfo**			pNewSkininfo
		);

	//�ڵ����ͼ��
	bool CheckType(const GUID& guid, ID3DXFileData* pFileData) const;

	//��ȡ��������
	bool LoadMeshContainer(
		ID3DXFileData* pFileData, 
		IDirect3DDevice9* pDevice,
		GMeshContainer** pNewMeshContainer);

	//��ȡ���
	bool LoadFrame(
		ID3DXFileData* pFileData,
		IDirect3DDevice9* pDevice,
		GFrame** pNewFrame
		);

	//��ȡ�������
	bool LoadFrameHierarchy(
		ID3DXFileEnumObject* pFileData,
		IDirect3DDevice9* pDevice,
		GFrameContainer** pNewFrameContainer
		);

private:
	/*
	*	���������������(�������������);
	*	����2��, ��һ�λ�ö���������, �ڶ��λ�ȡ��������(��Ҫ�ȼ���);
	*/
	bool ParseBasemesh(
		ID3DXFileData* pFileData,			//�����ļ��ڵ�
		DWORD* pNVertices,					//����������
		DWORD* pNFaces,						//��������
		void* pVertices,					//�������ݸ�ֵ
		DWORD* pIndex,						//�����������
		size_t verStride					//������
		);
	//��������
	bool ParseNormals(
		ID3DXFileData* pFileData,			//�����ļ��ڵ�
		DWORD nVertices,					//�������
		void* pVertices,					//�������ݸ�ֵ
		const DWORD* pIndex,				//�������ݲ�ѯ
		size_t verStride,					//������
		size_t normalOffset				//�������� ƫ��
		);
	//������������
	bool ParseTexcoord(
		ID3DXFileData* pFileData,			//�����ļ��ڵ�
		void* pVertices,					//�������ݸ�ֵ
		size_t verStride,					//������
		size_t texOffset					//�������� ƫ��
		);
	//������������
	bool ParseMaterial(
		ID3DXFileData* pFileData,			//�����ļ��ڵ�
		DWORD* pAttributes,				//���Ի�������ֵ
		DWORD  nFaces,						//�����(���Ի���������)
		DWORD* nMaterials,					//���Ը���
		ID3DXBuffer** ppMaterial			//���ʻ���
		);
	//������Ƥ��Ϣ
	bool ParseSkinInfo(
		ID3DXFileData* pFileData,			//�����ļ��ڵ�
		GSkinInfo** pNewSkininfo			//��Ƥ��Ϣ�ӿ�
		);

public:

	//������������
	bool ParseAnimationController(
		ID3DXFileEnumObject* pFileData,					//�ļ�
		GAnimationController** pNewAnimationController	//����������
		);

	//������
	bool ParseAnimationSet(
		ID3DXFileData* pFileData,							//�ļ��ڵ�
		GAnimationSet** pNewAnimationSet					//���������
		);

	bool ParseAnimation(
		ID3DXFileData* pFileData,							//�ļ��ڵ�
		GAnimation** pNewAnimation							//����
		);
private:
	std::string								m_strTemplate;		//ģ�������ַ���
	std::unordered_map<std::string, GUID>	m_GuidMap;			//ģ��GUID��
	ID3DXFile*								m_pFile;			//DX�ļ����ʽӿ�
	std::vector<ID3DXFileData*>				m_LoadStack;		//�����õ�ջ
	std::string								m_UnknownString;	//δ֪�����ַ���
};
#endif
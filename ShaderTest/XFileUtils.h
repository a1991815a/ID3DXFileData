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
	std::unordered_map<GUID, GLoadCall_t, GuidHasher> m_CallMap;		//回调映射表
	GLoadCall_t	m_UnknownCall;											//未知类型回调
};

struct GLoadXFile: public GLoadCallback
{
public:
	GLoadXFile();
	//网格读取
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
	//得到GUID值
	const GUID& getTemplateGuid(const char* strTemplate) const;
	//通过GUID得到字符串
	const std::string& getStringFromGuid(const GUID& guid) const;
	//打印出所有节点
	void PrintAll(ID3DXFileEnumObject* pObject, std::ostream& os);

	//读取文件
	bool LoadXFile(const char* path, ID3DXFileEnumObject** pNewObject) const;

	//遍历节点
	void LoadChild(GLoadCallback& call, ID3DXFileEnumObject* pObject);

	//按GUID搜索节点
	ID3DXFileData* SearchChild(const GUID& guid, ID3DXFileData* pFileData, size_t offset = 0);

	//按GUID统计该类型孩子数量
	size_t GetChildCountByGuid(const GUID& guid, ID3DXFileData* pFileData);

	//读取一个静态模型
	bool LoadMesh(
		ID3DXFileData*		pFileData, 
		IDirect3DDevice9*	pDevice, 
		ID3DXMesh**			pNewMesh, 
		ID3DXBuffer**		pMaterial,
		DWORD*				pNumAttribute,
		GSkinInfo**			pNewSkininfo
		);

	//节点类型检查
	bool CheckType(const GUID& guid, ID3DXFileData* pFileData) const;

	//读取网格容器
	bool LoadMeshContainer(
		ID3DXFileData* pFileData, 
		IDirect3DDevice9* pDevice,
		GMeshContainer** pNewMeshContainer);

	//读取框架
	bool LoadFrame(
		ID3DXFileData* pFileData,
		IDirect3DDevice9* pDevice,
		GFrame** pNewFrame
		);

	//读取框架容器
	bool LoadFrameHierarchy(
		ID3DXFileEnumObject* pFileData,
		IDirect3DDevice9* pDevice,
		GFrameContainer** pNewFrameContainer
		);

private:
	/*
	*	解析网格基本数据(顶点坐标和索引);
	*	调用2次, 第一次获得顶点和面个数, 第二次获取顶点数据(需要先加锁);
	*/
	bool ParseBasemesh(
		ID3DXFileData* pFileData,			//网格文件节点
		DWORD* pNVertices,					//顶点个数输出
		DWORD* pNFaces,						//面个数输出
		void* pVertices,					//顶点数据赋值
		DWORD* pIndex,						//索引数据输出
		size_t verStride					//顶点跨度
		);
	//解析法线
	bool ParseNormals(
		ID3DXFileData* pFileData,			//网格文件节点
		DWORD nVertices,					//顶点个数
		void* pVertices,					//顶点数据赋值
		const DWORD* pIndex,				//索引数据查询
		size_t verStride,					//顶点跨度
		size_t normalOffset				//法线数据 偏移
		);
	//解析纹理坐标
	bool ParseTexcoord(
		ID3DXFileData* pFileData,			//网格文件节点
		void* pVertices,					//顶点数据赋值
		size_t verStride,					//顶点跨度
		size_t texOffset					//纹理坐标 偏移
		);
	//解析材质数组
	bool ParseMaterial(
		ID3DXFileData* pFileData,			//网格文件节点
		DWORD* pAttributes,				//属性缓冲区赋值
		DWORD  nFaces,						//面个数(属性缓冲区长度)
		DWORD* nMaterials,					//属性个数
		ID3DXBuffer** ppMaterial			//材质缓冲
		);
	//解析蒙皮信息
	bool ParseSkinInfo(
		ID3DXFileData* pFileData,			//网格文件节点
		GSkinInfo** pNewSkininfo			//蒙皮信息接口
		);

public:

	//解析动画数据
	bool ParseAnimationController(
		ID3DXFileEnumObject* pFileData,					//文件
		GAnimationController** pNewAnimationController	//动画控制器
		);

	//动画集
	bool ParseAnimationSet(
		ID3DXFileData* pFileData,							//文件节点
		GAnimationSet** pNewAnimationSet					//输出动画集
		);

	bool ParseAnimation(
		ID3DXFileData* pFileData,							//文件节点
		GAnimation** pNewAnimation							//动画
		);
private:
	std::string								m_strTemplate;		//模板声明字符串
	std::unordered_map<std::string, GUID>	m_GuidMap;			//模板GUID表
	ID3DXFile*								m_pFile;			//DX文件访问接口
	std::vector<ID3DXFileData*>				m_LoadStack;		//加载用的栈
	std::string								m_UnknownString;	//未知类型字符串
};
#endif
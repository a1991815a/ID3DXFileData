#ifndef __XFILESTRUCT__
#define __XFILESTRUCT__
#include <d3d9.h>
#include <d3dx9.h>
#include <string>
#include "GSkinInfo.h"

#pragma pack(push)
#pragma pack(1)

struct XVertex_p3_n3{
	float x, y, z;
	float nx, ny, nz;

	D3DXVECTOR3& getNormal(){
		return *(D3DXVECTOR3*)&nx;
	};

	D3DXVECTOR3& getPosition() {
		return *(D3DXVECTOR3*)&x;
	}

	void setPosition(const D3DXVECTOR3* pos){
		*(D3DXVECTOR3*)&x = *pos;
	};

	void setNormal(const D3DXVECTOR3* normal){
		*(D3DXVECTOR3*)&nx = *normal;
	};

	static const DWORD fvf = D3DFVF_XYZ | D3DFVF_NORMAL;
};

struct XVertex_p3_n3_t1{
	float x, y, z;
	float nx, ny, nz;
	float u, v;

	D3DXVECTOR3& getPosition() {
		return *(D3DXVECTOR3*)&x;
	}

	void setPosition(const D3DXVECTOR3* pos){
		*(D3DXVECTOR3*)&x = *pos;
	};

	void setNormal(const D3DXVECTOR3* normal){
		*(D3DXVECTOR3*)&nx = *normal;
	};

	D3DXVECTOR3& getNormal(){
		return *(D3DXVECTOR3*)&nx;
	};
	static const DWORD fvf = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
};

struct XVector {
	float x, y, z;
};

//RGB
struct XColorRGB{
	float red;
	float green;
	float blue;
	D3DXCOLOR getD3DXColor() const {
		return D3DXCOLOR(red, green, blue, 1.0f);
	};
};

//RGBA
struct XColorRGBA{
	float red;
	float green;
	float blue;
	float alpha;
	D3DXCOLOR getD3DXColor() const{
		return D3DXCOLOR(red, green, blue, alpha);
	};
};

//索引颜色
struct XIndexedColor{
	DWORD		index;								//对应的索引
	XColorRGBA	indexColor;						//颜色
};

//纹理坐标
struct XCoords2d{
	float u, v;									//纹理坐标
};

//网格面
struct XMeshFace{
	DWORD			nFaceVertexIndices;			//索引个数
	DWORD			faceVertexIndices[3];		//索引数组
};

//纹理文件名
struct XTextureFilename{
	const char* filename;						//纹理路径
};

//材质
struct XMaterial{
	XColorRGBA faceColor;						//漫反射光
	float power;								//镜面Power
	XColorRGB specularColor;					//镜面光
	XColorRGB emissiveColor;					//自发光

	void GetD3DMATERIAL9(D3DMATERIAL9* material) const;
};

//材质列表
struct XMeshMaterialList {
	DWORD nMaterials;
	DWORD nFaceIndexes;
};

//网格容器
struct GMeshContainer {
	GMeshContainer() 
		:Name(), pMesh(nullptr), pMaterials(nullptr), NumMaterials(0),
		pAdjacency(nullptr), pSkinInfo(nullptr), pNextMeshContainer(nullptr)
	{};
	std::string		Name;					//网格名
	ID3DXMesh*		pMesh;					//网格
	ID3DXBuffer*	pMaterials;				//材质信息
	DWORD			NumMaterials;			//材质个数
	DWORD*			pAdjacency;				//临近信息
	GSkinInfo*		pSkinInfo;				//蒙皮信息

	GMeshContainer*	pNextMeshContainer;		//下一个网格
};

//框架
struct GFrame {
	GFrame()
		:Name(), 
		TransformationMatrix(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1),
		pMeshContainer(nullptr), pFrameSibling(nullptr), pFrameFirstChild(nullptr)
	{};

	void DebugPrint(std::ostream& os, size_t depth = 0) const;

	std::string		Name;						//框架名
	D3DXMATRIX		TransformationMatrix;		//变换矩阵
	GMeshContainer*	pMeshContainer;				//网格容器
	GFrame*			pFrameSibling;				//兄弟
	GFrame*			pFrameFirstChild;			//孩子
};

#pragma pack(pop)

#define GROOTFRAME "GRootFrame"

//框架容器
class GFrameContainer {
	friend class XFileUtils;
private:
	bool InsertFrame(GFrame* pFrame);
public:
	typedef std::unordered_map<std::string, GFrame*> map_container;
	typedef std::vector<GFrame*> vector_container;
public:
	//得到顶层框架
	GFrame* getRootFrame() const;
	//得到框架
	GFrame* getFrameByName(const std::string& name) const;
	//得到框架个数
	size_t getFrameCount() const;

	//遍历框架
	map_container::iterator begin();
	map_container::iterator end();
	map_container::const_iterator begin() const;
	map_container::const_iterator end() const;
private:
	std::unordered_map<std::string, GFrame*>	m_FrameMap;		//框架表
	GFrame*										m_pRootFrames;	//顶层节点框架
};

#endif
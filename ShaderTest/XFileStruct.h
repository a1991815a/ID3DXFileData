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

//������ɫ
struct XIndexedColor{
	DWORD		index;								//��Ӧ������
	XColorRGBA	indexColor;						//��ɫ
};

//��������
struct XCoords2d{
	float u, v;									//��������
};

//������
struct XMeshFace{
	DWORD			nFaceVertexIndices;			//��������
	DWORD			faceVertexIndices[3];		//��������
};

//�����ļ���
struct XTextureFilename{
	const char* filename;						//����·��
};

//����
struct XMaterial{
	XColorRGBA faceColor;						//�������
	float power;								//����Power
	XColorRGB specularColor;					//�����
	XColorRGB emissiveColor;					//�Է���

	void GetD3DMATERIAL9(D3DMATERIAL9* material) const;
};

//�����б�
struct XMeshMaterialList {
	DWORD nMaterials;
	DWORD nFaceIndexes;
};

//��������
struct GMeshContainer {
	GMeshContainer() 
		:Name(), pMesh(nullptr), pMaterials(nullptr), NumMaterials(0),
		pAdjacency(nullptr), pSkinInfo(nullptr), pNextMeshContainer(nullptr)
	{};
	std::string		Name;					//������
	ID3DXMesh*		pMesh;					//����
	ID3DXBuffer*	pMaterials;				//������Ϣ
	DWORD			NumMaterials;			//���ʸ���
	DWORD*			pAdjacency;				//�ٽ���Ϣ
	GSkinInfo*		pSkinInfo;				//��Ƥ��Ϣ

	GMeshContainer*	pNextMeshContainer;		//��һ������
};

//���
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

	std::string		Name;						//�����
	D3DXMATRIX		TransformationMatrix;		//�任����
	GMeshContainer*	pMeshContainer;				//��������
	GFrame*			pFrameSibling;				//�ֵ�
	GFrame*			pFrameFirstChild;			//����
};

#pragma pack(pop)

#define GROOTFRAME "GRootFrame"

//�������
class GFrameContainer {
	friend class XFileUtils;
private:
	bool InsertFrame(GFrame* pFrame);
public:
	typedef std::unordered_map<std::string, GFrame*> map_container;
	typedef std::vector<GFrame*> vector_container;
public:
	//�õ�������
	GFrame* getRootFrame() const;
	//�õ����
	GFrame* getFrameByName(const std::string& name) const;
	//�õ���ܸ���
	size_t getFrameCount() const;

	//�������
	map_container::iterator begin();
	map_container::iterator end();
	map_container::const_iterator begin() const;
	map_container::const_iterator end() const;
private:
	std::unordered_map<std::string, GFrame*>	m_FrameMap;		//��ܱ�
	GFrame*										m_pRootFrames;	//����ڵ���
};

#endif
#ifndef __GSKININFO__
#define __GSKININFO__

#include <string>
#include <wtypes.h>
#include <d3dx9math.h>
#include <unordered_map>

//骨骼
class GSkinBone {
	friend class GSkinInfo;
	friend class XFileUtils;
private:
	GSkinBone();
	~GSkinBone();
public:
	//得到骨骼名称
	const std::string& getBoneName() const;
	//得到骨骼包含的顶点数
	DWORD getNumVertices() const;
	//得到顶点数组
	const DWORD* getVertexIndices() const;
	//得到权重数组
	const float* getWeights() const;
	//得到骨骼空间变换矩阵
	const D3DXMATRIX& getMatrixOffset() const;
public:
	void DebugPrint(std::ostream& os) const;
	void DebugPrintDescribe(std::ostream& os) const;
private:
	std::string		transformNodeName;		//骨骼名
	DWORD			nWeights;				//权重数
	DWORD*			vertexIndices;			//受影响的顶点索引数组
	float*			weights;				//权重数组
	D3DXMATRIX		matrixOffset;			//顶点->骨骼空间  矩阵
};

//蒙皮信息
class GSkinInfo {
	friend class XFileUtils;
private:
	GSkinInfo();
	~GSkinInfo();
public:
	typedef std::unordered_map<std::string, GSkinBone*>::iterator		iterator;
	typedef std::unordered_map<std::string, GSkinBone*>::const_iterator const_iterator;
	//得到骨骼
	const GSkinBone* getBone(const std::string& name) const;
	//得到骨骼数
	size_t getNumBone() const;
	//得到最大顶点骨骼共享数
	size_t getMaxSkinWeightsPerVertex() const;
	
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;

public:
	void DebugPrint(std::ostream& os) const;
private:
	std::unordered_map<std::string, GSkinBone*> m_pBoneMap;

	size_t m_nMaxSkinWeightsPerVertex;					//最大顶点骨骼共享数
	size_t m_nMaxSkinWeightsPerFace;					//保留值.. = 0
	size_t m_nBones;									//骨骼数
};
#endif
#ifndef __GSKININFO__
#define __GSKININFO__

#include <string>
#include <wtypes.h>
#include <d3dx9math.h>
#include <unordered_map>

//����
class GSkinBone {
	friend class GSkinInfo;
	friend class XFileUtils;
private:
	GSkinBone();
	~GSkinBone();
public:
	//�õ���������
	const std::string& getBoneName() const;
	//�õ����������Ķ�����
	DWORD getNumVertices() const;
	//�õ���������
	const DWORD* getVertexIndices() const;
	//�õ�Ȩ������
	const float* getWeights() const;
	//�õ������ռ�任����
	const D3DXMATRIX& getMatrixOffset() const;
public:
	void DebugPrint(std::ostream& os) const;
	void DebugPrintDescribe(std::ostream& os) const;
private:
	std::string		transformNodeName;		//������
	DWORD			nWeights;				//Ȩ����
	DWORD*			vertexIndices;			//��Ӱ��Ķ�����������
	float*			weights;				//Ȩ������
	D3DXMATRIX		matrixOffset;			//����->�����ռ�  ����
};

//��Ƥ��Ϣ
class GSkinInfo {
	friend class XFileUtils;
private:
	GSkinInfo();
	~GSkinInfo();
public:
	typedef std::unordered_map<std::string, GSkinBone*>::iterator		iterator;
	typedef std::unordered_map<std::string, GSkinBone*>::const_iterator const_iterator;
	//�õ�����
	const GSkinBone* getBone(const std::string& name) const;
	//�õ�������
	size_t getNumBone() const;
	//�õ���󶥵����������
	size_t getMaxSkinWeightsPerVertex() const;
	
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;

public:
	void DebugPrint(std::ostream& os) const;
private:
	std::unordered_map<std::string, GSkinBone*> m_pBoneMap;

	size_t m_nMaxSkinWeightsPerVertex;					//��󶥵����������
	size_t m_nMaxSkinWeightsPerFace;					//����ֵ.. = 0
	size_t m_nBones;									//������
};
#endif
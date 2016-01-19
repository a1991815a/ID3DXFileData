#include "GSkinInfo.h"



GSkinBone::GSkinBone()
	:transformNodeName(), nWeights(0), vertexIndices(nullptr), weights(nullptr),
	matrixOffset(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		)
{

}

GSkinBone::~GSkinBone()
{
	if (vertexIndices)
		delete[] vertexIndices;
	if (weights)
		delete[] weights;
}

const std::string& GSkinBone::getBoneName() const
{
	return transformNodeName;
}

DWORD GSkinBone::getNumVertices() const
{
	return nWeights;
}

const DWORD* GSkinBone::getVertexIndices() const
{
	return vertexIndices;
}

const float* GSkinBone::getWeights() const
{
	return weights;
}

const D3DXMATRIX& GSkinBone::getMatrixOffset() const
{
	return matrixOffset;
}

void GSkinBone::DebugPrint(std::ostream& os) const
{
	os << "¹Ç÷ÀÃû:" << transformNodeName.c_str() << "  ¶¥µã:" << nWeights << std::endl;
	os << "¾ØÕó:";
	for (size_t col = 0; col < 4; ++col)
		for (size_t row = 0; row < 4; ++row)
			os << matrixOffset.m[row][col] << ",";
	os << std::endl;
}

void GSkinBone::DebugPrintDescribe(std::ostream& os) const
{
	DebugPrint(os);
}

GSkinInfo::GSkinInfo()
	:m_pBoneMap(), m_nMaxSkinWeightsPerVertex(4), m_nMaxSkinWeightsPerFace(0),
	m_nBones(0)
{

}

GSkinInfo::~GSkinInfo()
{
	for (auto itor = m_pBoneMap.begin(); itor != m_pBoneMap.end(); ++itor)
		delete itor->second;
}

const GSkinBone* GSkinInfo::getBone(const std::string& name) const
{
	return m_pBoneMap.at(name);
}

size_t GSkinInfo::getNumBone() const
{
	return m_nBones;
}

size_t GSkinInfo::getMaxSkinWeightsPerVertex() const
{
	return m_nMaxSkinWeightsPerVertex;
}

GSkinInfo::iterator GSkinInfo::begin()
{
	return m_pBoneMap.begin();
}

GSkinInfo::const_iterator GSkinInfo::begin() const
{
	return m_pBoneMap.begin();
}

GSkinInfo::iterator GSkinInfo::end()
{
	return m_pBoneMap.end();
}

GSkinInfo::const_iterator GSkinInfo::end() const
{
	return m_pBoneMap.end();
}

void GSkinInfo::DebugPrint(std::ostream& os) const
{
	auto itor = m_pBoneMap.begin();
	for (; itor != m_pBoneMap.end(); ++itor)
		itor->second->DebugPrint(os);
}

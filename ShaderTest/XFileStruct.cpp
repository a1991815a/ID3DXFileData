#include "XFileStruct.h"
#include "XFileUtils.h"

void XMaterial::GetD3DMATERIAL9(D3DMATERIAL9* material) const
{
	material->Ambient = material->Diffuse = faceColor.getD3DXColor();
	material->Emissive = emissiveColor.getD3DXColor();
	material->Specular = specularColor.getD3DXColor();
	material->Power = power;
}

bool GFrameContainer::InsertFrame(GFrame* pFrame)
{
	auto itor = m_FrameMap.find(pFrame->Name);
	if (itor != m_FrameMap.end())
		return false;
	m_FrameMap.insert(map_container::value_type(pFrame->Name, pFrame));
	return true;
}

GFrame* GFrameContainer::getRootFrame() const
{
	return m_pRootFrames;
}

GFrame* GFrameContainer::getFrameByName(const std::string& name) const
{
	auto itor = m_FrameMap.find(name);
	if (itor == m_FrameMap.end())
		return nullptr;
	return itor->second;
}


size_t GFrameContainer::getFrameCount() const
{
	return m_FrameMap.size();
}

GFrameContainer::map_container::iterator GFrameContainer::begin()
{
	return m_FrameMap.begin();
}

GFrameContainer::map_container::const_iterator GFrameContainer::begin() const
{
	return m_FrameMap.begin();
}

GFrameContainer::map_container::iterator GFrameContainer::end()
{
	return m_FrameMap.end();
}

GFrameContainer::map_container::const_iterator GFrameContainer::end() const
{
	return m_FrameMap.end();
}

void GFrame::DebugPrint(std::ostream& os, size_t depth /*= 0 */) const
{
	for (size_t i = 0; i < depth; ++i)
		os << "  ";
	os << "Frame " << Name.c_str() << std::endl;

	for (size_t i = 0; i < depth; ++i)
		os << "  ";
	os << "{" << std::endl;
	for (size_t i = 0; i < depth + 1; ++i)
		os << "  ";
	os << "TransformationMatrix: ";
	for (size_t col = 0; col < 4; ++col)
		for (size_t row = 0; row < 4; ++row)
			os << TransformationMatrix.m[row][col] << ",";
	os << std::endl;

	if (pMeshContainer)
	{
		for (size_t i = 0; i < depth + 1; ++i)
			os << "  ";
		os << "Mesh " << pMeshContainer->Name.c_str() << std::endl;
	}

	if (pFrameFirstChild)
		pFrameFirstChild->DebugPrint(os, depth + 1);
	
	for (size_t i = 0; i < depth; ++i)
		os << "  ";
	os << "}" << std::endl;

	if (pFrameSibling)
		pFrameSibling->DebugPrint(os, depth);
}

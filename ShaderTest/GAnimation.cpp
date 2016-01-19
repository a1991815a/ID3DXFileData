#include "GAnimation.h"

DWORD GAnimationKey::getKeyType() const
{
	return m_keyType;
}

DWORD GAnimationKey::getNumKey() const
{
	return m_nKeys;
}

const GTimedFloatKeys* GAnimationKey::getKeys() const
{
	return m_keys;
}

void GAnimationKey::DebugPrint(std::ostream& os)
{
	os << "KeyType:" << m_keyType << std::endl;
	os << "nKeys:" << m_nKeys << std::endl;
	for (size_t i = 0; i < m_nKeys; ++i)
	{
		os << m_keys[i].time << ";" << m_keys[i].nValues << ";";
		for (size_t col = 0; col < 4; ++col)
			for (size_t row = 0; row < 4; ++row)
				os << m_keys[i].Matrix.m[row][col] << ",";
		os << std::endl;
	}
}

const std::string& GAnimation::getName() const
{
	return m_Name;
}

const std::string& GAnimation::getBindBoneName() const
{
	return m_pBoneName;
}

void GAnimation::getAnimationOptions(GAnimationOptions* pOptions) const
{
	*pOptions = m_AnimationOptions;
}

GAnimationKey* GAnimation::getAnimationKey() const
{
	return m_pAnimationKey;
}

void GAnimation::DebugPrint(std::ostream& os)
{
	os << "  {" << std::endl;
	os << "Animation " << m_Name.c_str() << std::endl;
	os << "BindBone: " << m_pBoneName.c_str() << std::endl;
	os << "Options:" << std::endl;
	os << "  openclosed:" << m_AnimationOptions.openclosed << std::endl;
	os << "  positionquality:" << m_AnimationOptions.positionquality << std::endl;
	if(m_pAnimationKey)
		m_pAnimationKey->DebugPrint(os);
	os << "  }" << std::endl;
}

GAnimationSet::~GAnimationSet()
{
	for (auto itor = m_AnimationVector.begin(); itor != m_AnimationVector.end(); ++itor)
		delete *itor;
}

const std::string& GAnimationSet::getName() const
{
	return m_Name;
}

GAnimation* GAnimationSet::getAnimationByName(const std::string& name) const
{
	auto itor = m_AnimationVector.begin();
	for (; itor != m_AnimationVector.end(); ++itor)
		if ((*itor)->m_Name == name)
			return *itor;
	return nullptr;
}

size_t GAnimationSet::getAnimationCount() const
{
	return m_AnimationVector.size();
}

GAnimation* GAnimationSet::getAnimationByIndex(size_t index) const
{
	return m_AnimationVector.at(index);
}

void GAnimationSet::DebugPrint(std::ostream& os)
{
	os << "AnimationSet " << m_Name.c_str() << " : " << std::endl;
	os << "{" << std::endl;
	auto itor = m_AnimationVector.begin();
	for (; itor != m_AnimationVector.end(); ++itor)
	{
		(*itor)->DebugPrint(os);
		os << std::endl;
	}
	os << "}" << std::endl;
}

GAnimationController::GAnimationController()
{

}

bool GAnimationController::InsertAnimationSet(GAnimationSet* pAnimationSet)
{
	if (m_AnimationSetMap.find(pAnimationSet->m_Name) != m_AnimationSetMap.end())
		return false;
	m_AnimationSetMap.insert(
		std::unordered_map<std::string, GAnimationSet*>::value_type(
			pAnimationSet->m_Name, pAnimationSet
			)
		);
	return true;
}

GAnimationController::~GAnimationController()
{
	auto itor = m_AnimationSetMap.begin();
	for (; itor != m_AnimationSetMap.end(); ++itor)
		delete itor->second;
}

size_t GAnimationController::getAnimationSetCount() const
{
	return m_AnimationSetMap.size();
}

GAnimationSet* GAnimationController::getAnimationSetByName(const std::string& name) const
{
	return m_AnimationSetMap.at(name);
}

void GAnimationController::DebugPrint(std::ostream& os)
{
	auto itor = m_AnimationSetMap.begin();
	for (; itor != m_AnimationSetMap.end(); ++itor)
	{
		itor->second->DebugPrint(os);
		os << std::endl;
	}
}

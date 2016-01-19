#ifndef __GANIMATION__
#define __GANIMATION__
#include <string>
#include "XFileStruct.h"

//动画帧数据
struct GTimedFloatKeys {
	GTimedFloatKeys() 
		:time(0), nValues(0), 
		Matrix(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1)
	{};
	DWORD time;						//帧时间
	DWORD nValues;					//浮点数个数(没使用)
	D3DXMATRIX Matrix;				//动画变换矩阵
};

//动画设置
struct GAnimationOptions {
	GAnimationOptions() 
		:openclosed(0), positionquality(0)
	{};
	DWORD openclosed;				//是否是闭合动画 0为闭合， 1为开放动画; 默认0
	DWORD positionquality;			//动画的质量;; 默认0
};

//动画键(包含多个动画帧，和 动画变换矩阵)
class GAnimationKey {
	friend class GAnimation;
	friend class XFileUtils;
private:
	GAnimationKey() 
		:m_keyType(0), m_nKeys(0), m_keys(0)
	{};
	~GAnimationKey() 
	{
		if (m_keys)
			delete[] m_keys;
	};
public:
	//得到动画键类型
	DWORD getKeyType() const;
	//得到动画帧个数
	DWORD getNumKey() const;
	//得到动画帧数组
	const GTimedFloatKeys* getKeys() const;

	//调试信息
	void DebugPrint(std::ostream& os);
private:
	DWORD m_keyType;					//动画键类型
	DWORD m_nKeys;						//动画帧 个数
	GTimedFloatKeys* m_keys;			//动画帧 数组
};

//动画(针对某个骨骼的动画)
class GAnimation{
	friend class GAnimationSet;
	friend class XFileUtils;
private:
	GAnimation() 
		:m_Name(), m_pBoneName(), m_AnimationOptions()
	{};
	~GAnimation() {
		delete m_pAnimationKey;
	};
public:
	//得到动画名
	const std::string& getName() const;
	//得到绑定框架(骨骼)
	const std::string& getBindBoneName() const;
	//得到动画设置
	void getAnimationOptions(GAnimationOptions* pOptions) const;
	//得到动画帧数据
	GAnimationKey* getAnimationKey() const;

	//调试信息
	void DebugPrint(std::ostream& os);
private:
	std::string			m_Name;					//动画 名字
	std::string			m_pBoneName;			//动画所绑定的 骨骼框架名字
	GAnimationOptions	m_AnimationOptions;		//动画设置
	GAnimationKey*		m_pAnimationKey;		//动画帧数据
};

//动画集(包含许多骨骼动画的 动画集)
class GAnimationSet {
	friend class GAnimationController;
	friend class XFileUtils;
private:
	~GAnimationSet();
public:
	//得到动画集名字
	const std::string& getName() const;
	//得到动画
	GAnimation* getAnimationByName(const std::string& name) const;
	//得到动画数量
	size_t getAnimationCount() const;
	//按索引得到动画
	GAnimation* getAnimationByIndex(size_t index) const;

	//调试信息
	void DebugPrint(std::ostream& os);
private:
	std::string								m_Name;						//动画集 名字
	std::vector<GAnimation*>				m_AnimationVector;			//动画 数组
};

//动画控制器(包含许多动画集的控制器)
class GAnimationController {
	friend class XFileUtils;
private:
	GAnimationController();
	bool InsertAnimationSet(GAnimationSet* pAnimationSet);
public:
	~GAnimationController();
	//得到动画集总数
	size_t getAnimationSetCount() const;
	//得到动画集
	GAnimationSet* getAnimationSetByName(const std::string& name) const;

	//调试信息
	void DebugPrint(std::ostream& os);
private:
	std::unordered_map<std::string, GAnimationSet*>		m_AnimationSetMap;	//动画集容器
};

#endif
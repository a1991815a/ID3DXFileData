#ifndef __GANIMATION__
#define __GANIMATION__
#include <string>
#include "XFileStruct.h"

//����֡����
struct GTimedFloatKeys {
	GTimedFloatKeys() 
		:time(0), nValues(0), 
		Matrix(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1)
	{};
	DWORD time;						//֡ʱ��
	DWORD nValues;					//����������(ûʹ��)
	D3DXMATRIX Matrix;				//�����任����
};

//��������
struct GAnimationOptions {
	GAnimationOptions() 
		:openclosed(0), positionquality(0)
	{};
	DWORD openclosed;				//�Ƿ��Ǳպ϶��� 0Ϊ�պϣ� 1Ϊ���Ŷ���; Ĭ��0
	DWORD positionquality;			//����������;; Ĭ��0
};

//������(�����������֡���� �����任����)
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
	//�õ�����������
	DWORD getKeyType() const;
	//�õ�����֡����
	DWORD getNumKey() const;
	//�õ�����֡����
	const GTimedFloatKeys* getKeys() const;

	//������Ϣ
	void DebugPrint(std::ostream& os);
private:
	DWORD m_keyType;					//����������
	DWORD m_nKeys;						//����֡ ����
	GTimedFloatKeys* m_keys;			//����֡ ����
};

//����(���ĳ�������Ķ���)
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
	//�õ�������
	const std::string& getName() const;
	//�õ��󶨿��(����)
	const std::string& getBindBoneName() const;
	//�õ���������
	void getAnimationOptions(GAnimationOptions* pOptions) const;
	//�õ�����֡����
	GAnimationKey* getAnimationKey() const;

	//������Ϣ
	void DebugPrint(std::ostream& os);
private:
	std::string			m_Name;					//���� ����
	std::string			m_pBoneName;			//�������󶨵� �����������
	GAnimationOptions	m_AnimationOptions;		//��������
	GAnimationKey*		m_pAnimationKey;		//����֡����
};

//������(���������������� ������)
class GAnimationSet {
	friend class GAnimationController;
	friend class XFileUtils;
private:
	~GAnimationSet();
public:
	//�õ�����������
	const std::string& getName() const;
	//�õ�����
	GAnimation* getAnimationByName(const std::string& name) const;
	//�õ���������
	size_t getAnimationCount() const;
	//�������õ�����
	GAnimation* getAnimationByIndex(size_t index) const;

	//������Ϣ
	void DebugPrint(std::ostream& os);
private:
	std::string								m_Name;						//������ ����
	std::vector<GAnimation*>				m_AnimationVector;			//���� ����
};

//����������(������ද�����Ŀ�����)
class GAnimationController {
	friend class XFileUtils;
private:
	GAnimationController();
	bool InsertAnimationSet(GAnimationSet* pAnimationSet);
public:
	~GAnimationController();
	//�õ�����������
	size_t getAnimationSetCount() const;
	//�õ�������
	GAnimationSet* getAnimationSetByName(const std::string& name) const;

	//������Ϣ
	void DebugPrint(std::ostream& os);
private:
	std::unordered_map<std::string, GAnimationSet*>		m_AnimationSetMap;	//����������
};

#endif
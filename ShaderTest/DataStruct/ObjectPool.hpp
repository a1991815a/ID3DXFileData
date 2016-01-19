#ifndef __OBJECTPOOL__
#define __OBJECTPOOL__
#include <vector>
#include <unordered_set>
#include <type_traits>

/**
*	�����������
*	ģ�����: ָ������(����: <int*>)
*/
template<typename _Ty>
class ObjectPool{
private:
	//�����������ƶ�����
	ObjectPool(const ObjectPool&);
	ObjectPool(ObjectPool&&);
public:
	typedef typename std::remove_pointer<_Ty>::type		value_type;			//�޳�ָ��
	typedef typename value_type*						pointer;			//ָ������
	typedef typename const value_type*					const_pointer;		//����ָ��
	typedef typename value_type&						reference;			//����
	typedef typename const value_type&					const_reference;	//��������
	typedef typename std::vector<_Ty>::iterator			iterator;			//������
	typedef typename std::vector<_Ty>::const_iterator	const_iterator;		//������
public:
	//���캯��
	ObjectPool(){};
	ObjectPool(size_t _size)
	{
		reserve(_size);
	};
	~ObjectPool(){
		clear();
	};

	//��֤���ж����ܸ���
	void reserve(size_t _size){
		for (size_t i = size(); i < _size; ++i)
		{
			_Ty val = (_Ty)malloc(sizeof(value_type));
			memset(val, 0, sizeof(value_type));
			m_vFreePoolVector.push_back(val);
		}
	};

	//�ܶ������
	size_t size() const{
		return m_sUsePoolSet.size() + m_vFreePoolVector.size();
	};

	//�����б���
	size_t use_size() const{
		return m_sUsePoolSet.size();
	};

	//�����б���
	size_t free_size() const{
		return m_vFreePoolVector.size();
	};

	//�������
	_Ty allocate(){
		if(m_vFreePoolVector.empty())
			return nullptr;
		_Ty val = m_vFreePoolVector.back();
		m_sUsePoolSet.insert(val);
		m_vFreePoolVector.pop_back();
		new (val) value_type();
		return val;
	};

	//���ն���
	void deallocate(_Ty val){
		auto itor = m_sUsePoolSet.find(val);
		if(itor == m_sUsePoolSet.end())
			return;
		(*itor)->~value_type();
		memset(*itor, 0, sizeof(value_type));
		m_vFreePoolVector.push_back(*itor);
		m_sUsePoolSet.erase(itor);
	};

	//�������ж���(�����ͷ��ڴ�)
	void freeAll() {
		auto itor = m_sUsePoolSet.begin();
		for (; itor != m_sUsePoolSet.end(); ++itor)
		{
			(*itor)->~value_type();
			memset(*itor, 0, sizeof(value_type));
			m_vFreePoolVector.push_back(*itor);
		}
		m_sUsePoolSet.clear();
	};

	//�������ж����ͷ������ڴ�
	void clear(){
		for (auto itor = m_sUsePoolSet.begin(); itor != m_sUsePoolSet.end(); ++itor)
			delete *itor;	//�����б���Ҫִ����������
		m_sUsePoolSet.clear();

		for (auto itor = m_vFreePoolVector.begin(); itor != m_vFreePoolVector.end(); ++itor)
			free(*itor);	//�����б���ִ������
		m_vFreePoolVector.clear();
	};

	//�����б��������
	typename iterator begin() {
		return m_sUsePoolSet.begin();
	};
	typename const_iterator begin() const {
		return m_sUsePoolSet.begin();
	};
	typename iterator end() {
		return m_sUsePoolSet.end();
	};
	typename const_iterator end() const {
		return m_sUsePoolSet.end();
	};


private:
	std::unordered_set<_Ty> m_sUsePoolSet;
	std::vector<_Ty> m_vFreePoolVector;
};

#endif
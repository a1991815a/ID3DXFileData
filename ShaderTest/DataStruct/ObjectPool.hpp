#ifndef __OBJECTPOOL__
#define __OBJECTPOOL__
#include <vector>
#include <unordered_set>
#include <type_traits>

/**
*	可生长对象池
*	模板参数: 指针类型(例如: <int*>)
*/
template<typename _Ty>
class ObjectPool{
private:
	//不允许拷贝和移动拷贝
	ObjectPool(const ObjectPool&);
	ObjectPool(ObjectPool&&);
public:
	typedef typename std::remove_pointer<_Ty>::type		value_type;			//剔除指针
	typedef typename value_type*						pointer;			//指针类型
	typedef typename const value_type*					const_pointer;		//常量指针
	typedef typename value_type&						reference;			//引用
	typedef typename const value_type&					const_reference;	//常量引用
	typedef typename std::vector<_Ty>::iterator			iterator;			//迭代器
	typedef typename std::vector<_Ty>::const_iterator	const_iterator;		//迭代器
public:
	//构造函数
	ObjectPool(){};
	ObjectPool(size_t _size)
	{
		reserve(_size);
	};
	~ObjectPool(){
		clear();
	};

	//保证池中对象总个数
	void reserve(size_t _size){
		for (size_t i = size(); i < _size; ++i)
		{
			_Ty val = (_Ty)malloc(sizeof(value_type));
			memset(val, 0, sizeof(value_type));
			m_vFreePoolVector.push_back(val);
		}
	};

	//总对象个数
	size_t size() const{
		return m_sUsePoolSet.size() + m_vFreePoolVector.size();
	};

	//在用列表长度
	size_t use_size() const{
		return m_sUsePoolSet.size();
	};

	//自由列表长度
	size_t free_size() const{
		return m_vFreePoolVector.size();
	};

	//分配对象
	_Ty allocate(){
		if(m_vFreePoolVector.empty())
			return nullptr;
		_Ty val = m_vFreePoolVector.back();
		m_sUsePoolSet.insert(val);
		m_vFreePoolVector.pop_back();
		new (val) value_type();
		return val;
	};

	//回收对象
	void deallocate(_Ty val){
		auto itor = m_sUsePoolSet.find(val);
		if(itor == m_sUsePoolSet.end())
			return;
		(*itor)->~value_type();
		memset(*itor, 0, sizeof(value_type));
		m_vFreePoolVector.push_back(*itor);
		m_sUsePoolSet.erase(itor);
	};

	//回收所有对象(并不释放内存)
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

	//回收所有对象并释放所有内存
	void clear(){
		for (auto itor = m_sUsePoolSet.begin(); itor != m_sUsePoolSet.end(); ++itor)
			delete *itor;	//在用列表需要执行析构函数
		m_sUsePoolSet.clear();

		for (auto itor = m_vFreePoolVector.begin(); itor != m_vFreePoolVector.end(); ++itor)
			free(*itor);	//自由列表不用执行析构
		m_vFreePoolVector.clear();
	};

	//再用列表迭代函数
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
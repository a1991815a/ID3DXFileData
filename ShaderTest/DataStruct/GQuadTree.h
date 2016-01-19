#ifndef __GQUADTREE__
#define __GQUADTREE__
#include <vector>
#include <stdexcept>
#include <functional>




template<typename _Ty>
class GQuadTree;

template<typename _Ty>
class GQuadNode {
	friend class GQuadTree<_Ty>;
public:
	GQuadNode() 
		:m_pChild1(nullptr), m_pChild2(nullptr), m_pChild3(nullptr), m_pChild4(nullptr),
		m_pParent(nullptr), m_uDepth(0), m_Data()
	{};

	GQuadNode<_Ty>* getParent() {
		return m_pParent;
	};
	GQuadNode<_Ty>* getSibling() {
		if (m_pParent == nullptr)
			return nullptr;
		for (size_t i = 0; i < 3; ++i)
			if (this == m_pParent->m_pChildren[i])
				return m_pParent->m_pChildren[i + 1];
		return nullptr;
	};
	GQuadNode<_Ty>* getFirstChild() {
		return m_pChild1;
	};
	GQuadNode<_Ty>* getChild(size_t index) {
		while (index > 3)
			throw std::range_error("getChild: out of range!");
		return m_pChildren[index];
	}
	//得到兄弟(可以是非亲兄弟)
	GQuadNode<_Ty>* getDepthSibling() {
		GQuadNode<_Ty>* node = getSibling();
		if (node != nullptr)
			return node;
		//亲兄弟遍历失败
		GQuadNode<_Ty>* parent = this;
		while ((parent = parent->m_pParent) != nullptr)
		{
			node = parent->getSibling();
			//如果没有父亲兄弟，则继续往上遍历
			if (node == nullptr)
				continue;
			//如果不为空并且 < 当前深度则继续遍历
			while (node != nullptr && node->m_uDepth < m_uDepth)
				node = node->getFirstChild();
			//如果等于当前深度则返回
			if (node->m_uDepth == m_uDepth)
				return node;
		}
		return nullptr;
	}

	const GQuadNode<_Ty>* getParent() const {
		return m_pParent;
	};
	const GQuadNode<_Ty>* getSibling() const {
		if (m_pParent == nullptr)
			return nullptr;
		for (size_t i = 0; i < 3; ++i)
			if (this == m_pParent->m_pChildren[i])
				return m_pParent->m_pChildren[i + 1];
		return nullptr;
	};
	const GQuadNode<_Ty>* getFirstChild() const {
		return m_pChild1;
	};
	
	const GQuadNode<_Ty>* getChild(size_t index) const {
		while (index > 3)
			throw std::range_error("getChild: out of range!");
		return m_pChildren[index];
	}

	_Ty& getData() {
		return m_Data;
	};
	const _Ty& getData() const {
		return m_Data;
	};
	size_t getDepth() const {
		return m_uDepth;
	};
	size_t getNumNodeOnDepth() const {
		return pow(4, m_uDepth);
	};

	GQuadNode<_Ty>* DepthNext() {
		for (size_t i = 0; i < 4; ++i)
			if (m_pChildren[i] != nullptr)
				return m_pChildren[i];
		//没有孩子可遍历了

		//父亲为空则为根节点，表示遍历结束
		if (m_pParent == nullptr)
			return nullptr;
		
		//先去同一深度中的兄弟
		GQuadNode<_Ty>* node = getSibling();
		//存在兄弟则直接返回
		if (node != nullptr)
			return node;

		
		GQuadNode<_Ty>* parent = this;
		//向上一深度遍历兄弟
		while ((parent = parent->m_pParent) != nullptr)
		{
			node = parent->getSibling();
			if (node != nullptr)
				return node;
		}
		return node;
	};

	

private:
	union
	{
		struct{
			GQuadNode<_Ty>* m_pChild1;
			GQuadNode<_Ty>* m_pChild2;
			GQuadNode<_Ty>* m_pChild3;
			GQuadNode<_Ty>* m_pChild4;
		};
		GQuadNode<_Ty>*		m_pChildren[4];					//四个孩子节点
	};

	GQuadNode<_Ty>*			m_pParent;						//父节点
	size_t					m_uDepth;						//当前深度
	_Ty						m_Data;							//数据
};

template<typename _Ty>
class GQuadTree{
public:
	typedef _Ty					value_type;
	typedef _Ty*				pointer;
	typedef const _Ty*			const_pointer;
	typedef _Ty&				reference;
	typedef const _Ty&			const_reference;
	typedef size_t				size_type;
	typedef	size_t				difference_type;
	typedef GQuadNode<_Ty>		node_type;
public:
	typedef std::function<void(typename node_type*)> GQuadNodeFunc_t;
public:
	//构造函数
	GQuadTree() 
		:m_pRoot(nullptr), m_uSize(0)
	{};
	//拷贝构造
	GQuadTree(const GQuadTree& tree)
	{};
	//移动构造
	GQuadTree(GQuadTree&& tree) 
		:m_pRoot(tree.m_pRoot), m_uSize(tree.m_uSize)
	{
		tree.m_pRoot = nullptr;
		tree.m_uSize = 0;
	};
	//析构函数
	~GQuadTree() {
		dealloc(m_pRoot);
	};

	//插入根节点
	GQuadNode<_Ty>* insertRoot(const _Ty& ins_data) {
		while (m_pRoot != nullptr)
			throw std::runtime_error("There has been root!");	//如果已经有根节点则返回空
		m_pRoot = alloc();										//创建根节点
		m_pRoot->m_Data = ins_data;								//插入数据
		m_pRoot->m_uDepth = 0;									//设置深度为0
		return m_pRoot;											//返回根节点
	};
	//插入孩子节点到该父亲节点(返回插入的节点,如果孩子已满插入失败则返回空)
	GQuadNode<_Ty>* insert(GQuadNode<_Ty>* parent, const _Ty& ins_data){
		if (parent == nullptr)
			return insertRoot(ins_data);						//插入根节点
		GQuadNode<_Ty>* node = nullptr;
		for (size_t i = 0; i < 4; ++i)
		{
			if (parent->m_pChildren[i] == nullptr)
			{
				node = alloc();									//创建节点
				node->m_Data = ins_data;						//数据赋值
				node->m_pParent = parent;						//子节点连接父节点
				node->m_uDepth = parent->m_uDepth + 1;			//设置为父节点深度+1
				parent->m_pChildren[i] = node;					//父节点连接子节点
				break;
			}
		}
		return node;
	};
	//插入节点为childIndex个孩子 childIndex: 0~3
	GQuadNode<_Ty>* insert(GQuadNode<_Ty>* parent, const _Ty& ins_data, size_t childIndex) {
		while (parent == nullptr)
			throw std::range_error("insert: Parent is null!");
		if (parent->m_pChildren[childIndex])					//已经存在节点:返回null
			return nullptr;
		GQuadNode<_Ty>* node = alloc();							//创建节点
		parent->m_pChildren[childIndex] = node;
		node->m_Data = ins_data;
		node->m_pParent = parent;
		node->m_uDepth = parent->m_uDepth + 1;

		return node;
	};
	//删除节点(返回父亲节点)
	GQuadNode<_Ty>* erase(GQuadNode<_Ty>* node){
		return dealloc(node);
	};
	//清空所有节点
	void clear() {
		dealloc(m_pRoot);
	};

	//返回节点总数
	size_t size() const{
		return m_uSize;
	};

	//返回根节点
	GQuadNode<_Ty>* root(){
		return m_pRoot;
	};

	//返回根节点(常量版)
	const GQuadNode<_Ty>* root() const{
		return m_pRoot;
	}

	void BreadthTraverse(GQuadNodeFunc_t func) {
		std::vector<GQuadNode<_Ty>*> stack_node;
		stack_node.push_back(m_pRoot);
		while (!stack_node.empty())
		{
			for (size_t i = 0; i < stack_node.size(); ++i)
			{
				GQuadNode<_Ty>* node = stack_node.at(i);
				if (node != nullptr)
					func(node);
			}
			std::vector<GQuadNode<_Ty>*> new_stack_node;
			while (!stack_node.empty())
			{
				GQuadNode<_Ty>* node = stack_node.back();
				stack_node.pop_back();
				if (node)
					for (size_t i = 0; i < 4; ++i)
						new_stack_node.push_back(node->getChild(i));
			}
			stack_node.swap(new_stack_node);
		}
	};

private:
	//创建节点(返回创建的节点)
	GQuadNode<_Ty>* alloc(){
		++m_uSize;
		return new GQuadNode<_Ty>();
	};
	//删除节点(返回父节点)
	GQuadNode<_Ty>* dealloc(GQuadNode<_Ty>* node){
		if(node == nullptr)
			return nullptr;

		//得到父节点
		GQuadNode<_Ty>* parent = node->m_pParent;
		if(parent)							//如果父节点不为空则从父节点删除该节点
		{
			for (size_t i = 0; i < 4; ++i)
			{
				if(node == parent->m_pChildren[i])
				{
					parent->m_pChildren[i] = nullptr;
					break;
				}
			}
		}

		std::vector<GQuadNode<_Ty>*> stack_node;
		stack_node.push_back(node);

		while (!stack_node.empty())
		{
			GQuadNode<_Ty>* node = stack_node.back();
			stack_node.pop_back();
			if(node == nullptr)
				continue;
			for (size_t i = 0; i < 4; ++i)					//遍历添加子节点到栈
				stack_node.push_back(node->m_pChildren[i]);
			delete node;
			--m_uSize;
		}

		return parent;
	};

private:
	GQuadNode<_Ty>*			m_pRoot;				//根节点
	size_t					m_uSize;				//节点个数
};

#endif
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
	//�õ��ֵ�(�����Ƿ����ֵ�)
	GQuadNode<_Ty>* getDepthSibling() {
		GQuadNode<_Ty>* node = getSibling();
		if (node != nullptr)
			return node;
		//���ֵܱ���ʧ��
		GQuadNode<_Ty>* parent = this;
		while ((parent = parent->m_pParent) != nullptr)
		{
			node = parent->getSibling();
			//���û�и����ֵܣ���������ϱ���
			if (node == nullptr)
				continue;
			//�����Ϊ�ղ��� < ��ǰ������������
			while (node != nullptr && node->m_uDepth < m_uDepth)
				node = node->getFirstChild();
			//������ڵ�ǰ����򷵻�
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
		//û�к��ӿɱ�����

		//����Ϊ����Ϊ���ڵ㣬��ʾ��������
		if (m_pParent == nullptr)
			return nullptr;
		
		//��ȥͬһ����е��ֵ�
		GQuadNode<_Ty>* node = getSibling();
		//�����ֵ���ֱ�ӷ���
		if (node != nullptr)
			return node;

		
		GQuadNode<_Ty>* parent = this;
		//����һ��ȱ����ֵ�
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
		GQuadNode<_Ty>*		m_pChildren[4];					//�ĸ����ӽڵ�
	};

	GQuadNode<_Ty>*			m_pParent;						//���ڵ�
	size_t					m_uDepth;						//��ǰ���
	_Ty						m_Data;							//����
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
	//���캯��
	GQuadTree() 
		:m_pRoot(nullptr), m_uSize(0)
	{};
	//��������
	GQuadTree(const GQuadTree& tree)
	{};
	//�ƶ�����
	GQuadTree(GQuadTree&& tree) 
		:m_pRoot(tree.m_pRoot), m_uSize(tree.m_uSize)
	{
		tree.m_pRoot = nullptr;
		tree.m_uSize = 0;
	};
	//��������
	~GQuadTree() {
		dealloc(m_pRoot);
	};

	//������ڵ�
	GQuadNode<_Ty>* insertRoot(const _Ty& ins_data) {
		while (m_pRoot != nullptr)
			throw std::runtime_error("There has been root!");	//����Ѿ��и��ڵ��򷵻ؿ�
		m_pRoot = alloc();										//�������ڵ�
		m_pRoot->m_Data = ins_data;								//��������
		m_pRoot->m_uDepth = 0;									//�������Ϊ0
		return m_pRoot;											//���ظ��ڵ�
	};
	//���뺢�ӽڵ㵽�ø��׽ڵ�(���ز���Ľڵ�,���������������ʧ���򷵻ؿ�)
	GQuadNode<_Ty>* insert(GQuadNode<_Ty>* parent, const _Ty& ins_data){
		if (parent == nullptr)
			return insertRoot(ins_data);						//������ڵ�
		GQuadNode<_Ty>* node = nullptr;
		for (size_t i = 0; i < 4; ++i)
		{
			if (parent->m_pChildren[i] == nullptr)
			{
				node = alloc();									//�����ڵ�
				node->m_Data = ins_data;						//���ݸ�ֵ
				node->m_pParent = parent;						//�ӽڵ����Ӹ��ڵ�
				node->m_uDepth = parent->m_uDepth + 1;			//����Ϊ���ڵ����+1
				parent->m_pChildren[i] = node;					//���ڵ������ӽڵ�
				break;
			}
		}
		return node;
	};
	//����ڵ�ΪchildIndex������ childIndex: 0~3
	GQuadNode<_Ty>* insert(GQuadNode<_Ty>* parent, const _Ty& ins_data, size_t childIndex) {
		while (parent == nullptr)
			throw std::range_error("insert: Parent is null!");
		if (parent->m_pChildren[childIndex])					//�Ѿ����ڽڵ�:����null
			return nullptr;
		GQuadNode<_Ty>* node = alloc();							//�����ڵ�
		parent->m_pChildren[childIndex] = node;
		node->m_Data = ins_data;
		node->m_pParent = parent;
		node->m_uDepth = parent->m_uDepth + 1;

		return node;
	};
	//ɾ���ڵ�(���ظ��׽ڵ�)
	GQuadNode<_Ty>* erase(GQuadNode<_Ty>* node){
		return dealloc(node);
	};
	//������нڵ�
	void clear() {
		dealloc(m_pRoot);
	};

	//���ؽڵ�����
	size_t size() const{
		return m_uSize;
	};

	//���ظ��ڵ�
	GQuadNode<_Ty>* root(){
		return m_pRoot;
	};

	//���ظ��ڵ�(������)
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
	//�����ڵ�(���ش����Ľڵ�)
	GQuadNode<_Ty>* alloc(){
		++m_uSize;
		return new GQuadNode<_Ty>();
	};
	//ɾ���ڵ�(���ظ��ڵ�)
	GQuadNode<_Ty>* dealloc(GQuadNode<_Ty>* node){
		if(node == nullptr)
			return nullptr;

		//�õ����ڵ�
		GQuadNode<_Ty>* parent = node->m_pParent;
		if(parent)							//������ڵ㲻Ϊ����Ӹ��ڵ�ɾ���ýڵ�
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
			for (size_t i = 0; i < 4; ++i)					//��������ӽڵ㵽ջ
				stack_node.push_back(node->m_pChildren[i]);
			delete node;
			--m_uSize;
		}

		return parent;
	};

private:
	GQuadNode<_Ty>*			m_pRoot;				//���ڵ�
	size_t					m_uSize;				//�ڵ����
};

#endif
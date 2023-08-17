#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

#include <cassert>
#include <vector>
#include <functional>
#include <iostream>
#include <string>

template <typename T>
class NodeBinaryTree
{
public:
    typedef T Type;
private:
    typedef NodeBinaryTree<T> Node;
public:
    T m_data;
    Node* m_pParent = nullptr;
    std::vector<Node*> m_pChild = { nullptr, nullptr };

public:
    NodeBinaryTree(Node* pParent, T data, Node* p0 = nullptr, Node* p1 = nullptr)
        : m_pParent(pParent), m_data(data)
    {
        m_pChild[0] = p0;
        m_pChild[1] = p1;
    }

    T getData() const { return m_data; }
    T& getDataRef() { return m_data; }

    void setChild(const Node* pChild, size_t pos) { m_pChild[pos] = const_cast<Node*>(pChild); }
    Node* getChild(size_t branch) const { return m_pChild[branch]; }
    Node*& getChildRef(size_t branch) { return m_pChild[branch]; }
    Node* getParent() const { return m_pParent; }
};

template <typename Container, typename Iterator>
class general_iterator
{
protected:
    Container* m_pContainer = nullptr;
    Iterator m_pNode = nullptr;

public:
    general_iterator(Container* pContainer, Iterator pNode)
        : m_pContainer(pContainer), m_pNode(pNode)
    {}

    general_iterator(const general_iterator& other)
        : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode)
    {}

    general_iterator(general_iterator&& other)
        : m_pContainer(std::move(other.m_pContainer)), m_pNode(std::move(other.m_pNode))
    {}

    general_iterator& operator=(const general_iterator& other)
    {
        if (this != &other)
        {
            m_pContainer = other.m_pContainer;
            m_pNode = other.m_pNode;
        }
        return *this;
    }

    general_iterator& operator=(general_iterator&& other)
    {
        if (this != &other)
        {
            m_pContainer = std::move(other.m_pContainer);
            m_pNode = std::move(other.m_pNode);
        }
        return *this;
    }

    Iterator getNode() const { return m_pNode; }
};

template <typename Container>
class binary_tree_iterator : public general_iterator<Container, typename Container::Node*>
{
public:
    typedef general_iterator<Container, typename Container::Node*> Parent;
    typedef typename Container::Node Node;
    typedef binary_tree_iterator<Container> myself;

public:
    binary_tree_iterator(Container* pContainer, Node* pNode)
        : Parent(pContainer, pNode)
    {}

    binary_tree_iterator(const myself& other)
        : Parent(other)
    {}

    binary_tree_iterator(myself&& other)
        : Parent(std::move(other))
    {}

    binary_tree_iterator& operator=(const myself& other)
    {
        if (this != &other)
        {
            Parent::operator=(other);
        }
        return *this;
    }

    binary_tree_iterator& operator=(myself&& other)
    {
        if (this != &other)
        {
            Parent::operator=(std::move(other));
        }
        return *this;
    }

    binary_tree_iterator operator++()
    {
        // TODO: Implement the logic to move to the next node
        return *this;
    }
};

template <typename _T>
struct BinaryTreeAscTraits
{
    using T = _T;
    using Node = NodeBinaryTree<T>;
    using CompareFn = std::less<T>;
};

template <typename _T>
struct BinaryTreeDescTraits
{
    using T = _T;
    using Node = NodeBinaryTree<T>;
    using CompareFn = std::greater<T>;
};

template <typename Traits>
class BinaryTree
{
public:
    typedef typename Traits::T value_type;
    typedef typename Traits::Node Node;
    typedef typename Traits::CompareFn CompareFn;
    typedef BinaryTree<Traits> myself;
    typedef binary_tree_iterator<myself> iterator;

protected:
    Node* m_pRoot = nullptr;
    size_t m_size = 0;
    CompareFn Compfn;

public:
    size_t size() const { return m_size; }
    bool empty() const { return size() == 0; }
    virtual void insert(const value_type& elem) { internal_insert1(elem, nullptr, m_pRoot); }

protected:
    Node* CreateNode(Node* pParent, const value_type& elem) { return new Node(pParent, elem); }
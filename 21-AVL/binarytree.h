#ifndef __BINARY_TREE_H__  
#define __BINARY_TREE_H__ 
//#include <utility>
//#include <algorithm>
#include <cassert>
#include "types.h"
//#include "util.h"
#include "tree_iterators.h"
using namespace std;

template <typename KeyNode>
class NodeBinaryTree
{
public:
    typedef KeyNode Type;

private:
    typedef NodeBinaryTree<KeyNode> Node;

    KeyNode m_data;
    Node* m_pParent;
    Node* m_pChild[2];

public:
    NodeBinaryTree(Node* pParent, const KeyNode& elem)
        : m_data(elem), m_pParent(pParent)
    {
        m_pChild[0] = m_pChild[1] = nullptr;
    }

    KeyNode& getDataRef() { return m_data; }
    Node*& getParentRef() { return m_pParent; }
    Node*& getChildRef(size_t index) { return m_pChild[index]; }
};

template <typename KeyNode>
struct BinaryTreeAscTraits
{
    using T = KeyNode;
    using Node = NodeBinaryTree<T>;
    using CompareFn = std::less<T>;
};

template <typename KeyNode>
struct BinaryTreeDescTraits
{
    using T = KeyNode;
    using Node = NodeBinaryTree<T>;
    using CompareFn = std::greater<T>;
};

template <typename Traits>
class binary_tree_iterator;

template <typename Traits>
class BinaryTree
{
public:
    using TraitsType = Traits;
    using value_type = typename Traits::T;
    using Node = typename Traits::Node;
    using CompareFn = typename Traits::CompareFn;
    using iterator = binary_tree_iterator<Traits>;

private:
    Node* m_pRoot;
    size_t m_size;

protected:
    CompareFn Compfn;

    virtual Node* CreateNode(Node* pParent, const value_type& elem)
    {
        return new Node(pParent, elem);
    }

    Node* internal_insert1(value_type& elem, Node* pParent, Node*& rpOrigin)
    {
        if (!rpOrigin)
        {
            ++m_size;
            return (rpOrigin = CreateNode(pParent, elem));
        }
        size_t branch = Compfn(elem, rpOrigin->getDataRef());
        return internal_insert1(elem, rpOrigin, rpOrigin->getChildRef(branch));
    }

public:
    BinaryTree() : m_pRoot(nullptr), m_size(0) {}

    virtual ~BinaryTree()
    {
        // Aquí deberías implementar la lógica para liberar la memoria de los nodos del árbol
    }

    void insert(value_type& elem)
    {
        internal_insert1(elem, nullptr, m_pRoot);
    }

    // Resto de las funciones del árbol binario...

    friend class binary_tree_iterator<Traits>;
};

template <typename Traits>
class binary_tree_iterator
{
public:
    using TraitsType = Traits;
    using value_type = typename Traits::T;
    using Node = typename Traits::Node;
    using CompareFn = typename Traits::CompareFn;

private:
    Node* m_pNode;

    void go_left()
    {
        while (m_pNode->getChildRef(0))
        {
            m_pNode = m_pNode->getChildRef(0);
        }
    }

    void go_right()
    {
        while (m_pNode->getChildRef(1))
        {
            m_pNode = m_pNode->getChildRef(1);
        }
    }

public:
    binary_tree_iterator(Node* pNode) : m_pNode(pNode) {}

    value_type& operator*() const { return m_pNode->getDataRef(); }

    binary_tree_iterator& operator++()
    {
        if (m_pNode->getChildRef(1))
        {
            m_pNode = m_pNode->getChildRef(1);
            go_left();
        }
        else
        {
            Node* pPrev = m_pNode;
            m_pNode = m_pNode->getParentRef();
            while (m_pNode && m_pNode->getChildRef(1) == pPrev)
            {
                pPrev = m_pNode;
                m_pNode = m_pNode->getParentRef();
            }
        }
        return *this;
    }

    bool operator==(const binary_tree_iterator& other) const
    {
        return m_pNode == other.m_pNode;
    }

    bool operator!=(const binary_tree_iterator& other) const
    {
        return !(*this == other);
    }
};

int main()
{
    using BinaryTreeAsc = BinaryTree<BinaryTreeAscTraits<int>>;
    BinaryTreeAsc binaryTreeAsc;
    int values[] = {5, 3, 7, 2, 4, 6, 8};

    for (int value : values)
    {
        binaryTreeAsc.insert(value);
    }
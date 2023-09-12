#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__
// #include <utility>
// #include <algorithm>
#include <cassert>
#include "types.h"
#include "keynode.h"
#include "tree_iterator.h"
// #include "util.h"
using namespace std;

template <typename T, typename V>
class NodeBinaryTree
{
public:
    // TODO: Change T by KeyNode
    using value_type = T;
    using Type = T;
    using LinkedValueType = V;

private:
    typedef NodeBinaryTree<T, V> Node;

public:
    KeyNode<T, V> m_data;
    Node *m_pParent = nullptr;
    vector<Node *> m_pChild = {nullptr, nullptr}; // 2 hijos inicializados en nullptr
    size_t m_level = 0;
    bool m_flag = false;
public:
    NodeBinaryTree(Node *pParent, KeyNode<T, V> data, size_t level = 0, Node *p0 = nullptr, Node *p1 = nullptr)
        : m_pParent(pParent), m_data(data)
    {
        m_pChild[0] = p0;
        m_pChild[1] = p1;
        m_level = level;
    }

    NodeBinaryTree(Node *pParent, value_type data, LinkedValueType value, size_t level = 0, Node *p0 = nullptr, Node *p1 = nullptr)
        : m_pParent(pParent), m_data(data, value)
    {
        m_pChild[0] = p0;
        m_pChild[1] = p1;
        m_level = level;
    }

    // TODO: Keynode
    value_type getData() { return m_data.getData(); }
    value_type &getDataRef() { return m_data.getDataRef(); }
    LinkedValueType getValue() const { return m_data.getValue(); }
    LinkedValueType &getValueRef() { return m_data.getValueRef(); }

    // TODO: review if these functions must remain public/private
    void setpChild(const Node *pChild, size_t pos) { m_pChild[pos] = pChild; }
    Node *getChild(size_t branch) { return m_pChild[branch]; }
    Node *&getChildRef(size_t branch) { return m_pChild[branch]; }
    Node *getParent() { return m_pParent; }

    size_t getLevel() { return m_level; }

    void setFlag(bool flag) { m_flag = flag; }
    bool getFlag() { return m_flag; }

    Node *getDirection(Node *pNode, size_t direction = 0)
    {
        assert(pNode != nullptr);
        while (pNode != nullptr && pNode->getChild(direction) != nullptr)
            pNode = pNode->getChild(direction);
            // cout<<pNode->getData()<<endl;
        return pNode;
    }
};

template <typename _T, typename _V>
struct BinaryTreeAscTraits
{
    using T = _T;
    using V = _V;
    using Node = NodeBinaryTree<T, V>;
    using CompareFn = less<T>;
    using LinkedValueType = _V;
};

template <typename _T, typename _V>
struct BinaryTreeDescTraits
{
    using T = _T;
    using V = _V;
    using Node = NodeBinaryTree<T, V>;
    using CompareFn = greater<T>;
    using LinkedValueType = _V;
};

template <typename Traits>
class BinaryTree
{
public:
    typedef typename Traits::T value_type;
    typedef typename Traits::Node Node;
    typedef typename Traits::LinkedValueType LinkedValueType;

    typedef typename Traits::CompareFn CompareFn;
    typedef BinaryTree<Traits> myself;
    typedef binary_tree_iterator<myself> print_iterator;
    typedef binary_tree_iterator_inorder<myself> inor_iterator;
    typedef binary_tree_iterator_posorder<myself> posor_iterator;
    typedef binary_tree_iterator_preorder<myself> presor_iterator;

protected:
    Node *m_pRoot = nullptr;
    size_t m_size = 0;
    CompareFn Compfn;

public:
    size_t size() const { return m_size; }
    bool empty() const { return size() == 0; }
    // TODO: insert must receive two paramaters: elem and LinkedValueType value
    void insert(value_type &key, LinkedValueType value) { internal_insert1(key, value, nullptr, m_pRoot, 0); }
    inor_iterator inbegin() { inor_iterator iter(this, m_pRoot->getDirection(m_pRoot, 0));    return iter;    }
    inor_iterator inend()   { inor_iterator iter(this, m_pRoot->getDirection(m_pRoot, 1));     return iter;    }
    posor_iterator posbegin() { posor_iterator iter(this, m_pRoot->getDirection(m_pRoot, 0));    return iter;    }
    posor_iterator posend()   { posor_iterator iter(this, m_pRoot);     return iter;    }
    presor_iterator prebegin() { presor_iterator iter(this, m_pRoot);    return iter;    }
    presor_iterator preend()   { presor_iterator iter(this, m_pRoot->getDirection(m_pRoot, 1));     return iter;    }
    print_iterator printbegin() { print_iterator iter(this, m_pRoot->getDirection(m_pRoot, 1));    return iter;    }
    print_iterator printend()   { print_iterator iter(this, m_pRoot->getDirection(m_pRoot, 0));     return iter;    }

protected:
    Node *CreateNode(Node *pParent, value_type &key, LinkedValueType value, size_t level) { return new Node(pParent, key, value, level); }
    Node *internal_insert1(value_type &key, LinkedValueType value, Node *pParent, Node *&rpOrigin, size_t level = 0)
    {
        if (!rpOrigin) //  llegué al fondo de una rama
        {
            ++m_size;
            return (rpOrigin = CreateNode(pParent, key, value, level));
        }
        size_t branch = Compfn(key, rpOrigin->getDataRef());
        return internal_insert1(key, value, rpOrigin, rpOrigin->getChildRef(branch), level + 1);
    }

public:

    // TODO: generalize this function by using iterators and apply any function
    void print(void (*func) (Node& node, ostream &os), ostream &os)
    {
        foreach(printbegin(),printend(), func, os);
    }

    void inorder(void (*func) (Node& node, ostream &os), ostream &os)
    {
        foreach(inbegin(),inend(), func, os);
    }

    // TODO: generalize this function by using iterators and apply any function
    // Create a new iterator to walk in postorder
    void postorder(void (*func) (Node& node, ostream &os), ostream &os)
    {
        foreach(posbegin(),posend(), func, os);
    }

    // TODO: generalize this function by using iterators and apply any function
    // Create a new iterator to walk in postorder
    void preorder(void (*func) (Node& node, ostream &os), ostream &os)
    {
        foreach(prebegin(),preend(), func, os);
    }
};

#endif
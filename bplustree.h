#ifndef __BPlus_TREE_H__
#define __BPlus_TREE_H__
// #include <utility>
// #include <algorithm>
#include <cassert>
#include "types.h"
#include "keynode.h"
#include <mutex>
#define DEFAULT_BTREE_ORDER 3
// #include "util.h"
using namespace std;

template <typename T, typename V>
class NodeBPlusTree
{
public:
    // TODO: Change T by KeyNode
    using value_type = T;
    using Type = T;
    using LinkedValueType = V;

private:
    typedef NodeBPlusTree<T, V> Node;

public:
    KeyNode<T, V> m_data;
    Node *m_pParent = nullptr;
    vector<Node *> m_pChild = {nullptr, nullptr}; // 2 hijos inicializados en nullptr
    size_t m_level = 0;
    bool m_flag = false;

public:
    NodeBPlusTree(Node *pParent, KeyNode<T, V> data, size_t level = 0, Node *p0 = nullptr, Node *p1 = nullptr)
        : m_pParent(pParent), m_data(data)
    {
        m_pChild[0] = p0;
        m_pChild[1] = p1;
        m_level = level;
    }

    NodeBPlusTree(Node *pParent, value_type data, LinkedValueType value, size_t level = 0, Node *p0 = nullptr, Node *p1 = nullptr)
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
struct BPlusTreeAscTraits
{
    using T = _T;
    using V = _V;
    using Node = NodeBPlusTree<T, V>;
    using CompareFn = less<T>;
    using LinkedValueType = _V;
};

template <typename _T, typename _V>
struct BPlusTreeDescTraits
{
    using T = _T;
    using V = _V;
    using Node = NodeBPlusTree<T, V>;
    using CompareFn = greater<T>;
    using LinkedValueType = _V;
};

template <typename Traits>
class BPlusTree
{
public:
    typedef typename Traits::T value_type;
    typedef typename Traits::Node Node;
    typedef typename Traits::LinkedValueType LinkedValueType;

    typedef typename Traits::CompareFn CompareFn;
    typedef BPlusTree<Traits> myself;

protected:
    Node *m_pRoot = nullptr;
    size_t m_size = 0;
    CompareFn Compfn;
    mutex mutex_;

public:
    Node * getRoot() {
        return m_pRoot;
    }
    size_t size() const { return m_size; }
    bool empty() const { return size() == 0; }
    // TODO: insert must receive two paramaters: elem and LinkedValueType value
    void insert(value_type &key, LinkedValueType value) { lock_guard<mutex> lock(mutex_); internal_insert1(key, value, nullptr, m_pRoot, 0); }

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
    string lines(string txt, size_t lvl)
    {
        for (auto i = 0; i < lvl; i++)
            txt += txt;
        return txt;
    }
    void print(Node *pNode, ostream &os, size_t level)
    {
        if (pNode)
        {
            Node *pParent = pNode->getParent();
            print(pNode->getChild(1), cout, level + 1);
            // os << string(" | ") * level << pNode->getDataRef() << "(" << (pParent?(pNode->getBranch()?"R-":"L-") + to_string(pParent->getData()):"Root") << ")" <<endl;
            cout << lines(" | ",  level) << pNode->getDataRef() << "(" << (pParent ? to_string(pParent->getData()) : "Root") << ")" << endl;
            print(pNode->getChild(0), cout, level + 1);
        }
    }
};

#endif
#ifndef __BPLUS_TREE_H__
#define __BPLUS_TREE_H__
// #include <utility>
// #include <algorithm>
#include <cassert>
#include "types.h"
#include "keynode.h"
// #include "util.h"
#define MAX 3
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
    bool is_leaf = false;
    Node **ptr;
    value_type * keys;
    int size;

public:
    // NodeBPlusTree();
    NodeBPlusTree(Node *pParent, KeyNode<T, V> data, size_t level = 0, bool leaf = 0, int data_size = 0, Node *p0 = nullptr, Node *p1 = nullptr)
        : m_pParent(pParent), m_data(data)
    {
        m_pChild[0] = p0;
        m_pChild[1] = p1;
        m_level = level;
        is_leaf = leaf;
        size = data_size;
    }

    NodeBPlusTree(Node *pParent, value_type data, LinkedValueType value, size_t level = 0, bool leaf = 0, int data_size = 0, Node *p0 = nullptr, Node *p1 = nullptr)
        : m_pParent(pParent), m_data(data, value)
    {
        m_pChild[0] = p0;
        m_pChild[1] = p1;
        m_level = level;
        is_leaf = leaf;
        size = data_size;
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

public:
    size_t size() const { return m_size; }
    bool empty() const { return size() == 0; }
    Node* getRoot()
    {
        return m_pRoot;
    }
    // TODO: insert must receive two paramaters: elem and LinkedValueType value
    void insert(value_type &key, LinkedValueType value) { internal_insert1(key, value, nullptr, m_pRoot, 0); }

protected:
    Node *CreateNode(Node *pParent, value_type &key, LinkedValueType value, size_t level, bool leaf, int size) { return new Node(pParent, key, value, level, leaf, size); }
    // Node *internal_insert1(value_type &key, LinkedValueType value, Node *pParent, Node *&rpOrigin, size_t level = 0)
    // {
    //     if (rpOrigin) //  llegué al fondo de una rama
    //     {
    //         ++m_size;
    //         return (rpOrigin = CreateNode(pParent, key, value, level));
    //     }
    //     // size_t branch = Compfn(key, rpOrigin->getDataRef());
    //     // return internal_insert1(key, value, rpOrigin, rpOrigin->getChildRef(branch), level + 1);
    // }

    void internal_insert1(value_type &key, LinkedValueType value, Node *pParent, Node *&rpOrigin, size_t level = 0)
    {
        if (m_pRoot == nullptr)
        {
            m_pRoot = CreateNode(pParent, key, value, level, 1, 1);
            ++m_size;
        }
    }
    
    // void insertInternal(int x, Node *cursor, Node *child)
    // {
    //     if (cursor->size < MAX)
    //     {
    //         // if cursor is not full
    //         // find the correct position for new key
    //         int i = 0;
    //         while (x > cursor->keys[i] && i < cursor->size)
    //             i++;
    //         // make space for new key
    //         for (int j = cursor->size; j > i; j--)
    //         {
    //             cursor->keys[j] = cursor->keys[j - 1];
    //         } // make space for new pointer
    //         for (int j = cursor->size + 1; j > i + 1; j--)
    //         {
    //             cursor->ptr[j] = cursor->ptr[j - 1];
    //         }
    //         cursor->keys[i] = x;
    //         cursor->size++;
    //         cursor->ptr[i + 1] = child;
    //         cout << "Inserted key in an Internal node successfully\n";
    //     }
    //     else
    //     {
    //         cout << "Inserted key in an Internal node successfully\n";
    //         cout << "Overflow in internal node!\nSplitting internal node\n";
    //         // if overflow in internal node
    //         // create new internal node
    //         Node *newInternal = new Node;
    //         // create virtual Internal Node;
    //         int virtualKey[MAX + 1];
    //         Node *virtualPtr[MAX + 2];
    //         for (int i = 0; i < MAX; i++)
    //         {
    //             virtualKey[i] = cursor->keys[i];
    //         }
    //         for (int i = 0; i < MAX + 1; i++)
    //         {
    //             virtualPtr[i] = cursor->ptr[i];
    //         }
    //         int i = 0, j;
    //         while (x > virtualKey[i] && i < MAX)
    //             i++;
    //         // make space for new key
    //         for (int j = MAX + 1; j > i; j--)
    //         {
    //             virtualKey[j] = virtualKey[j - 1];
    //         }
    //         virtualKey[i] = x;
    //         // make space for new ptr
    //         for (int j = MAX + 2; j > i + 1; j--)
    //         {
    //             virtualPtr[j] = virtualPtr[j - 1];
    //         }
    //         virtualPtr[i + 1] = child;
    //         newInternal->is_leaf = false;
    //         // split cursor into two nodes
    //         cursor->size = (MAX + 1) / 2;
    //         newInternal->size = MAX - (MAX + 1) / 2;
    //         // give elements and pointers to the new node
    //         for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++)
    //         {
    //             newInternal->keys[i] = virtualKey[j];
    //         }
    //         for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++)
    //         {
    //             newInternal->ptr[i] = virtualPtr[j];
    //         }
    //         // m = cursor->key[cursor->size]
    //         if (cursor == m_pRoot)
    //         {
    //             // if cursor is a root node, we create a new root
    //             Node *newRoot = new Node;
    //             newRoot->keys[0] = cursor->keys[cursor->size];
    //             newRoot->ptr[0] = cursor;
    //             newRoot->ptr[1] = newInternal;
    //             newRoot->is_leaf = false;
    //             newRoot->size = 1;
    //             m_pRoot = newRoot;
    //             cout << "Created new root\n";
    //         }
    //         else
    //         {
    //             // recursion
    //             // find depth first search to find parent of cursor
    //             insertInternal(cursor->keys[cursor->size], findParent(m_pRoot, cursor), newInternal);
    //         }
    //     }
    // }

    // Node *findParent(Node *cursor, Node *child)
    // {
    //     // finds parent using depth first traversal and ignores leaf nodes as they cannot be parents
    //     // also ignores second last level because we will never find parent of a leaf node during insertion using this function
    //     Node *parent;
    //     if (cursor->is_leaf || (cursor->ptr[0])->is_leaf)
    //     {
    //         return NULL;
    //     }
    //     for (int i = 0; i < cursor->size + 1; i++)
    //     {
    //         if (cursor->ptr[i] == child)
    //         {
    //             parent = cursor;
    //             return parent;
    //         }
    //         else
    //         {
    //             parent = findParent(cursor->ptr[i], child);
    //             if (parent != NULL)
    //                 return parent;
    //         }
    //     }
    //     return parent;
    // }

public:
    // TODO: generalize this function by using iterators and apply any function
    void print(Node* cursor)
    {
        //depth first display
        if(cursor!=nullptr)
        {
            for(int i = 0; i < cursor->size; i++)
            {
                cout<<cursor->keys[i]<<" ";
            }
            cout<<"\n";
            if(cursor->is_leaf != true)
            {
                for(int i = 0; i < cursor->size+1; i++)
                {
                    print(cursor->ptr[i]);
                }
            }
        }
    }
};

#endif
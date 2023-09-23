#ifndef __BPLUS_H__
#define __BPLUS_H__

#include <iostream>
#include <vector>
#include <algorithm> // swap algorithm
#include <cassert>
#include <math.h>
#include "types.h"
#include "keynode.h"
#include "xtrait.h"
#include "array.h"
#include "foreach.h"

using XTraitFloatStringDesc = XTrait<float, string, std::less<const KeyNode<int, int> &>>;
using XTraitIntIntAsc = XTrait<int, int, std::greater<const KeyNode<int, int> &>>;

using namespace std;

#define DEFAULT_BTREE_ORDER 3

template <typename Traits>
class CBPlusTree;

template <typename Traits>
class CBPlusTreeNode;

// *************************************************************************
//                             Iterator Class
// *************************************************************************
template <typename Container>
class bplus_iterator
{
private:
    typedef typename Container::TreeNode Node;
    typedef typename Container::Node KeyNode;
    typedef bplus_iterator<Container> myself;

protected:
    Container *m_pContainer;
    Node *m_pNode;
    size_t m_Pos;

public:
    // *************************************************************************
    //                             Constructors
    // *************************************************************************
    bplus_iterator(Container *pContainer, Node *pNode) : m_pContainer(pContainer), m_pNode(pNode) { initialize(); }

    bplus_iterator(myself &other) : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode) { initialize(); }

    bplus_iterator(myself &&other) // Move constructor
    {
        m_pContainer = move(other.m_pContainer);
        m_pNode = move(other.m_pNode);
        initialize();
    }

    // *************************************************************************
    //                              Methods
    // *************************************************************************
    void initialize()
    {
        Node *node = m_pNode;
        if (node == nullptr)
        {
            return;
        }
        while (!node->isLeaf())
        {
            node = node->m_Children[0];
        }
        m_pNode = node;
        m_Pos = 0;
    }

    myself operator=(myself &iter)
    {
        m_pContainer = move(iter.m_pContainer);
        m_pNode = move(iter.m_pNode);
        return *(myself *)this; // Pending static_cast?
    }

    bplus_iterator operator++()
    {
        if (m_Pos < m_pNode->m_Data.size() - 1)
        {
            m_Pos++;
        }
        else
        {
            m_pNode = m_pNode->m_Next;
            m_Pos = 0;
        }
        return *this;
    }

    bool operator==(myself iter) { return m_pNode == iter.m_pNode; }
    bool operator!=(myself iter) { return !(*this == iter); }
    KeyNode &operator*() { return m_pNode->getData(m_Pos); }
};

// *************************************************************************
//                               Node Class
// *************************************************************************
template <typename Traits>
class CBPlusTreeNode
{
public:
    // *************************************************************************
    //                                Utils
    // *************************************************************************
    friend class CBPlusTree<Traits>;
    friend class bplus_iterator<CBPlusTree<Traits>>;
    template <typename T>
    friend ostream &operator<<(ostream &os, CBPlusTree<T> &tree);

    template <typename T>
    friend istream &operator>>(istream &is, CBPlusTree<Traits> &tree);

    using value_type = typename Traits::value_type;
    using KeyType = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    // Node is a KeyNode, contains a key and a value
    using Node = typename Traits::Node;
    using CompareFn = typename Traits::CompareFn;
    using Myself = CBPlusTreeNode<Traits>;

private:
    // *************************************************************************
    //                               Members
    // *************************************************************************
    const size_t m_Order;
    const size_t m_MaxChildren = m_Order;
    const size_t m_MinChildren = ceil(m_Order / 2.f);
    const size_t m_MaxKeys = m_MaxChildren - 1;
    const size_t m_MinKeys = m_MinChildren - 1;
    vector<Node> m_Data;
    vector<Myself *> m_Children;
    Myself *m_Parent = nullptr;
    Myself *m_Next = nullptr;
    CompareFn Compare = CompareFn();

public:
    // *************************************************************************
    //                             Costructors
    // *************************************************************************
    CBPlusTreeNode(size_t order) : m_Order(order) {}

    // *************************************************************************
    //                               Methods
    // *************************************************************************
    bool isRoot()
    {
        return m_Parent == nullptr;
    }

    bool isLeaf()
    {
        return m_Children.empty();
    }

    bool isOverFlow()
    {
        return m_Data.size() > m_MaxKeys;
    }

    bool isUnderFlow()
    {
        return m_Data.size() < m_MinKeys;
    }

    KeyType &getKey(size_t pos)
    {
        return m_Data[pos].getDataRef();
    }

    LinkedValueType &getValue(size_t pos)
    {
        return m_Data[pos].getValueRef();
    }

    Node &getData(size_t pos)
    {
        return m_Data[pos];
    }

    vector<Node> &getData()
    {
        return m_Data;
    }
};

// Created by: @ecuadros
// *************************************************************************
//                              Tree Class
// *************************************************************************
template <typename Traits>
class CBPlusTree
{
public:
    // *************************************************************************
    //                                Utils
    // *************************************************************************
    using value_type = typename Traits::value_type;
    using KeyType = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using Node = typename Traits::Node;
    using CompareFn = typename Traits::CompareFn;
    using Myself = CBPlusTree<Traits>;
    using TreeNode = CBPlusTreeNode<Traits>;
    friend class bplus_iterator<Myself>;

private:
    // *************************************************************************
    //                               Members
    // *************************************************************************
    string m_Name = "BPlusTree";
    size_t m_Size = 0;
    size_t m_Order = DEFAULT_BTREE_ORDER;
    TreeNode *m_Root = nullptr;
    const size_t m_MaxChildren = m_Order;
    const size_t m_MinChildren = m_Order / 2;
    const CompareFn Compare = CompareFn();

public:
    // *************************************************************************
    //                             Costructors
    // *************************************************************************
    CBPlusTree(string name = "BPlusTree", size_t order = DEFAULT_BTREE_ORDER) : m_Name(name), m_Order(order), m_Root(new TreeNode(order)) {}
    CBPlusTree() {}
    CBPlusTree(const Myself &other) {}

    // *************************************************************************
    //                             Destructor
    // *************************************************************************
    virtual ~CBPlusTree()
    {
        cerr << "Destroying " << m_Name << "..." << endl;
        destroy();
    }

    // *************************************************************************
    //                               Methods
    // *************************************************************************
    void destroy()
    {
        destroy(m_Root);
        m_Root->m_Children.clear();
        m_Root->m_Data.clear();
    }

    void destroy(TreeNode* node)
    {
        for(size_t i = 0; i < node->m_Children.size(); i++)
        {
            destroy(node->m_Children[i]);
            delete node->m_Children[i];
        }
    }

    TreeNode *leafSearch(const KeyType &key)
    {
        TreeNode *node = m_Root;
        while (node && !node->isLeaf())
        {
            vector<Node> &data = node->m_Data;
            vector<TreeNode *> &children = node->m_Children;
            for (size_t i = 0; i < data.size(); i++)
            {
                if (key == node->getKey(i))
                {
                    node = children[i + 1];
                    break;
                }
                else if (!Compare(Node(key, 0), node->m_Data[i]))
                {
                    node = children[i];
                    break;
                }
                else if (i == data.size() - 1)
                {
                    node = children[i + 1];
                    break;
                }
            }
        }
        return node;
    }

    Node &search(const KeyType &key)
    {
        // Search leaf node
        TreeNode *node = leafSearch(key);

        // Linear search key in leaf node
        if (node)
        {
            vector<Node *> &data = node->m_Data;
            for (size_t i = 0; i < data.size(); i++)
            {
                if (key == node->getKey(i))
                {
                    return data[i];
                }
            }
        }
        return Node(0, 0);
    }

    void insert(const KeyType &key, LinkedValueType value)
    {
        // Search and insert data in leaf
        TreeNode *node = leafSearch(key);
        Node compared(key, value);
        auto insertIt = node->m_Data.end();
        for (auto it = node->m_Data.begin(); it != node->m_Data.end(); it++)
        {
            if (!Compare(compared, *it))
            {
                insertIt = it;
                break;
            }
        }
        node->m_Data.insert(insertIt, Node(key, value));

        // If overflow
        if (node->isOverFlow())
        {
            // Split leaf
            TreeNode *newNode = new TreeNode(m_Order);
            newNode->m_Parent = node->m_Parent;
            size_t mid = node->m_Data.size() / 2;
            newNode->m_Data.insert(newNode->m_Data.begin(), node->m_Data.begin() + mid, node->m_Data.end());
            node->m_Data.erase(node->m_Data.begin() + mid, node->m_Data.end());

            // Update leaf next
            newNode->m_Next = node->m_Next;
            node->m_Next = newNode;

            // Update parent
            parentInsert(node, newNode, newNode->getKey(0));
        }
    }

    void parentInsert(TreeNode *node, TreeNode *newNode, const KeyType &key)
    {
        if (node->isRoot())
        {
            TreeNode *newRoot = new TreeNode(m_Order);
            newRoot->m_Data.push_back(Node(key, 0));
            newRoot->m_Children.push_back(node);
            newRoot->m_Children.push_back(newNode);
            m_Root = newRoot;
            node->m_Parent = newRoot;
            newNode->m_Parent = newRoot;
        }
        else
        {
            TreeNode *parent = node->m_Parent;

            // Search and insert key in parent
            for (size_t i = 0; i < parent->m_Children.size(); i++)
            {
                if (parent->m_Children[i] == node)
                {
                    parent->m_Children.insert(parent->m_Children.begin() + i + 1, newNode);
                    parent->m_Data.insert(parent->m_Data.begin() + i, Node(key, 0));
                    break;
                }
            }

            // If overflow
            if (parent->isOverFlow())
            {
                // Split parent
                TreeNode *newParent = new TreeNode(parent->m_Order);
                newParent->m_Parent = parent->m_Parent;
                size_t mid = (parent->m_Data.size() / 2);
                newParent->m_Data.insert(newParent->m_Data.begin(), parent->m_Data.begin() + mid + 1, parent->m_Data.end());
                newParent->m_Children.insert(newParent->m_Children.begin(), parent->m_Children.begin() + mid + 1, parent->m_Children.end());
                KeyType val = parent->getKey(mid);
                parent->m_Data.erase(parent->m_Data.begin() + mid, parent->m_Data.end());
                parent->m_Children.erase(parent->m_Children.begin() + mid + 1, parent->m_Children.end());

                // Update children parent
                for (size_t i = 0; i < newParent->m_Children.size(); i++)
                {
                    newParent->m_Children[i]->m_Parent = newParent;
                }

                // Update parent
                parentInsert(parent, newParent, val);
            }
        }
    }

    void remove(const KeyType& key)
    {
        // Find leaf node
        TreeNode *node = leafSearch(key);

        // Remove key from leaf node
        remove(key, node);
    }

    void remove(const KeyType &key, TreeNode* node)
    {
        if(node == nullptr) return;
        bool nodeIsLeaf = node->isLeaf();

        // Find key in leaf node
        bool found = false;
        size_t pos = 0;
        for(size_t i = 0; i < node->m_Data.size(); i++)
        {
            if(key == node->getKey(i))
            {
                pos = i;
                found = true;
                break;
            }
        }
        if(!found) return;

        // Check if node is root 
        if(node->isRoot())
        {
            node->m_Data.erase(node->m_Data.begin() + pos);
            return;
        }
        // Check if removing will not cause underflow
        else if(node->m_Data.size() > node->m_MinKeys)
        {
            node->m_Data.erase(node->m_Data.begin() + pos);
        }
        else 
        {
            TreeNode* right = nullptr;
            TreeNode* left = nullptr;
            size_t posInParent = 0;

            // Find siblings
            for(size_t i = 0; i < node->m_Parent->m_Children.size(); i++)
            {
                if(node->m_Parent->m_Children[i] == node)
                {
                    posInParent = i;
                    if(i > 0)
                    {
                        left = node->m_Parent->m_Children[i - 1];
                    }
                    if(i < node->m_Parent->m_Children.size() - 1)
                    {
                        right = node->m_Parent->m_Children[i + 1];
                    }
                    break;
                }
            }
            bool borrowSuccess = false;
            // Try to borrow from right
            if(right != nullptr)
            {
                if(right->m_Data.size() > right->m_MinKeys)
                {
                    node->m_Data.erase(node->m_Data.begin() + pos);
                    node->m_Data.push_back(right->m_Data[0]);
                    right->m_Data.erase(right->m_Data.begin());
                    node->m_Parent->m_Data[pos] = right->m_Data[0];
                    borrowSuccess = true;
                }
            }
            // Try to borrow from left
            else if(left != nullptr)
            {
                if(left->m_Data.size() > left->m_MinKeys)
                {
                    node->m_Data.erase(node->m_Data.begin() + pos);
                    node->m_Data.insert(node->m_Data.begin(), left->m_Data.back());
                    left->m_Data.pop_back();
                    node->m_Parent->m_Data[pos - 1] = node->m_Data[0];
                    borrowSuccess = true;
                }
            }
            
            // Merge if borrow failed
            if(!borrowSuccess)
            {
                if(right != nullptr)
                {
                    node->m_Data.erase(node->m_Data.begin() + pos);
                    node->m_Data.insert(node->m_Data.end(), right->m_Data.begin(), right->m_Data.end());
                    if(node->isLeaf())
                    {
                        node->m_Next = right->m_Next;
                    }
                    remove(node->m_Parent->getKey(posInParent), node->m_Parent);
                    node->m_Parent->m_Children.erase(node->m_Parent->m_Children.begin() + posInParent + 1);
                    delete right;
                }
                else if(left != nullptr)
                {
                    node->m_Data.erase(node->m_Data.begin() + pos);
                    node->m_Data.insert(node->m_Data.begin(), left->m_Data.begin(), left->m_Data.end());
                    if(node->isLeaf())
                    {
                        left->m_Next = node->m_Next;
                    }
                    remove(node->m_Parent->getKey(posInParent - 1), node->m_Parent);
                    node->m_Parent->m_Children.erase(node->m_Parent->m_Children.begin() + posInParent);
                    delete node;
                }
                return;
            }
        }
        remove(key, node->m_Parent);
    }

    void print(ostream &os)
    {
        os << m_Name << endl;
        innerPrint(os, m_Root, 0);
    }

    void read(istream &is)
    {
        while (!is.eof())
        {
            KeyType key;
            LinkedValueType value;
            is >> key >> value;
            insert(key, value);
        }
    }

    bplus_iterator<Myself> begin()
    {
        return bplus_iterator<Myself>(this, m_Root);
    }

    bplus_iterator<Myself> end()
    {
        return bplus_iterator<Myself>(this, nullptr);
    }

    template <typename Callable, typename... Args>
    void forEach(Callable f, Args &&...args)
    {
        ::foreach (begin(), end(), f, args...);
    }

    // *************************************************************************
    //                               Operators
    // *************************************************************************
    // DONE : agregar el operator []
    Node &operator[](const KeyType &key)
    {
        return search(key);
    }

private:
    // *************************************************************************
    //                            Private Methods
    // *************************************************************************
    void innerPrint(ostream &os, TreeNode *node, size_t level)
    {
        if (node)
        {
            vector<Node> &data = node->m_Data;
            vector<TreeNode *> &children = node->m_Children;
            for (size_t i = 0; i < data.size(); i++)
            {
                if (node->isLeaf())
                {
                    os << string(level, '\t') << "(" << node->getKey(i) << "->" << node->getValue(i) << ")" << endl;
                }
                else
                {
                    innerPrint(os, children[i], level + 1);
                    os << string(level, '\t') << "(" << node->getKey(i) << ")" << endl;
                }
            }
            if (!node->isLeaf())
            {
                innerPrint(os, children.back(), level + 1);
            }
        }
    }
};

// *************************************************************************
//                          Related Functions
// *************************************************************************

template <typename Traits>
ostream &operator<<(ostream &os, CBPlusTree<Traits> &tree)
{
    tree.print(os);
    return os;
}

template <typename Traits>
istream &operator>>(istream &is, CBPlusTree<Traits> &tree)
{
    tree.read(is);
    return is;
}

#endif

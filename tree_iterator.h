#ifndef __TREE_ITERATOR_H__
#define __TREE_ITERATOR_H__
#include <algorithm>
#include <utility>

template <typename Container> //
class binary_tree_iterator
{
public:
    typedef typename Container::Node Node;
    typedef typename Node::Type Type;
    // typedef class general_iterator<Container> Parent;
    typedef binary_tree_iterator<Container> myself; //

protected:
    Container *m_pContainer;
    Node *m_pNode;
    bool m_flag = 0;

public:
    binary_tree_iterator(Container *pContainer, Node *pNode)
        : m_pContainer(pContainer), m_pNode(pNode) {
            m_flag = pNode->getFlag();
        }
    binary_tree_iterator(myself &other)
        : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode), m_flag(other.m_flag) {}
    binary_tree_iterator(myself &&other) // Move constructor
    {
        m_pContainer = move(other.m_pContainer);
        m_pNode = move(other.m_pNode);
        m_flag = move(other.m_flag);
    }
    myself operator=(myself &iter)
    {
        m_pContainer = move(iter.m_pContainer);
        m_pNode = move(iter.m_pNode);
        return *(myself *)this; // Pending static_cast?
    }

    bool operator==(myself iter) { return m_pNode == iter.m_pNode; }
    bool operator!=(myself iter) { return !(*this == iter); }
    Node &operator*() { return *m_pNode; }

    // https://github.com/edson2495/2023-I-MCC639B-EDA/blob/d84bb94d5fa9bd3e95b4c64efef2bb346d40e2a4/tree_iterators.h

    myself operator++()
    {
        bool r;
        Node *pNode = m_pNode;
        pNode->setFlag(!m_flag);
        while (!r)
            pNode = printing(pNode, r);
        m_pNode = pNode;
        return *this;
    }

    Node *printing(Node *pNode, bool &r)
    {
        if (pNode->getChild(1) != nullptr && pNode->getChild(1)->getFlag() == m_flag)
        {
            return printing(pNode->getChild(1), r);
        }
        if (pNode->getFlag() == m_flag)
        {
            r = 1;
            pNode->setFlag(!m_flag);
            return pNode;
        }
        if (pNode->getChild(0) != nullptr && pNode->getChild(0)->getFlag() == m_flag)
        {
            return printing(pNode->getChild(0), r);
        }
        return pNode->getParent();
    }
};

template <typename Container> //
class binary_tree_iterator_inorder
{
public:
    typedef typename Container::Node Node;
    typedef typename Node::Type Type;
    // typedef class general_iterator<Container> Parent;
    typedef binary_tree_iterator_inorder<Container> myself; //

protected:
    Container *m_pContainer;
    Node *m_pNode;
    bool m_flag = 0;

public:
    binary_tree_iterator_inorder(Container *pContainer, Node *pNode)
        : m_pContainer(pContainer), m_pNode(pNode), m_flag(pNode->getFlag()) { }
    binary_tree_iterator_inorder(myself &other)
        : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode), m_flag(other.m_flag) {}
    binary_tree_iterator_inorder(myself &&other) // Move constructor
    {
        m_pContainer = move(other.m_pContainer);
        m_pNode = move(other.m_pNode);
        m_flag = move(other.m_flag);
    }
    myself operator=(myself &iter)
    {
        m_pContainer = move(iter.m_pContainer);
        m_pNode = move(iter.m_pNode);
        return *(myself *)this; // Pending static_cast?
    }

    bool operator==(myself iter) { return m_pNode == iter.m_pNode; }
    bool operator!=(myself iter) { return !(*this == iter); }
    Node &operator*() { return *m_pNode; }

    // https://github.com/edson2495/2023-I-MCC639B-EDA/blob/d84bb94d5fa9bd3e95b4c64efef2bb346d40e2a4/tree_iterators.h

    myself operator++()
    {
        bool r;
        Node *pNode = m_pNode;
        pNode->setFlag(!m_flag);
        while (!r)
            pNode = printing(pNode, r);
        m_pNode = pNode;
        return *this;
    }

    Node *printing(Node *pNode, bool &r)
    {
        if (pNode->getFlag() == m_flag)
        {
            r = 1;
            pNode->setFlag(!m_flag);
            return pNode;
        }
        if (pNode->getChild(0) != nullptr && pNode->getChild(0)->getFlag() == m_flag)
        {
            return printing(pNode->getChild(0), r);
        }
        if (pNode->getChild(1) != nullptr && pNode->getChild(1)->getFlag() == m_flag)
        {
            return printing(pNode->getChild(1), r);
        }
        return pNode->getParent();
    }
};


template <typename Container> //
class binary_tree_iterator_posorder
{
public:
    typedef typename Container::Node Node;
    typedef typename Node::Type Type;
    // typedef class general_iterator<Container> Parent;
    typedef binary_tree_iterator_posorder<Container> myself; //

protected:
    Container *m_pContainer;
    Node *m_pNode;
    bool m_flag = 0;

public:
    binary_tree_iterator_posorder(Container *pContainer, Node *pNode)
        : m_pContainer(pContainer), m_pNode(pNode), m_flag(pNode->getFlag()) { }
    binary_tree_iterator_posorder(myself &other)
        : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode), m_flag(other.m_flag) {}
    binary_tree_iterator_posorder(myself &&other) // Move constructor
    {
        m_pContainer = move(other.m_pContainer);
        m_pNode = move(other.m_pNode);
        m_flag = move(other.m_flag);
    }
    myself operator=(myself &iter)
    {
        m_pContainer = move(iter.m_pContainer);
        m_pNode = move(iter.m_pNode);
        return *(myself *)this; // Pending static_cast?
    }

    bool operator==(myself iter) { return m_pNode == iter.m_pNode; }
    bool operator!=(myself iter) { return !(*this == iter); }
    Node &operator*() { return *m_pNode; }

    // https://github.com/edson2495/2023-I-MCC639B-EDA/blob/d84bb94d5fa9bd3e95b4c64efef2bb346d40e2a4/tree_iterators.h

    myself operator++()
    {
        bool r;
        Node *pNode = m_pNode;
        pNode->setFlag(!m_flag);
        while (!r)
            pNode = printing(pNode, r);
        m_pNode = pNode;
        return *this;
    }

    Node *printing(Node *pNode, bool &r)
    {
        if (pNode->getChild(0) != nullptr && pNode->getChild(0)->getFlag() == m_flag)
        {
            return printing(pNode->getChild(0), r);
        }
        if (pNode->getChild(1) != nullptr && pNode->getChild(1)->getFlag() == m_flag)
        {
            return printing(pNode->getChild(1), r);
        }
        if (pNode->getFlag() == m_flag)
        {
            r = 1;
            pNode->setFlag(!m_flag);
            return pNode;
        }
        return pNode->getParent();
    }
};

template <typename Container> //
class binary_tree_iterator_preorder
{
public:
    typedef typename Container::Node Node;
    typedef typename Node::Type Type;
    // typedef class general_iterator<Container> Parent;
    typedef binary_tree_iterator_preorder<Container> myself; //

protected:
    Container *m_pContainer;
    Node *m_pNode;
    bool m_flag = 0;

public:
    binary_tree_iterator_preorder(Container *pContainer, Node *pNode)
        : m_pContainer(pContainer), m_pNode(pNode), m_flag(pNode->getFlag()) { }
    binary_tree_iterator_preorder(myself &other)
        : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode), m_flag(other.m_flag) {}
    binary_tree_iterator_preorder(myself &&other) // Move constructor
    {
        m_pContainer = move(other.m_pContainer);
        m_pNode = move(other.m_pNode);
        m_flag = move(other.m_flag);
    }
    myself operator=(myself &iter)
    {
        m_pContainer = move(iter.m_pContainer);
        m_pNode = move(iter.m_pNode);
        return *(myself *)this; // Pending static_cast?
    }

    bool operator==(myself iter) { return m_pNode == iter.m_pNode; }
    bool operator!=(myself iter) { return !(*this == iter); }
    Node &operator*() { return *m_pNode; }

    // https://github.com/edson2495/2023-I-MCC639B-EDA/blob/d84bb94d5fa9bd3e95b4c64efef2bb346d40e2a4/tree_iterators.h

    myself operator++()
    {
        bool r;
        Node *pNode = m_pNode;
        pNode->setFlag(!m_flag);
        while (!r)
            pNode = printing(pNode, r);
        m_pNode = pNode;
        return *this;
    }

    Node *printing(Node *pNode, bool &r)
    {
        if (pNode->getFlag() == m_flag)
        {
            r = 1;
            pNode->setFlag(!m_flag);
            return pNode;
        }
        if (pNode->getChild(0) != nullptr && pNode->getChild(0)->getFlag() == m_flag)
        {
            return printing(pNode->getChild(0), r);
        }
        if (pNode->getChild(1) != nullptr && pNode->getChild(1)->getFlag() == m_flag)
        {
            return printing(pNode->getChild(1), r);
        }

        return pNode->getParent();
    }
};

#endif

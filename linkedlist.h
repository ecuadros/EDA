#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <utility>
#include <algorithm>
#include <cassert>
#include "iterator.h"
#include "types.h"

template <typename Node, typename MemberType>
void CreateBridge(Node*& rParent, Node* pNew, MemberType _pMember)
{
    Node* Node::*pMember = reinterpret_cast<Node* Node::*>(_pMember);
    pNew->*pMember = rParent;
    rParent = pNew;
}

template <typename T>
class NodeLinkedList
{
public:
    typedef T Type;

private:
    typedef NodeLinkedList<T> Node;

public:
    T m_data;
    Node* m_pNext;

public:
    NodeLinkedList(T data, Node* pNext = nullptr)
        : m_data(data), m_pNext(pNext)
    {}

    T getData() const { return m_data; }
    T& getDataRef() { return m_data; }

    void setpNext(NodeLinkedList* pNext) { m_pNext = pNext; }
    Node* getpNext() const { return m_pNext; }
    Node*& getpNextRef() { return m_pNext; }
};

template <typename Container>
class forward_iterator : public general_iterator<Container, forward_iterator<Container>>
{
public:
    typedef class general_iterator<Container, forward_iterator<Container>> Parent;
    typedef typename Container::Node Node;
    typedef forward_iterator<Container> myself;

public:
    forward_iterator(Container* pContainer, Node* pNode)
        : Parent(pContainer, pNode)
    {}

    forward_iterator(const myself& other)
        : Parent(other)
    {}

    forward_iterator(myself&& other)
        : Parent(std::move(other))
    {}

    forward_iterator& operator++()
    {
        Parent::m_pNode = static_cast<Node*>(Parent::m_pNode->getpNext());
        return *this;
    }
};

template <typename _T>
struct LLTraitAsc
{
    using T = _T;
    using Node = NodeLinkedList<T>;
    using CompareFn = std::less<T>;
};

template <typename _T>
struct LLTraitDesc
{
    using T = _T;
    using Node = NodeLinkedList<T>;
    using CompareFn = std::greater<T>;
};

template <typename Traits>
class LinkedList
{
public:
    typedef typename Traits::T value_type;
    typedef typename Traits::Node Node;
    typedef typename Traits::CompareFn CompareFn;
    typedef LinkedList<Traits> myself;
    typedef forward_iterator<myself> iterator;

protected:
    Node* m_pHead = nullptr;
    Node* m_pTail = nullptr;
    size_t m_size = 0;
    CompareFn Compfn;

public:
    size_t size() const { return m_size; }
    bool empty() const { return size() == 0; }

public:
    LinkedList() {}

    void insert(const value_type& elem)
    {
        insert_forward(elem);
    }

    value_type& operator[](size_t pos)
    {
        assert(pos < size());
        Node* pTmp = m_pHead;
        for (size_t i = 0; i < pos; i++)
            pTmp = pTmp->getpNext();
        return pTmp->getDataRef();
    }

    iterator begin()
    {
        iterator iter(this, m_pHead);
        return iter;
    }

    iterator end()
    {
        iterator iter(this, nullptr);
        return iter;
    }

    void push_front(const value_type& elem)
    {
        Node* pNew = CreateNode(elem);
        pNew->setpNext(m_pHead);
        m_pHead = pNew;
        m_size++;
        if (m_size == 1)
            m_pTail = pNew;
    }

    void push_back(const value_type& elem)
    {
        Node* pNew = CreateNode(elem, nullptr);
        if (m_pTail)
            m_pTail->setpNext(pNew);
        m_pTail = pNew;
        if (!m_pHead)
            m_pHead = pNew;
        m_size++;
    }

protected:
    Node** findPrev(value_type& elem)
    {
        return findPrev(m_pHead, elem);
    }

    Node** findPrev(Node*& rpPrev, value_type& elem)
    {
        if (!rpPrev || Compfn(elem, rpPrev->getData()))
            return &rpPrev;
        return findPrev(reinterpret_cast<Node*&>(rpPrev->getpNextRef()), elem);
    }

    Node* CreateNode(const value_type& data, Node* pNext = nullptr)
    {
        return new Node(data, pNext);
    }

    Node** insert_forward(const value_type& elem)
    {
        Node** pParent = findPrev(elem);
        Node* pNew = CreateNode(elem);
        ::CreateBridge(*pParent, pNew, &Node::m_pNext);
        if (!pNew->getpNext())
            m_pTail = pNew;
        return pParent;
    }

public:
    value_type PopHead()
    {
        if (m_pHead)
        {
            Node* pNode = m_pHead;
            value_type data = pNode->getData();
            m_pHead = m_pHead->getpNext();
            delete pNode;
            m_size--;
            if (!m_size)
                m_pTail = nullptr;
            return data;
        }
        throw std::runtime_error("LinkedList is empty");
    }
};

#endif
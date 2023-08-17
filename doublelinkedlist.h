#ifndef __DOUBLE_LINKEDLIST_H__
#define __DOUBLE_LINKEDLIST_H__

#include "linkedlist.h"

template <typename Container>
class backward_iterator : public general_iterator<Container, backward_iterator<Container>>
{
public:
    typedef typename Container::Node Node;
    typedef class general_iterator<Container, backward_iterator<Container>> Parent;
    typedef backward_iterator<Container> myself;

public:
    backward_iterator(Container* pContainer, Node* pNode) : Parent(pContainer, pNode) {}
    backward_iterator(myself& other) : Parent(other) {}
    backward_iterator(myself&& other) : Parent(std::move(other)) {}

public:
    backward_iterator operator++()
    {
        Parent::m_pNode = Parent::m_pNode->getpPrev();
        return *this;
    }
};

template <typename T>
class NodeDLL
{
public:
    typedef NodeDLL<T> Node;

private:
    T m_data;
    Node* m_pNext;
    Node* m_pPrev;

public:
    NodeDLL(T data, Node* pNext = nullptr, Node* pPrev = nullptr)
        : m_data(data), m_pNext(pNext), m_pPrev(pPrev)
    {
    }

    void setpNext(Node* pNext) { m_pNext = pNext; }
    Node* getpNext() { return m_pNext; }
    Node*& getpNextRef() { return m_pNext; }

    void setpPrev(Node* pPrev) { m_pPrev = pPrev; }
    Node* getpPrev() { return m_pPrev; }
    Node*& getpPrevRef() { return m_pPrev; }
};

template <typename _T>
struct DLLAscTraits
{
    typedef _T T;
    typedef NodeDLL<T> Node;
    typedef std::less<T> CompareFn;
};

template <typename _T>
struct DLLDescTraits
{
    typedef _T T;
    typedef NodeDLL<T> Node;
    typedef std::greater<T> CompareFn;
};

template <typename Traits>
class DoubleLinkedList : public LinkedList<Traits>
{
public:
    typedef typename Traits::T value_type;
    typedef typename Traits::Node Node;
    typedef typename Traits::CompareFn CompareFn;
    typedef DoubleLinkedList<Traits> myself;
    typedef LinkedList<Traits> Parent;
    typedef forward_iterator<myself> iterator;
    typedef backward_iterator<myself> riterator;

public:
    DoubleLinkedList() {}

    void insert(value_type elem)
    {
        Node* pPrevTail = Parent::m_pTail;
        Node* pNew = *Parent::insert_forward(elem);
        if (pNew != Parent::m_pTail)
            ::CreateBridge(static_cast<Node*>(pNew->getpNext())->getpPrevRef(), pNew, &Node::m_pPrev);
        else
            pNew->setpPrev(pPrevTail);
    }

    riterator rbegin() { riterator iter(this, Parent::m_pTail); return iter; }
    riterator rend() { riterator iter(this, nullptr); return iter; }

    void push_front(value_type elem)
    {
        Parent::push_front(elem);
        if (Parent::size() > 1)
            static_cast<Node*>(Parent::m_pHead->getpNext())->setpPrev(Parent::m_pHead);
    }

    void push_back(value_type elem)
    {
        Node* pPrevTail = Parent::m_pTail;
        Parent::push_back(elem);
        Parent::m_pTail->setpPrev(pPrevTail);
    }
};

#endif
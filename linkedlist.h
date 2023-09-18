#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__
#include <utility>
#include <algorithm>
#include <exception>
#include <iostream>
#include <cassert>
#include "iterator.h"
#include "foreach.h"
#include "types.h"
using namespace std;

template <typename Node, typename MemberType>
void CreateBridge(Node *&rParent, Node *pNew, MemberType _pMember)
{
    Node *Node::*pMember = (Node * Node::*)_pMember;
    pNew->*pMember = rParent;
    rParent = pNew;
}

template <typename Traits>
class NodeLinkedList
{
  public:
    using KeyType = typename Traits::value_type;
    using Type = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using KeyNode = typename Traits::Node;
  private:
    using T = typename Traits::value_type;
    typedef NodeLinkedList<Traits> Node;

public:
    // DONE change T y KeyNode (Viene de XTrait)
    KeyNode m_data;
    Node *m_pNext; //
public:
    NodeLinkedList(const T &data, const LinkedValueType &v,  Node *pNext = nullptr)
        : m_data(data, v), m_pNext(pNext){};
    // DONE: Move to KeyNode, ahora estas funciones son solo helpers
    // Para llamar a las funciones de KeyNode
    T getData() { return m_data.getData(); }
    T &getDataRef() { return m_data.getDataRef(); }
    LinkedValueType  getValue() const { return m_data.getValue(); }
    LinkedValueType& getValueRef()    { return m_data.getValueRef(); }

    void setpNext(NodeLinkedList *pNext) { m_pNext = pNext; }
    Node *getpNext() { return getpNextRef(); }
    Node *&getpNextRef() { return m_pNext; }

    KeyNode &getDataNode() { return m_data; }
};

// DONE remove general_iterator
template <typename Container>
class forward_iterator
{
private:
    typedef typename Container::Node    Node;
    typedef typename Node::Type         Type;
    typedef typename Node::Type         value_type;
    typedef forward_iterator<Container> myself;
protected:
    Container *m_pContainer;
    Node      *m_pNode;
public:

    forward_iterator(Container *pContainer, Node *pNode) : m_pContainer(pContainer), m_pNode(pNode) {}
    forward_iterator(myself &other) : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode){}
    forward_iterator(myself &&other) // Move constructor
    {   m_pContainer = move(other.m_pContainer);
        m_pNode      = move(other.m_pNode);
    }

    myself operator=(myself &iter)
    {   m_pContainer = move(iter.m_pContainer);
        m_pNode      = move(iter.m_pNode);
        return *(myself *)this; // Pending static_cast?
    }
    bool operator==(myself iter)   { return m_pNode == iter.m_pNode; }
    bool operator!=(myself iter)   { return !(*this == iter);        }
    Type &operator*()                    { return m_pNode->getDataRef();   }

public:
    forward_iterator operator++()
    {
        m_pNode = (Node *)m_pNode->getpNext();
        return *this;
    }
};

template <typename Traits>
class LinkedList
{
public:
    using value_type = typename Traits::value_type;
    using KeyType = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using KeyNode = typename Traits::Node;
    using Node = NodeLinkedList<Traits>;
    using CompareFn = typename Traits::CompareFn;
    typedef LinkedList<Traits> myself;
    typedef forward_iterator<myself> iterator;

protected:
    Node *m_pHead = nullptr,
         *m_pTail = nullptr;
    size_t m_size = 0;
    CompareFn Compfn;

public:
    size_t size() const { return m_size; }
    bool empty() const { return size() == 0; }

public:
    LinkedList() {}
    // DONE add LinkedValueType value
    void insert(const value_type &key, const LinkedValueType &val) { insert_forward(key, val); }
    value_type &operator[](size_t pos)
    {
        assert(pos <= size());
        Node *pTmp = m_pHead;
        for (auto i = 0; i < pos; i++)
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

    template <typename Callable, typename... Args>
    void foreach (Callable op, Args && ...args)
    {
        ::foreach(begin(), end(), op, args...);
    }

    void push_front(const value_type &elem, const LinkedValueType &val)
    {
        Node *pNew = CreateNode(elem, val);
        pNew->setpNext(m_pHead);
        m_pHead = pNew;
        m_size++;
        if (m_size == 1)
            m_pTail = pNew;
    }
    void push_back(const value_type &elem, const LinkedValueType &val)
    {
        Node *pNew = CreateNode(elem, val);
        if (m_pTail)
            m_pTail->setpNext(pNew);
        m_pTail = pNew;
        if (!m_pHead)
            m_pHead = pNew;
        m_size++;
    }

protected:
    Node **findPrev(const value_type &elem) { return findPrev(m_pHead, elem); }
    Node **findPrev(Node *&rpPrev, const value_type &elem)
    {
        if (!rpPrev || !Compfn(KeyNode(elem, LinkedValueType()), rpPrev->getDataNode()))
            return &rpPrev; // Retorna la direccion del puntero que me apunta
        return findPrev((Node *&)rpPrev->getpNextRef(), elem);
    }
    Node *CreateNode(const value_type &data, const LinkedValueType &v, Node *pNext = nullptr) { return new Node(data, v, pNext); }
    Node **insert_forward(const value_type &elem, const LinkedValueType &v)
    {
        Node **pParent = findPrev(elem);
        Node *pNew = CreateNode(elem, v);
        ::CreateBridge(*pParent, pNew, &Node::m_pNext);
        if (!pNew->getpNext())
            m_pTail = pNew;
        m_size++;
        return pParent;
    }
    void destroy()
    {
        while (m_pHead)
        {
            PopHead();
        }
    }

public:
    value_type PopHead()
    {
        if (m_pHead)
        {
            Node *pNode = m_pHead;
            value_type data = pNode->getData();
            m_pHead = m_pHead->getpNext();
            delete pNode;
            m_size--;
            if (!m_size)
                m_pTail = nullptr;
            return data;
        }
        throw "Empty list";
    }
    // DONE add print
    void print(ostream &os)
    {
        Node *pTmp = m_pHead;
        if(!pTmp){
            os << "*" << endl;
            return;
        }
        while (pTmp)
        {
            os << "(" << pTmp->getData() << " " << pTmp->getValue() << ")->";
            pTmp = pTmp->getpNext();
        }
        os << "*" << endl;
    }

    void read(istream &is) {
        destroy();
        while(!is.eof()) {
            KeyType key;
            LinkedValueType val;
            string colon;
            is >> key >> colon >> val;
            insert(key, val);
        }
    }

    template <typename T>
    friend ostream &operator<<(ostream &os, LinkedList<T> &obj);

    template <typename T>
    friend istream &operator>>(istream &is, LinkedList<T> &obj);
};

// DONE add operator<<
template <typename T>
ostream &operator<<(ostream &os, LinkedList<T> &obj)
{
    obj.print(os);
    return os;
}

// DONE: add operator>>
template <typename T>
istream &operator>>(istream &is, LinkedList<T> &obj)
{
    obj.read(is);
    return is;
}


#endif
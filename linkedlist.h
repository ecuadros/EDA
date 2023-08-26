#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__
#include <utility>
#include <algorithm>
#include <cassert>
#include "iterator.h"
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
    using LinkedValueType = typename Traits::LinkedValueType;
    using Node = typename Traits::Node;
  private:
    using T = typename Traits::value_type;
    typedef NodeLinkedList<Traits> LLNode;

public:
    // DONE change T y KeyNode
    Node m_data;
    LLNode *m_pNext; //
public:
    NodeLinkedList(const T &data, const LinkedValueType &v,  LLNode *pNext = nullptr)
        : m_data(data, v), m_pNext(pNext){};
    // DONE: Move to KeyNode, ahora estas funciones son solo helpers
    // Para llamar a las funciones de KeyNode
    T getData() { return m_data.getData(); }
    T &getDataRef() { return m_data.getDataRef(); }
    LinkedValueType  getValue() const { return m_data.getValue(); }
    LinkedValueType& getValueRef()    { return m_data.getValueRef(); }

    void setpNext(NodeLinkedList *pNext) { m_pNext = pNext; }
    LLNode *getpNext() { return getpNextRef(); }
    LLNode *&getpNextRef() { return m_pNext; }
};

// DONE remove general_iterator : Ya no aplica, el macro que se agrega
// como parte de otro caso, usa informacion de general_iterator
template <typename Container>
class forward_iterator : public general_iterator<Container, class forward_iterator<Container>> //
{
public:
    // typedef class general_iterator<Container, forward_iterator<Container>> Parent;
    // typedef typename Container::Node Node; //
    // typedef forward_iterator<Container> myself;

    // DONE: subir al padre, ya no aplica, se usa macro
    _ITER_TYPEDEFS(Container, forward_iterator)

public:
    forward_iterator(Container *pContainer, Node *pNode) : Parent(pContainer, pNode) {}
    forward_iterator(myself &other) : Parent(other) {}
    forward_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

public:
    forward_iterator operator++()
    {
        Parent::m_pNode = (Node *)Parent::m_pNode->getpNext();
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
    using Node = typename Traits::Node;
    using LLNode = NodeLinkedList<Traits>;
    using CompareFn = typename Traits::CompareFn;
    typedef LinkedList<Traits> myself;
    typedef forward_iterator<myself> iterator;

protected:
    LLNode *m_pHead = nullptr,
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
        LLNode *pTmp = m_pHead;
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

    void push_front(const value_type &elem, const LinkedValueType &val)
    {
        LLNode *pNew = CreateNode(elem, val);
        pNew->setpNext(m_pHead);
        m_pHead = pNew;
        m_size++;
        if (m_size == 1)
            m_pTail = pNew;
    }
    void push_back(const value_type &elem, const LinkedValueType &val)
    {
        LLNode *pNew = CreateNode(elem, val);
        if (m_pTail)
            m_pTail->setpNext(pNew);
        m_pTail = pNew;
        if (!m_pHead)
            m_pHead = pNew;
        m_size++;
    }

protected:
    LLNode **findPrev(const value_type &elem) { return findPrev(m_pHead, elem); }
    LLNode **findPrev(LLNode *&rpPrev, const value_type &elem)
    {
        if (!rpPrev || !Compfn(elem, rpPrev->getData()))
            return &rpPrev; // Retorna la direccion del puntero que me apunta
        return findPrev((LLNode *&)rpPrev->getpNextRef(), elem);
    }
    LLNode *CreateNode(const value_type &data, const LinkedValueType &v, LLNode *pNext = nullptr) { return new LLNode(data, v, pNext); }
    LLNode **insert_forward(const value_type &elem, const LinkedValueType &v)
    {
        LLNode **pParent = findPrev(elem);
        LLNode *pNew = CreateNode(elem, v);
        ::CreateBridge(*pParent, pNew, &LLNode::m_pNext);
        if (!pNew->getpNext())
            m_pTail = pNew;
        m_size++;
        return pParent;
    }

public:
    value_type PopHead()
    {
        if (m_pHead)
        {
            LLNode *pNode = m_pHead;
            value_type data = pNode->getData();
            m_pHead = m_pHead->getpNext();
            delete pNode;
            m_size--;
            if (!m_size)
                m_pTail = nullptr;
            return data;
        }
        throw "hola excepcion"; // Create custom exception pending
    }
    // DONE add print
    void print(ostream &os)
    {
        LLNode *pTmp = m_pHead;
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
    // DONE: Read from stream
    using KeyType = typename LinkedList<T>::KeyType;
    using LinkedValueType = typename LinkedList<T>::LinkedValueType;
    KeyType key;
    LinkedValueType val;
    is >> key >> val;
    obj.insert(key, val);
    return is;
}


#endif
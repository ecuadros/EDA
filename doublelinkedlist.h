#ifndef __DOUBLE_LINKEDLIST_H__
#define __DOUBLE_LINKEDLIST_H__

#include "iterator.h"
#include "keynode.h"
#include "linkedlist.h"

template <typename Container> //
class doubleLinkedList_iterator
{
public:
  typedef typename Container::Node Node;
  typedef typename Node::Type Type;
  // typedef class general_iterator<Container> Parent;
  typedef doubleLinkedList_iterator<Container> myself; //

protected:
  Container *m_pContainer;
  Node *m_pNode;

public:
  doubleLinkedList_iterator(Container *pContainer, Node *pNode)
      : m_pContainer(pContainer), m_pNode(pNode) {}
  doubleLinkedList_iterator(myself &other)
      : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode) {}
  doubleLinkedList_iterator(myself &&other) // Move constructor
  {
    m_pContainer = move(other.m_pContainer);
    m_pNode = move(other.m_pNode);
  }

  bool operator==(myself iter) { return m_pNode == iter.m_pNode; }
  bool operator!=(myself iter) { return !(*this == iter); }
  Node *operator*() { return m_pNode; }

  myself operator++()
  {
    m_pNode = m_pNode->getpNext();
    return *this;
  }
};


template <typename Container> //
class doubleLinkedList_riterator
{
public:
  typedef typename Container::Node Node;
  typedef typename Node::Type Type;
  // typedef class general_iterator<Container> Parent;
  typedef doubleLinkedList_riterator<Container> myself; //

protected:
  Container *m_pContainer;
  Node *m_pNode;

public:
  doubleLinkedList_riterator(Container *pContainer, Node *pNode)
      : m_pContainer(pContainer), m_pNode(pNode) {}
  doubleLinkedList_riterator(myself &other)
      : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode) {}
  doubleLinkedList_riterator(myself &&other) // Move constructor
  {
    m_pContainer = move(other.m_pContainer);
    m_pNode = move(other.m_pNode);
  }

  bool operator==(myself iter) { return m_pNode == iter.m_pNode; }
  bool operator!=(myself iter) { return !(*this == iter); }
  Node *operator*() { return m_pNode; }

  myself operator++()
  {
    m_pNode = m_pNode->getpPrev();
    return *this;
  }
};

// TODO Remove inheritance
template <typename T, typename V>
class NodeDLL
{
public:
  // typedef T               Type;
  using value_type = T;
  using Type = T;
  using LinkedValueType = V;

private:
  typedef NodeDLL<T, T> Node;

public:
  KeyNode<T, V> m_data;
  Node *m_pNext; //
  Node *m_pPrev; //
public:
  NodeDLL(KeyNode<T, V> data, Node *pNext = nullptr, Node *pPrev = nullptr)
      : m_data(data), m_pNext(pNext), m_pPrev(pPrev)
  {
  }

  NodeDLL(value_type key, LinkedValueType value, Node *pNext = nullptr, Node *pPrev = nullptr)
  {
    m_data = KeyNode(key, value);
    m_pNext = pNext;
    m_pPrev = pPrev;
  }

  value_type getData() { return m_data.getData(); }
  value_type &getDataRef() { return m_data.getDataRef(); }

  LinkedValueType getValue() { return m_data.getValue(); }
  LinkedValueType &getValueRef() { return m_data.getValueRef(); }

  void setpPrev(Node *pPrev) { m_pPrev = pPrev; }
  Node *getpPrev() { return getpPrevRef(); }
  Node *&getpPrevRef() { return m_pPrev; }

  void setpNext(Node *pNext) { m_pNext = pNext; }
  Node *getpNext() { return getpNextRef(); }
  Node *&getpNextRef() { return m_pNext; }
};

template <typename _T, typename _V>
struct DLLAscTraits
{
  typedef _T T;
  typedef _V V;
  typedef NodeDLL<T, V> Node;
  using CompareFn = less<T>;
  using LinkedValueType = _V;
};

template <typename _T, typename _V>
struct DLLDescTraits
{
  typedef _T T;
  typedef _V V;
  typedef NodeDLL<T, V> Node;
  using CompareFn = greater<T>;
  using LinkedValueType = _V;
};

template <typename Traits>
class DoubleLinkedList
{
public:
  typedef typename Traits::T value_type;
  typedef typename Traits::Node Node;
  typedef typename Traits::LinkedValueType LinkedValueType;
  typedef typename Traits::CompareFn CompareFn;
  typedef DoubleLinkedList<Traits> myself;
  typedef doubleLinkedList_iterator<myself> iterator;
  typedef doubleLinkedList_riterator<myself> riterator;

  using nodeDLL = NodeDLL<value_type, LinkedValueType>;

protected:
  Node *m_pHead = nullptr,
       *m_pTail = nullptr;
  size_t m_size = 0;
  CompareFn Compfn;

public:
  size_t size() const { return m_size; }
  bool empty() const { return size() == 0; }

public:
  DoubleLinkedList() {}
  void insert(value_type elem, LinkedValueType value)
  {
    Node *pPrevTail = m_pTail;
    Node **pParent = findPrev(elem);
    Node *pNew = CreateNode(elem, value);
    ::CreateBridge(*pParent, pNew, &Node::m_pNext);
    if (!pNew->getpNext()) {
      m_pTail = pNew;
    }

    if( pNew != m_pTail )
        ::CreateBridge( (pNew->getpNext())->getpPrevRef(), pNew, &Node::m_pPrev);
    else
        pNew->setpPrev(pPrevTail);

    m_size++;  
  }

  Node **findPrev(value_type &elem) { return findPrev(m_pHead, elem); }
  Node **findPrev(Node *&rpPrev, value_type &elem)
  {
    if (!rpPrev || Compfn(elem, rpPrev->getData()) )
      return &rpPrev; // Retorna la direccion del puntero que me apunta
    return findPrev((Node *&)rpPrev->getpNextRef(), elem);
  }

  Node *CreateNode(value_type &data, LinkedValueType &value, Node *pNext = nullptr, Node *pPrev = nullptr) { return new Node(data, value, pNext, pPrev); }
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
  riterator rbegin()
  {
    riterator iter(this, m_pTail);
    return iter;
  }
  riterator rend()
  {
    riterator iter(this, nullptr);
    return iter;
  }

  Node *getPos(size_t pos)
  {
    assert(pos <= size());
    Node *pTmp = m_pHead;
    for (auto i = 0; i < pos; i++)
      pTmp = pTmp->getpNext();
    return pTmp;
  }

  void push_front(value_type elem)
  {
    push_front(elem);
    if (size() > 1)
      ((Node *)m_pHead->m_pNext)->m_pPrev = m_pHead;
  }
  void push_back(value_type elem)
  {
    Node *pPrevTail = m_pTail;
    push_back(elem);
    m_pTail->setPrev(pPrevTail);
  }

  void _push_back(value_type elem, LinkedValueType value)
  {
    Node *pNew = CreateNode(elem, value);
    if (m_pTail)
      m_pTail->setpNext(pNew);
    m_pTail = pNew;
    if (!m_pHead)
      m_pHead = pNew;
    m_size++;
  }

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
    throw "hola excepcion"; // Create custom exception pending
  }

  void print(ostream &os)
  {
    nodeDLL *pTmp = m_pHead;
    // // auto iter = begin();
    // // for(; iter != end() ; ++iter)
    // //   os << "{" << *iter << "," << pTmp->getValue() << "}";
    // //   pTmp = pTmp->getpNext();

    while (pTmp) {
      os << "{" << pTmp->getData() << ", " << pTmp->getValue() << "}, ";
      pTmp = pTmp->getpNext();
    }

  }

  void read(istream &is)
  {

    while (m_pHead)
    {
      this->PopHead();
    }
    string str;
    value_type key;
    LinkedValueType value;
    getline(is, str); // skip the first line
    while (getline(is, str))
    {
      istringstream iss(str);
      iss >> key >> value;
      insert(key, value);
    }
  }
};

template <typename T>
inline ostream &operator<<(ostream &os, DoubleLinkedList<T> &obj)
{
  obj.print(os);
  return os;
}

template <typename T>
inline istream &operator>>(istream &is, DoubleLinkedList<T> &obj)
{
  obj.read(is);
  return is;
}

#endif
#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__
#include <utility>
#include <algorithm>
#include <cassert>
#include "iterator.h"
#include "types.h"
#include "keynode.h"
#include <fstream>
#include <sstream>
using namespace std;

template <typename Node, typename MemberType>
void CreateBridge(Node *&rParent, Node *pNew, MemberType _pMember)
{
  Node *Node::*pMember = (Node * Node::*)_pMember;
  pNew->*pMember = rParent;
  rParent = pNew;
}

template <typename T, typename V>
class NodeHash
{
public:
  using value_type = T;
  using Type = T;
  using LinkedValueType = V;

private:
  typedef NodeHash<T, V> Node;

public:
  // TODO change T y KeyNode
  KeyNode<T, V> m_data;
  Node *m_pNext; //
public:
  NodeHash(KeyNode<T, V> data, Node *pNext = nullptr)
      : m_data(data), m_pNext(pNext){};
  NodeHash(value_type key, LinkedValueType value, Node *pNext = nullptr)
  {
    m_data = KeyNode(key, value);
    m_pNext = pNext;
  }
  // TODO Move to KeyNode
  value_type getData() { return m_data.getData(); }
  value_type &getDataRef() { return m_data.getDataRef(); }

  LinkedValueType getValue() { return m_data.getValue(); }
  LinkedValueType &getValueRef() { return m_data.getValueRef(); }

  void setpNext(NodeHash *pNext) { m_pNext = pNext; }
  Node *getpNext() { return getpNextRef(); }
  Node *&getpNextRef() { return m_pNext; }
};

template <typename Container> //
class hash_iterator
{
public:
  typedef typename Container::Node Node;
  typedef typename Node::Type Type;
  // typedef class general_iterator<Container> Parent;
  typedef hash_iterator<Container> myself; //

protected:
  Container *m_pContainer;
  Node *m_pNode;

public:
  hash_iterator(Container *pContainer, Node *pNode)
      : m_pContainer(pContainer), m_pNode(pNode) {}
  hash_iterator(myself &other)
      : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode) {}
  hash_iterator(myself &&other) // Move constructor
  {
    m_pContainer = move(other.m_pContainer);
    m_pNode = move(other.m_pNode);
  }

  bool operator==(myself iter) { return m_pNode == iter.m_pNode; }
  bool operator!=(myself iter) { return !(*this == iter); }
  Node *operator*() { return m_pNode; }

  myself operator++()
  {
    m_pNode = m_pNode->getpNext(); return *this;
  }
};

template <typename _T, typename _V>
struct HTraitDesc
{
  using T = _T;
  using V = _V;
  using Node = NodeHash<T, V>;
  using CompareFn = less<T>;
  using LinkedValueType = _V;
};

template <typename _T, typename _V>
struct HTraitAsc
{
  using T = _T;
  using V = _V;
  using Node = NodeHash<T, V>;
  using CompareFn = greater<T>;
  using LinkedValueType = _V;
};

template <typename Traits>
class Hash
{
public:
  typedef typename Traits::T value_type;
  typedef typename Traits::Node Node;
  typedef typename Traits::LinkedValueType LinkedValueType;

  typedef typename Traits::CompareFn CompareFn;
  typedef Hash<Traits> myself;
  typedef hash_iterator<myself> iterator;

  using nodeHash = NodeHash<value_type, LinkedValueType>;

protected:
  Node *m_pHead = nullptr,
       *m_pTail = nullptr;
  size_t m_size = 0;
  CompareFn Compfn;

public:
  size_t size() const { return m_size; }
  bool empty() const { return size() == 0; }

public:
  Hash() {}
  // TODO add LinkedValueType value
  void insert(value_type key, LinkedValueType value) { insert_forward(key, value); }

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

  void push_front(value_type elem)
  {
    Node *pNew = CreateNode(elem);
    pNew->setpNext(m_pHead);
    m_pHead = pNew;
    m_size++;
    if (m_size == 1)
      m_pTail = pNew;
  }


protected:
  Node **findPrev(value_type &elem) { return findPrev(m_pHead, elem); }
  Node **findPrev(Node *&rpPrev, value_type &elem)
  {
    if (!rpPrev || Compfn(elem, rpPrev->getData()))
      return &rpPrev; // Retorna la direccion del puntero que me apunta
    return findPrev((Node *&)rpPrev->getpNextRef(), elem);
  }
  Node *CreateNode(value_type &data, LinkedValueType &value, Node *pNext = nullptr) { return new Node(data, value, pNext); }

  Node **insert_forward(value_type &elem, LinkedValueType &value)
  {
    Node **pParent = findPrev(elem);
    Node *pNew = CreateNode(elem, value);
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
  // TODO add print
  void print(ostream &os)
  {
    nodeHash *pTmp = m_pHead;
    // // auto iter = begin();
    // // for(; iter != end() ; ++iter)
    // //   os << "{" << *iter << "," << pTmp->getValue() << "}";
    // //   pTmp = pTmp->getpNext();

    while (pTmp) {
      os << "[" << pTmp->getData() << " = " << pTmp->getValue() << "] " << endl;
      pTmp = pTmp->getpNext();
    }

  }

  void read(istream &is) {

    while (m_pHead) {
      this->PopHead();
    }
    string str;
    string separador;
    value_type key;
    LinkedValueType value;
    getline(is, str); // skip the first line
    while (getline(is, str)) {
      istringstream iss(str);
      iss >> key >> separador >> value;
      insert_forward(key, value);
    }
    
  }
};

// TODO add operator<<
template <typename T>
ostream &operator<<(ostream &os, Hash<T> &obj)
{
  obj.print(os);
  return os;
}
// TODO add operator>>
template <typename T>
istream &operator>>(istream &is, Hash<T> &obj)
{
  obj.read(is);
  return is;
}

#endif
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
class NodeGraph
{
public:
  using value_type = T;
  using Type = T;
  using LinkedValueType = V;

private:
  typedef NodeGraph<T, V> Node;

public:
  // TODO change T y KeyNode
  KeyNode<T, V> m_data;
  Node *m_pNext; //
  TX m_edge;
public:
  NodeGraph(KeyNode<T, V> data, Node *pNext = nullptr)
      : m_data(data), m_pNext(pNext){};
  NodeGraph(value_type key, LinkedValueType value, Node *pNext = nullptr, TX edge = 0)
  {
    m_data = KeyNode(key, value);
    m_pNext = pNext;
    m_edge = edge;
  }
  // TODO Move to KeyNode
  value_type getData() { return m_data.getData(); }
  value_type &getDataRef() { return m_data.getDataRef(); }

  TX getEdge() { return m_edge; }
  void setEdge(TX edge) { m_edge = edge; }

  LinkedValueType getValue() { return m_data.getValue(); }
  LinkedValueType &getValueRef() { return m_data.getValueRef(); }

  void setpNext(NodeGraph *pNext) { m_pNext = pNext; }
  Node *getpNext() { return getpNextRef(); }
  Node *&getpNextRef() { return m_pNext; }
};

template <typename _T, typename _V>
struct GraphTrait
{
    using  T         = _T;
    using  V         = _V;
    using  Node      = NodeGraph<T,V>;
    using  CompareFn = less<T>;
    using LinkedValueType = V;
};

template <typename Traits>
class Graph
{
public:
  typedef typename Traits::T value_type;
  typedef typename Traits::Node Node;
  typedef typename Traits::LinkedValueType LinkedValueType;

  typedef typename Traits::CompareFn CompareFn;
  typedef Graph<Traits> myself;

  using nodeGraph = NodeGraph<value_type, LinkedValueType>;

protected:
  Node *m_pHead = nullptr,
       *m_pTail = nullptr;
  size_t m_size = 0;
  CompareFn Compfn;

public:
  size_t size() const { return m_size; }
  bool empty() const { return size() == 0; }

public:
  Graph() {}
  // TODO add LinkedValueType value
  Node *findVertex(value_type elem) {
    Node *pTmp = m_pHead;
    cout << "Elemento a buscar " << elem << endl;
    while (pTmp) {
        cout << "getData " << pTmp->getData() << "elem " << elem << endl;
      if ( pTmp->getData() == elem ) {
        return pTmp;
        break;
      }
      pTmp = pTmp->getpNext();
    }
    return pTmp;
  }
  void insertEdge(value_type vertex, LinkedValueType LinkedValueType, TX edges) { push_front(vertex, LinkedValueType, edges); }

  void push_front(value_type elem, LinkedValueType LinkedValueType, TX edges)
  {
    Node *pNew = CreateNode(elem, LinkedValueType);
    pNew->setpNext(m_pHead);
    pNew->setEdge(edges);
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
    nodeGraph *pTmp = m_pHead;
    // // auto iter = begin();
    // // for(; iter != end() ; ++iter)
    // //   os << "{" << *iter << "," << pTmp->getValue() << "}";
    // //   pTmp = pTmp->getpNext();

    while (pTmp) {
      os << "(" << pTmp->getData() << " -> " << pTmp->getValue() << ") : " << pTmp->getEdge() << endl;
      pTmp = pTmp->getpNext();
    }

  }

  void read(istream &is) {

    while (m_pHead) {
      this->PopHead();
    }
    string str;
    value_type key;
    LinkedValueType value;
    TX edge;
    while (getline(is, str)) {
      istringstream iss(str);
      iss >> key >> value >> edge;
      push_front(key, value, edge);
    }
    
  }
};

// TODO add operator<<
template <typename T>
ostream &operator<<(ostream &os, Graph<T> &obj)
{
  obj.print(os);
  return os;
}
// TODO add operator>>
template <typename T>
istream &operator>>(istream &is, Graph<T> &obj)
{
  obj.read(is);
  return is;
}

#endif
#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__
#include <utility>
#include <algorithm>
#include <cassert>
//#include "iterator.h"
#include "types.h"
#include "keynode.h"
#include <iostream>
using namespace std;

template <typename Node, typename MemberType>
void CreateBridge(Node *&rParent, Node *pNew, MemberType _pMember)
{
  Node *Node::*pMember = (Node * Node::*)_pMember;
  pNew->*pMember = rParent;
  rParent = pNew;
}

// Esta clase debe admitir un formato keyNode < T, V>
template <typename T, typename V>
class NodeLinkedList
{
public:
  // De acuerdo a la nomenclatura manejada anteriormente:
  typedef T Type;

private:
  typedef NodeLinkedList < T, V > Node;

public:
  // TODO change T y KeyNode
  // LinkedList almacenará valores de tipo KeyNode en m_data.
  // y *m_pNext almacenará el puntero al siguiente valor de la lista enlazada.
  KeyNode<T, V> m_data;
  Node *m_pNext;

public:
  // Se declara el constructor, el cual recibirá un valor KeyNode y por defecto
  // tiene un puntero nulo (al siguiente elemento).
  NodeLinkedList(KeyNode<T, V> data, Node *pNext = nullptr)
  {
    m_data = data;
    m_pNext = pNext;
  }

  // En caso se declare la clase sin KeyNode, ésta puede crearlos si le brinda
  // los valores suficientes. Nuevamente, por defecto se le asigna un puntero
  // nulo al siguiente valor de la lista enlazada.
  NodeLinkedList(T keyNode_Value, V keyNode_Key, Node *pNext = nullptr)
  {
    m_data = KeyNode(keyNode_Value, keyNode_Key);
    m_pNext = pNext;
  }

  // TODO Move to KeyNode
  // Se crea la funcion getData que devolvera el valor de m_data, recordar
  // m_data es de tipo KeyNode y la función getData() es del KeyNode.
  T getData()
  {
    return m_data.getData();
  }

  // Se crea la funcion getDataRef que devolvera la referencia de m_data,
  // recordar que m_data es de tipo KeyNode y la función getDataRef() es del KeyNode.
  T &getDataRef()
  {
    return m_data.getDataRef();
  }

  V getKey()
  {
    return m_data.getValue();
  }

  V &getKeyRef()
  {
    return m_data.getValueRef();
  }

  void setpNext(NodeLinkedList *pNext)
  {
    m_pNext = pNext;
  }

  Node *getpNext()
  {
    return getpNextRef();
  }
  Node *&getpNextRef()
  {
    return m_pNext;
  }
};

// TODO remove general_iterator
template <typename Container>
class forward_iterator
{
public:
  // TODO: subir al padre
  typedef typename Container::Node Node; //
  typedef typename Node::Type         Type;
  typedef forward_iterator<Container> myself;

protected:
    Container *m_pContainer;
    Node      *m_pNode;

public:
  forward_iterator(Container *pContainer, Node *pNode)
  {
    m_pContainer = pContainer;
    m_pNode = pNode;
  }
  forward_iterator(myself &other)
  {
    m_pContainer = other.m_pContainer;
    m_pNode = other.m_pNode;
  }
  forward_iterator(myself &&other)
  {
    m_pContainer = move(other.m_pContainer);
    m_pNode = move(other.m_pNode);
  } 
public:
  forward_iterator operator++()
  {
    m_pNode = m_pNode->getpNext();
    return *this;
  }

  myself operator=(myself &iter)
  {
    m_pContainer = move(iter.m_pContainer);
    m_pNode = move(iter.m_pNode);
    return *(forward_iterator *) this;
  }
  bool operator==(myself iter)
  {
      return m_pNode == iter.m_pNode;
  }

  bool operator!=(myself iter)
  {
    return m_pNode != iter.m_pNode;
  }

  Type &operator*()
  {
    return m_pNode -> getDataRef();
  }
};

template <typename _T, typename _V>
struct LLTraitAsc
{
  using T = _T;
  using V = _V;
  using Node = NodeLinkedList<T, V>;
  using CompareFn = less<T>;
};

template <typename _T, typename _V>
struct LLTraitDesc
{
  using T = _T;
  using V = _V;
  using Node = NodeLinkedList<T, V>;
  using CompareFn = greater<T>;
};

template <typename Traits>
class LinkedList
{
public:
  typedef typename Traits::T value_type;
  typedef typename Traits::V key_type;
  typedef typename Traits::Node Node;

  typedef typename Traits::CompareFn CompareFn;
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
  // TODO add LinkedValueType value

  // En insert se ha de agregar los valores necesarios para crear el Nodo.
  void insert(value_type nodeValue, key_type nodeKey)
  {
    insert_forward(nodeValue, nodeKey);
  }

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

  void push_front(value_type elem)
  {
    Node *pNew = CreateNode(elem);
    pNew->setpNext(m_pHead);
    m_pHead = pNew;
    m_size++;
    if (m_size == 1)
      m_pTail = pNew;
  }
  void push_back(value_type elem)
  {
    Node *pNew = CreateNode(elem, nullptr);
    if (m_pTail)
      m_pTail->setpNext(pNew);
    m_pTail = pNew;
    if (!m_pHead)
      m_pHead = pNew;
    m_size++;
  }

protected:
  Node **findPrev(value_type &elem) { return findPrev(m_pHead, elem); }
  Node **findPrev(Node *&rpPrev, value_type &elem)
  {
    if (!rpPrev || Compfn(elem, rpPrev->getData()))
      return &rpPrev; // Retorna la direccion del puntero que me apunta
    return findPrev((Node *&)rpPrev->getpNextRef(), elem);
  }

  Node *CreateNode(value_type &nodeValue, key_type nodeKey, Node *pNext = nullptr)
  {
    return new Node(nodeValue, nodeKey, pNext);
  }

  Node **insert_forward(value_type nodeValue, key_type nodeKey)
  {
    Node **pParent = findPrev(nodeValue);
    Node *pNew = CreateNode(nodeValue, nodeKey);
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
  void print( ostream &os) const
  {
    Node *pTmp = m_pHead;    
      while (pTmp) 
      {
        os << pTmp->getData() << " " << pTmp->getKey() << endl;
        pTmp = pTmp->getpNext();
      }
  }


  void read( istream &is) 
  {
    value_type  value;
    key_type key;
    while (is.eof() == false)
        {           
            is >> value >> key; 
            insert(value, key) ;
        }
  }
};

// TODO add operator<<
template <typename Traits>
ostream &operator <<(ostream &os, const LinkedList<Traits> &obj)
{
  obj.print(os);
  return os;
}

// TODO add operator>>
template <typename Traits>
istream &operator >>(istream &is, LinkedList<Traits> &obj)
{
  obj.read(is);
  return is;
}

#endif
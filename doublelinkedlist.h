#ifndef __DOUBLE_LINKEDLIST_H__
#define __DOUBLE_LINKEDLIST_H__

#include "linkedlist.h"
#include "keynode.h"

template <typename Container>
class backward_iterator
{
public:
  // TODO: subir al padre
  typedef typename Container::Node Node; //
  typedef typename Node::Type Type;
  typedef backward_iterator<Container> myself;

protected:
    Container *m_pContainer;
    Node      *m_pNode;

public:
  backward_iterator(Container *pContainer, Node *pNode)
  {
    m_pContainer = pContainer;
    m_pNode = pNode;
  }

  backward_iterator(myself &other)
  {
    m_pContainer = other.m_pContainer;
    m_pNode = other.m_pNode;
  }

  backward_iterator(myself &&other)
  {
    m_pContainer = move(other.m_pContainer);
    m_pNode = move(other.m_pNode);
  }
public:
  backward_iterator operator++()
  {
    m_pNode = m_pNode->getpPrev();
    return *this;
  }

  myself operator=(myself &iter)
  {
    m_pContainer = move(iter.m_pContainer);
    m_pNode = move(iter.m_pNode);
    return *(backward_iterator *) this;
  }

  bool operator==(myself iter)
  {
      return m_pNode == iter.m_pNode;
  }

  bool operator!=(myself iter)
  {
    return m_pNode != iter.m_pNode;
  }
  //TEEEEEEEEEEEEEEEEEEEEEEEST
  Type &operator*()
  {
    return m_pNode ;
  }

};

// TODO Remove inheritance
template <typename T, typename V>
class NodeDLL
{
public:
  typedef T               Type;
private:
  typedef NodeDLL<T, V> Node;
public:
  KeyNode <T, V> m_data;
  Node *m_pPrev;
  Node *m_pNext;

public:
  NodeDLL(KeyNode<T, V> data, Node *pNext = nullptr, Node *pPrev = nullptr)
  {
    m_data = data;
    m_pNext = pNext;
    m_pPrev = pPrev;
  }

  NodeDLL(T keyNode_Value, V keyNode_Key, Node *pNext = nullptr, Node *pPrev = nullptr)
  {
    m_data = KeyNode(keyNode_Value, keyNode_Key);
    m_pNext = pNext;
    m_pPrev = pPrev;
  }

  T getData() { return m_data.getData(); }
  T &getDataRef() { return m_data.getDataRef();}

  V getKey() { return m_data.getValue();}
  V &getKeyRef() { return m_data.getValueRef();}

  void setpPrev(Node *pPrev) { m_pPrev = pPrev;}
  Node *getpPrev() { return getpPrevRef();}
  Node *&getpPrevRef(){ return m_pPrev;}

  void setpNext(Node *pNext) { m_pNext = pNext;}
  Node *getpNext() { return getpNextRef();}
  Node *&getpNextRef(){ return m_pNext;}
};

template <typename _T, typename _V>
struct DLLAscTraits
{
  typedef _T T;
  typedef _V V;
  typedef NodeDLL<T, V> Node;
  typedef less<T> CompareFn;
};

template <typename _T, typename _V>
struct DLLDescTraits
{
  typedef _T T;
  typedef _V V;
  typedef NodeDLL<T, V> Node;
  typedef greater<T> CompareFn;
};

// TODO remove inheritance
template <typename Traits>
class DoubleLinkedList
{
public:
  typedef typename Traits::T value_type;
  typedef typename Traits::V key_type;
  typedef typename Traits::Node Node;
  typedef typename Traits::CompareFn CompareFn;

  typedef DoubleLinkedList<Traits> myself;
  typedef forward_iterator<myself> iterator;
  typedef backward_iterator<myself> riterator;

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
  void insert(value_type nodeValue, key_type nodeKey)
  {
    Node *pPrevTail = m_pTail;
    Node *pNew = *insert_forward(nodeValue, nodeKey);
    if (pNew != m_pTail)
      ::CreateBridge(pNew->getpNext()->getpPrevRef(), pNew, &Node::m_pPrev);
    else
      pNew->setpPrev(pPrevTail);
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
  void push_front(value_type elem)
  {
    push_front(elem);
    if (size() > 1)
      ( *m_pHead->m_pNext)->m_pPrev = m_pHead;
  }
  void push_back(value_type elem)
  {
    Node *pPrevTail = m_pTail;
    push_back(elem);
    m_pTail->setpPrev(pPrevTail);
  }

  protected:
  Node **findPrev(value_type &elem) { return findPrev(m_pHead, elem); }
  Node **findPrev(Node *&rpPrev, value_type &elem)
  {
    if (!rpPrev || Compfn(elem, rpPrev->getData()))
      return &rpPrev; // Retorna la direccion del puntero que me apunta
    return findPrev((Node *&)rpPrev->getpNextRef(), elem);
  }

  Node *CreateNode(value_type &nodeValue, key_type &nodeKey, Node *pNext = nullptr, Node *pPrev = nullptr)
  {
    return new Node(nodeValue, nodeKey, pNext, pPrev);
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
ostream &operator <<(ostream &os, const DoubleLinkedList<Traits> &obj)
{
  obj.print(os);
  return os;
}

// TODO add operator>>
template <typename Traits>
istream &operator >>(istream &is, DoubleLinkedList<Traits> &obj)
{
  obj.read(is);
  return is;
}

#endif
#ifndef __DOUBLE_LINKEDLIST_H__
#define __DOUBLE_LINKEDLIST_H__

#include "keynode.h"
#include "linkedlist.h"

// TODO: Tarea 25: CLinkedList: remove general iterator
template <typename Container>
class backward_iterator
{
  public:
    typedef typename Container::Node Node;
    typedef typename Node::Type Type;
    typedef backward_iterator<Container> myself;
  protected:
    Container *m_pContainer;
    Node *m_pNode;
  public:
    backward_iterator(Container *pContainer, Node *pNode) : m_pContainer(pContainer), m_pNode(pNode) {}
    backward_iterator(myself &other) : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode) {}
    backward_iterator(myself &&other) {   
      m_pContainer = move(other.m_pContainer);
      m_pNode      = move(other.m_pNode);
    }

  public:
    backward_iterator operator--()
    {
      m_pNode = ((Node *)m_pNode)->getpPrev();
      return *this;
    }

    backward_iterator operator=(backward_iterator &iter)
          {   m_pContainer = move(iter.m_pContainer);
              m_pNode      = move(iter.m_pNode);
              return *(backward_iterator *)this;
          }

    bool operator==(backward_iterator iter)   { return m_pNode == iter.m_pNode; }
    bool operator!=(backward_iterator iter)   { return !(*this == iter);        }
    Node &operator*() { return m_pNode;}
};

template <typename T, typename V>
class NodeDLL
{
public:
  using value_type = T;
  using Type = T;
  using LinkedValueType = V;

private:
  typedef NodeDLL<T, T> Node;

public:
//TODO: Tarea 31: CDoubleLinkedList: add KeyNode
  KeyNode<T, V> m_data;
  Node *m_pNext;
  Node *m_pPrev;
public:
  NodeDLL(value_type key, LinkedValueType value, Node *pNext = nullptr, Node *pPrev = nullptr)
  {
    m_data = KeyNode(key, value);//TODO: Tarea 31: CDoubleLinkedList: add KeyNode
    m_pNext = pNext;
    m_pPrev = pPrev;
  }

  value_type getKey() { return m_data.getData(); }
  value_type &getKeyRef() { return m_data.getDataRef(); }

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
  typedef backward_iterator<myself> iterator;
  typedef backward_iterator<myself> riterator;

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
  //TODO: Tarea 30: CDoubleLinkedList: add 2 param to insert
  void insert(value_type elem, LinkedValueType valor)
  {
    Node **pParent = findPrev(valor);
    Node *pNew = CreateNode(elem, valor);
    ::CreateBridge(*pParent, pNew, &Node::m_pNext);
    if (!pNew->getpNext())
      m_pTail = pNew;
    m_size++;
  }

  Node **findPrev(value_type &valor) { return findPrev(m_pHead, valor); }
  Node **findPrev(Node *&rpPrev, value_type &valor)
  {
    if (!rpPrev || Compfn(valor, rpPrev->getValue()))
      return &rpPrev; // Retorna la direccion del puntero que me apunta
    return findPrev((Node *&)rpPrev->getpNextRef(), valor);
  }

  Node *CreateNode(value_type &data, LinkedValueType &value, Node *pNext = nullptr, Node *pPrev = nullptr) { return new Node(data, value, pNext, pPrev); }
  riterator rbegin(){ riterator iter(this, m_pTail);return iter; }
  riterator rend(){ riterator iter(this, nullptr);return iter; }

  Node *getPos(size_t pos)
  {
    assert(pos <= size());
    Node *pTmp = m_pHead;
    for (auto i = 0; i < pos; i++)
      pTmp = pTmp->getpNext();
    return pTmp;
  }
  
  //TODO: Tarea 29: CDoubleLinkedList: print, read
  void print(ostream &os)
  {
    Node *pActual = m_pHead;
    while (pActual)
    {
      os << pActual->getKey() << "->[" << pActual->getValue() << "] ";
      pActual = pActual->getpNext();
    }
  }

  void read(istream &is)
  {
    int filas;
      is >> filas;
      value_type key;
      LinkedValueType value;
      string puntuacion;
      while (filas--)
      {
        is >> key;
        is >> puntuacion;
        is >> value;
        this->insert(key, value);
      }
  }

  ostream &reverse_print(ostream &os)
  {
    Node *pActual;
    for (size_t i = size(); i > 0; --i)
    {
      pActual = getPos(i-1);
      os << pActual->getKey() << "->[" << pActual->getValue() << "] ";
    }
    os << endl;
    return os;
  }
};

//TODO: Tarea 27: CDoubleLinkedList: operator<<
template <typename T>
ostream &operator<<(ostream &os, DoubleLinkedList<T> &lista)
{
  lista.print(os);
  return os;
}

//TODO: Tarea 28: CDoubleLinkedList: operator>>
template <typename T>
istream &operator>>(istream &is, DoubleLinkedList<T> &lista)
{
  lista.read(is);
  return is;
}

#endif
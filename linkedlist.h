#ifndef __LINKEDLIST_H__  
#define __LINKEDLIST_H__ 
#include <utility>
#include <algorithm>
#include <cassert>
#include "types.h"
#include "keynode.h"
using namespace std;

template <typename Node, typename MemberType>
void CreateBridge(Node *&rParent, Node *pNew, MemberType _pMember)
{
    Node *Node::*pMember = (Node *Node::*)_pMember;
    pNew->*pMember = rParent;
    rParent = pNew; 
}

template <typename T, typename V>
class NodeLinkedList
{
public:
  using value_type   = T;
  using Type      = T;
  using LinkedValueType = V;
private:
  typedef NodeLinkedList<T,V> Node;
  public:
  //TODO: Tarea 26: CLinkedList: add KeyNode
    KeyNode<T,V> m_data;
    Node   *m_pNext;
  public:
    NodeLinkedList(value_type key, LinkedValueType valor, Node *pNext = nullptr){
        m_data = KeyNode(key, valor);//TODO: Tarea 26: CLinkedList: add KeyNode
        m_pNext = pNext;
      }
    value_type         getKey()                {   return m_data.getData();    }
    value_type        &getKeyRef()             {   return m_data.getDataRef();    }

    LinkedValueType         getValue()                {   return m_data.getValue();    }
    LinkedValueType        &getValueRef()             {   return m_data.getValueRef();    }

    void      setpNext(NodeLinkedList *pNext)  {   m_pNext = pNext;  }
    Node     *getpNext()               {   return getpNextRef();   }
    Node    *&getpNextRef()            {   return m_pNext;   }
};

// TODO: Tarea 25: CLinkedList: remove general iterator
template <typename Container>
class forward_iterator
{
  public: 
    typedef typename Container::Node Node;
    typedef typename Node::Type Type;
    typedef forward_iterator<Container> myself;
  protected:
    Container *m_pContainer;
    Node *m_pNode;
  public:
    forward_iterator(Container *pContainer, Node *pNode) : m_pContainer(pContainer), m_pNode(pNode) {}
    forward_iterator(myself &other)  : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode) {}
    forward_iterator(myself &&other) {   
      m_pContainer = move(other.m_pContainer);
      m_pNode      = move(other.m_pNode);
    }

  public:
    forward_iterator operator++() 
    {
      m_pNode = (Node *)m_pNode->getpNext();  
      return *this;
    }

    forward_iterator operator=(forward_iterator &iter)
          {   m_pContainer = move(iter.m_pContainer);
              m_pNode      = move(iter.m_pNode);
              return *(forward_iterator *)this;
          }

    bool operator==(forward_iterator iter)   { return m_pNode == iter.m_pNode; }
    bool operator!=(forward_iterator iter)   { return !(*this == iter);        }
    Type &operator*() { return m_pNode->getKeyRef();}
};

template <typename _T, typename _V>
struct LLTraitAsc
{
    using  T         = _T;
    using  V         = _V;
    using  Node      = NodeLinkedList<T,V>;
    using  CompareFn = less<V>;
    using  LinkedValueType = _V;
};

template <typename _T, typename _V>
struct LLTraitDesc
{
    using  T         = _T;
    using  V         = _V;
    using  Node      = NodeLinkedList<T,V>;
    using  CompareFn = greater<V>;
    using  LinkedValueType = _V;
};

template <typename Traits>
class LinkedList
{
  public:
    typedef typename Traits::T          value_type;
    typedef typename Traits::Node       Node;
    typedef typename Traits::LinkedValueType LinkedValueType;

    typedef typename Traits::CompareFn  CompareFn;
    typedef LinkedList<Traits>          myself;
    typedef forward_iterator<myself>    iterator;
    
  protected:
    Node    *m_pHead = nullptr, 
            *m_pTail = nullptr;
    size_t   m_size  = 0;
    CompareFn Compfn;
  public: 
    size_t  size()  const       { return m_size;       }
    bool    empty() const       { return size() == 0;  }

  public:
    LinkedList() {}
    //TODO: Tarea 24: CLinkedList: add 2 param to insert
    void    insert(value_type key, LinkedValueType value) { insert_forward(key, value);  }
    value_type &operator[](size_t pos)
    {
      assert(pos <= size());
      Node *pTmp = m_pHead;
      for(auto i= 0 ; i < pos ; i++)
        pTmp = pTmp->getpNext();
      return pTmp->getKeyRef();
    }
    iterator begin() { iterator iter(this, m_pHead);    return iter;    }
    iterator end()   { iterator iter(this, nullptr);    return iter;    }

  protected:
    Node **findPrev(value_type &valor) {   return findPrev(m_pHead, valor);   }
    Node **findPrev(Node *&rpPrev, value_type &valor)
    {   
      if(!rpPrev || Compfn(valor, rpPrev->getValue()) )
        return &rpPrev; // Retorna la direccion del puntero que me apunta
      return findPrev((Node *&)rpPrev->getpNextRef(), valor);
    }
    //TODO: Tarea 24: CLinkedList: add 2 param to insert
    Node *CreateNode(value_type &data, LinkedValueType &valor,  Node *pNext=nullptr){ return new Node(data, valor, pNext); }
    //TODO: Tarea 24: CLinkedList: add 2 param to insert
    Node **insert_forward(value_type &elem, LinkedValueType &valor)
    {
        Node **pParent = findPrev(valor);
        Node *pNew = CreateNode(elem,valor);
        ::CreateBridge(*pParent, pNew, &Node::m_pNext);
        if( !pNew->getpNext() )
          m_pTail = pNew;
        return pParent;
    }

  public:
    value_type PopHead()
    {
        if(m_pHead)
        {
            Node *pNode = m_pHead;
            value_type data = pNode->getKey();
            m_pHead = m_pHead->getpNext();
            delete pNode;
            m_size--;
            if(!m_size) m_pTail = nullptr;
            return data;
        }
        throw "hola excepcion"; // Create custom exception pending
    }

    //TODO: Tarea 23: CLinkedList: print, read
    void print (ostream &os)
    {
      Node *pActual = m_pHead;    
      while (pActual) 
      {
        os << pActual->getKey() << ":[" << pActual->getValue() << "] ";
        pActual = pActual->getpNext();
      }
    }

    LinkedValueType read (value_type key){
      Node *pActual = m_pHead;    
      while (pActual->getKey()!=key) 
      {
        pActual = pActual->getpNext();
      }
      return pActual->getValue();
    }
};

//TODO: Tarea 21: CLinkedList: operator<<
template <typename T>
ostream &operator<<(ostream &os, LinkedList<T> &lista)
{
    lista.print(os);
    return os;
}

//TODO: Tarea 22: CLinkedList: operator>>
template <typename T>
istream &operator>>(istream &is, LinkedList<T> &lista)
{
    lista.insert(lista.value_type, lista.LinkedValueType);
    return is;
}

#endif
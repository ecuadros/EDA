#ifndef __LINKEDLIST_H__  
#define __LINKEDLIST_H__ 
#include <utility>
#include <algorithm>
#include <cassert>
#include "iterator.h"
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
  // TODO change T y KeyNode
    KeyNode<T,V>       m_data;
    Node   *m_pNext;//
  public:
    NodeLinkedList(KeyNode<T,V> data, Node *pNext = nullptr) 
        : m_data(data), m_pNext(pNext)
    {};
    NodeLinkedList(value_type key, LinkedValueType value, Node *pNext = nullptr){
        m_data = KeyNode(key, value);
        m_pNext = pNext;
      }
    // TODO Move to KeyNode
    value_type         getData()                {   return m_data.getData();    }
    value_type        &getDataRef()             {   return m_data.getDataRef();    }

    LinkedValueType         getValue()                {   return m_data.getValue();    }
    LinkedValueType        &getValueRef()             {   return m_data.getValueRef();    }

    void      setpNext(NodeLinkedList *pNext)  {   m_pNext = pNext;  }
    Node     *getpNext()               {   return getpNextRef();   }
    Node    *&getpNextRef()            {   return m_pNext;   }
};

// TODO remove general_iterator
template <typename Container>
class forward_iterator : public general_iterator<Container,  class forward_iterator<Container> > // 
{public: 
    // TODO: subir al padre  
    typedef class general_iterator<Container, forward_iterator<Container> > Parent; 
    typedef typename Container::Node                                  Node; // 
    typedef forward_iterator<Container>                                     myself;

  public:
    forward_iterator(Container *pContainer, Node *pNode) : Parent (pContainer,pNode) {}
    forward_iterator(myself &other)  : Parent (other) {}
    forward_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

public:
    forward_iterator operator++() { Parent::m_pNode = (Node *)Parent::m_pNode->getpNext();  
                                    return *this;
                                  }
};

template <typename _T, typename _V>
struct LLTraitAsc
{
    using  T         = _T;
    using  V         = _V;
    using  Node      = NodeLinkedList<T,V>;
    using  CompareFn = less<T>;
    using  LinkedValueType = _V;
};

template <typename _T, typename _V>
struct LLTraitDesc
{
    using  T         = _T;
    using  V         = _V;
    using  Node      = NodeLinkedList<T,V>;
    using  CompareFn = greater<T>;
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

    using  NodeLL      = NodeLinkedList<value_type, LinkedValueType>;
    
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
    // TODO add LinkedValueType value
    void    insert(value_type key, LinkedValueType value) { insert_forward(key, value);  }
    value_type &operator[](size_t pos)
    {
      assert(pos <= size());
      Node *pTmp = m_pHead;
      for(auto i= 0 ; i < pos ; i++)
        pTmp = pTmp->getpNext();
      return pTmp->getDataRef();
    }
    iterator begin() { iterator iter(this, m_pHead);    return iter;    }
    iterator end()   { iterator iter(this, nullptr);    return iter;    }

    void    push_front(value_type elem)
    {
        Node *pNew = CreateNode(elem);
        pNew->setpNext(m_pHead);
        m_pHead = pNew;
        m_size++;
        if(m_size == 1)
          m_pTail = pNew;
    } 
    void    push_back(value_type elem)
    {   Node *pNew = CreateNode(elem, nullptr);
        if(m_pTail)
          m_pTail->setpNext(pNew);
        m_pTail = pNew;
        if(!m_pHead)
          m_pHead = pNew;
        m_size++;
    }

  protected:
    Node **findPrev(value_type &elem) {   return findPrev(m_pHead, elem);   }
    Node **findPrev(Node *&rpPrev, value_type &elem)
    {   
      if(!rpPrev || Compfn(elem, rpPrev->getData()) )
        return &rpPrev; // Retorna la direccion del puntero que me apunta
      return findPrev((Node *&)rpPrev->getpNextRef(), elem);
    }
    Node *CreateNode(value_type &data, LinkedValueType &value,  Node *pNext=nullptr){ return new Node(data, value, pNext); }
    Node **insert_forward(value_type &elem, LinkedValueType &value)
    {
        Node **pParent = findPrev(elem);
        Node *pNew = CreateNode(elem, value);
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
            value_type data = pNode->getData();
            m_pHead = m_pHead->getpNext();
            delete pNode;
            m_size--;
            if(!m_size) m_pTail = nullptr;
            return data;
        }
        throw "hola excepcion"; // Create custom exception pending
    }
    // TODO add print
    void print        (ostream &os){
      NodeLL *pTmp = m_pHead;    
      // auto iter = begin();
      // for(; iter != end() ; ++iter)
      //   os << "{" << *iter << "," << pTmp->getValue() << "}";
      //   pTmp = pTmp->getpNext();

      while (pTmp) {
        os << "{" << pTmp->getData() << ", " << pTmp->getValue() << "}, ";
        pTmp = pTmp->getpNext();
      }
    }
};

// TODO add operator<<
template <typename T>
ostream &operator<<(ostream &os, LinkedList<T> &obj){
    obj.print(os);
    return os;
}
// TODO add operator>>
template <typename T>
istream & operator>>(istream &is, LinkedList<T> &obj){
    obj.insert(obj.value_type, obj.LinkedValueType);
    return is;
}

#endif
#ifndef __DOUBLE_LINKEDLIST_H__  
#define __DOUBLE_LINKEDLIST_H__ 

#include "linkedlist.h"
#include "iterator.h"

template <typename Container>
class backward_iterator : public general_iterator<Container,  class backward_iterator<Container> > // 
{ public: 
    // typedef typename Container::Node                                         Node;
    // typedef class general_iterator<Container, backward_iterator<Container> > Parent;  // 
    // typedef backward_iterator<Container>                                     myself;

    // DONE: subir al padre, ya no aplican, se usa macro
    _ITER_TYPEDEFS(Container, backward_iterator)

  public:
    backward_iterator(Container *pContainer, Node *pNode) : Parent (pContainer,pNode) {}
    backward_iterator(myself &other)  : Parent (other) {}
    backward_iterator(myself &&other) : Parent(other) {}

  public:
    backward_iterator operator++() { Parent::m_pNode = ((Node *)Parent::m_pNode)->getpPrev();  
                                     return *this;
                                   }
};

// DONE Remove inheritance
template <typename Traits>
class NodeDLL
{
  public:
    using KeyType = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using KeyNode = typename Traits::Node;
  private:
    using T = typename Traits::value_type;
    typedef NodeDLL<Traits> Node;
  public: 
    Node *m_pNext;
    Node *m_pPrev;
    KeyNode m_data;
  public:
    NodeDLL(const T &data, const LinkedValueType &v, Node *pNext = nullptr, Node *pPrev = nullptr) 
        : m_data(data, v), m_pNext(pNext), m_pPrev(pPrev)
    {}
   
    void      setpPrev(Node *pPrev)  {   m_pPrev = pPrev;  }
    Node     *getpPrev()             {   return getpPrevRef();   }
    Node    *&getpPrevRef()          {   return m_pPrev;   }

    void setpNext(Node *pNext) { m_pNext = pNext; }
    Node *getpNext() { return getpNextRef(); }
    Node *&getpNextRef() { return m_pNext; }

    LinkedValueType  getValue() const { return m_data.getValue(); }
    LinkedValueType& getValueRef()    { return m_data.getValueRef(); }

    T getData() { return m_data.getData(); }
    T &getDataRef() { return m_data.getDataRef(); }
};

template <typename _T>
struct DLLAscTraits
{
    typedef   _T          T;
    typedef  NodeDLL<T>  Node;
    typedef  less<T>     CompareFn;
};

template <typename _T>
struct DLLDescTraits
{
    typedef   _T         T;
    typedef  NodeDLL<T>  Node;
    typedef  greater<T>  CompareFn;
};

// DONE remove inheritance
template <typename Traits>
class DoubleLinkedList
{
 public:
    typedef typename Traits::value_type    value_type;
    typedef typename Traits::Node       Node;
    typedef typename Traits::CompareFn  CompareFn;
    using LinkedValueType = typename Traits::LinkedValueType;
    typedef NodeDLL<Traits>    DLLNode;
    typedef LinkedList<Traits>          Parent;
    typedef forward_iterator<DLLNode>    iterator;
    typedef backward_iterator<DLLNode>   riterator;
protected:
    DLLNode *m_pHead = nullptr, *m_pTail = nullptr;
    size_t m_size = 0;
    CompareFn Compfn;
public:
    DoubleLinkedList() {}

    DLLNode **findPrev(const value_type &elem) { return findPrev(m_pHead, elem); }
    DLLNode **findPrev(DLLNode *&rpPrev, const value_type &elem)
    {
        if (!rpPrev || !Compfn(elem, rpPrev->getData()))
            return &rpPrev; // Retorna la direccion del puntero que me apunta
        return findPrev((DLLNode *&)rpPrev->getpNextRef(), elem);
    }

    DLLNode *CreateNode(const value_type &data, const LinkedValueType &v, DLLNode *pNext = nullptr, DLLNode *pPrev = nullptr) { 
      return new DLLNode(data, v, pNext, pPrev); 
    }
    DLLNode **insert_forward(const value_type &elem, const LinkedValueType &v)
    {
        DLLNode **pParent = findPrev(elem);
        DLLNode *pNew = CreateNode(elem, v);
        ::CreateBridge(*pParent, pNew, &DLLNode::m_pNext);
        if (!pNew->getpNext())
            m_pTail = pNew;
        m_size++;
        return pParent;
    }
    void    insert(const value_type &elem, const LinkedValueType &val)
    {   
        DLLNode *pPrevTail = m_pTail;
        DLLNode *pNew = *insert_forward(elem, val);
        if( pNew != m_pTail )
            ::CreateBridge( ((DLLNode *)pNew->getpNext())->getpPrevRef(), pNew, &DLLNode::m_pPrev);
        else
            pNew->setpPrev(pPrevTail);
    }
    riterator rbegin() { riterator iter(this, m_pTail);     return iter;    }
    riterator rend()   { riterator iter(this, nullptr);             return iter;    }
    size_t size() const { return m_size; }
    void  push_front(const value_type &elem, const LinkedValueType &val)
    {
        DLLNode *pNew = CreateNode(elem, val);
        pNew->setpNext(m_pHead);
        m_pHead = pNew;
        m_size++;
        if (m_size == 1)
            m_pTail = pNew;
        if(size() > 1)
            ((DLLNode *)m_pHead->m_pNext)->m_pPrev = m_pHead;
    }
    void push_back(const value_type &elem, const LinkedValueType &val)
    {   
        DLLNode *pPrevTail = m_pTail;
        DLLNode *pNew = CreateNode(elem, val);
        if (m_pTail)
            m_pTail->setpNext(pNew);
        m_pTail = pNew;
        if (!m_pHead)
            m_pHead = pNew;
        m_size++;
        m_pTail->setpPrev(pPrevTail);
    }

    // DONE add print
    void print(ostream &os)
    {
        DLLNode *pTmp = m_pHead;
        if(!pTmp){
            os << "*";
            return;
        }
        os << "*<->";
        while (pTmp)
        {
            os << "(" << pTmp->getData() << " " << pTmp->getValue() << ")<->";
            pTmp = pTmp->getpNext();
        }
        os << "*" << endl;
    }

    template <typename T>
    friend ostream &operator<<(ostream &os, DoubleLinkedList<T> &obj);

    template <typename T>
    friend istream &operator>>(istream &is, DoubleLinkedList<T> &obj);
};


// DONE add operator<<
template <typename T>
ostream &operator<<(ostream &os, DoubleLinkedList<T> &obj)
{
    obj.print(os);
    return os;
}

// DONE: add operator>>
template <typename T>
istream &operator>>(istream &is, DoubleLinkedList<T> &obj)
{
    // DONE: Read from stream
    using KeyType = typename DoubleLinkedList<T>::KeyType;
    using LinkedValueType = typename DoubleLinkedList<T>::LinkedValueType;
    KeyType key;
    LinkedValueType val;
    is >> key >> val;
    obj.insert(key, val);
    return is;
}

#endif
#ifndef __DOUBLE_LINKEDLIST_H__  
#define __DOUBLE_LINKEDLIST_H__ 

#include "linkedlist.h"
#include "iterator.h"

template <typename Container>
class backward_iterator
{ 
private:
    typedef typename Container::Node    Node;
    typedef typename Node::Type         Type;
    typedef typename Node::Type         value_type;
    typedef backward_iterator<Container> myself;
protected:
    Container *m_pContainer;
    Node      *m_pNode;
public: 
    backward_iterator(Container *pContainer, Node *pNode) : m_pContainer(pContainer), m_pNode(pNode) {}
    backward_iterator(myself &other) : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode){}
    backward_iterator(myself &&other) // Move constructor
    {   m_pContainer = move(other.m_pContainer);
        m_pNode      = move(other.m_pNode);
    }

    myself operator=(myself &iter)
    {   m_pContainer = move(iter.m_pContainer);
        m_pNode      = move(iter.m_pNode);
        return *(myself *)this; // Pending static_cast?
    }
    bool operator==(myself iter)   { return m_pNode == iter.m_pNode; }
    bool operator!=(myself iter)   { return !(*this == iter);        }
    Type &operator*()                    { return m_pNode->getDataRef();   }


public:
    backward_iterator operator++() { 
        m_pNode = ((Node *)m_pNode)->getpPrev();  
        return *this;
    }
};

// DONE Remove inheritance
template <typename Traits>
class NodeDLL
{
  public:
    using KeyType = typename Traits::value_type;
    using Type = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using KeyNode = typename Traits::Node;
    typedef NodeDLL<Traits> Node;
  private:
    using T = typename Traits::value_type;
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

    KeyNode &getDataNode() { return m_data; }
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
    typedef typename Traits::value_type       KeyType;
    typedef typename Traits::CompareFn  CompareFn;
    typedef typename Traits::Node    KeyNode;
    using LinkedValueType = typename Traits::LinkedValueType;
    typedef NodeDLL<Traits>    DLLNode;
    typedef NodeDLL<Traits>       Node;
    typedef LinkedList<Traits>          Parent;
    typedef DoubleLinkedList<Traits>    myself;
    typedef forward_iterator<myself>    iterator;
    typedef backward_iterator<myself>   riterator;
protected:
    DLLNode *m_pHead = nullptr, *m_pTail = nullptr;
    size_t m_size = 0;
    CompareFn Compfn;

    void destroy() {
        while(m_pHead) {
            DLLNode *pTmp = m_pHead;
            m_pHead = (DLLNode *)m_pHead->getpNext();
            delete pTmp;
        }
        m_pHead = m_pTail = nullptr;
        m_size = 0;
    }
public:
    DoubleLinkedList() {}

    DLLNode **findPrev(const value_type &elem) { return findPrev(m_pHead, elem); }
    DLLNode **findPrev(DLLNode *&rpPrev, const value_type &elem)
    {
        if (!rpPrev || !Compfn(KeyNode(elem, LinkedValueType()), rpPrev->getDataNode()))
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

    template <typename Callable, typename... Args>
    void foreachReverse (Callable op, Args && ...args)
    {
        ::foreach(rbegin(), rend(), op, args...);
    }

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

    void read(istream &is) {
        destroy();
        while(!is.eof()) {
            KeyType key;
            LinkedValueType val;
            string colon;
            is >> key >> colon >> val;
            insert(key, val);
        }
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
    obj.read(is);
    return is;
}

#endif
#ifndef __LINKEDLIST_H__  
#define __LINKEDLIST_H__ 
#include <utility>
#include <algorithm>
#include <cassert>
#include "iterator.h"
#include "types.h"
#include "xtrait.h"
#include <sstream> 
using namespace std;

template<typename Container>
class iterator_list{
    public:
        typedef typename Container::Node Node;
        typedef typename Node::value_type     value_type;
        typedef iterator_list<Container> myself;
    protected:
        Container *m_pContainer;
        Node      *m_pNode;   
    public:
        iterator_list(Container *pContainer, Node *pNode)
        : m_pContainer(pContainer), m_pNode(pNode){}
        iterator_list(myself &other)
        : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode){}
        iterator_list(myself &&other) {   
            m_pContainer = move(other.m_pContainer);
            m_pNode      = move(other.m_pNode);
          }
        bool operator==(myself iter)   { return m_pNode == iter.m_pNode; }
        bool operator!=(myself iter)   { return !(*this == iter);        }
        value_type &operator*() { return m_pNode->getDataRef();}
        iterator_list & operator++() {
            m_pNode=m_pNode->getpNext();
            return *this;
        }
};

template <typename Node, typename MemberType>
void CreateBridge(Node *&rParent, Node *pNew, MemberType _pMember){
    Node *Node::*pMember = (Node *Node::*)_pMember;
    pNew->*pMember = rParent;
    rParent = pNew; 
}

template <typename Traits>
class NodeLinkedList{
  public:
    using KeyNode = typename Traits::Node;
    using value_type= typename Traits::value_type;
    using LinkedValueType= typename Traits::LinkedValueType;
  private:
    using Node = NodeLinkedList<Traits>;
  public:
  // TODO change T y KeyNode
    KeyNode m_data;
    Node   *m_pNext;//
  public:
    NodeLinkedList(value_type el1,LinkedValueType el2, Node *pNext = nullptr) 
        : m_data(el1,el2), m_pNext(pNext)
    {};
    // TODO Move to KeyNode
    value_type getData(){ return m_data.getData();}
    value_type &getDataRef(){return m_data.getDataRef();}

    void      setpNext(Node *pNext)  {   m_pNext = pNext;  }
    Node     *getpNext()               {   return getpNextRef();   }
    Node    *&getpNextRef()            {   return m_pNext;   }
};
template <typename Traits>
struct LLTraitAsc
{
    using  value_type = typename Traits::value_type;
    using  LinkedValueType= typename Traits::LinkedValueType;
    using  Node      = NodeLinkedList<Traits>;
    using  CompareFn = less<value_type>;
};

template <typename Traits>
struct LLTraitDesc
{
    using  value_type = typename Traits::value_type;
    using  LinkedValueType= typename Traits::LinkedValueType;
    using  Node      = NodeLinkedList<Traits>;
    using  CompareFn = greater<value_type>;
};

//Declaration of Traits in order to use in my LinkedList
using TraitLLIntInt= XTrait<int,int>;
using Traits_LLAsc= LLTraitAsc<TraitLLIntInt>;
using Traits_LLDesc= LLTraitDesc<TraitLLIntInt>;

template <typename Traits>
class LinkedList
{
  public:
    using value_type= typename Traits::value_type;
    using LinkedValueType= typename Traits::LinkedValueType;
    using Node= typename Traits::Node;//At this context Node is NodeLinkList
    using CompareFn = typename Traits::CompareFn;
    using myself = LinkedList<Traits>;
    typedef iterator_list<myself>  iterator;
    
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
    void insert(value_type &val_1,LinkedValueType &val_2){ 
      insert_forward(val_1,val_2);
      m_size++;
    }
    value_type &operator[](size_t pos){
      assert(pos <= size());
      Node *pTmp = m_pHead;
      for(auto i= 0 ; i < pos ; i++)
        pTmp = pTmp->getpNext();
      return pTmp->getDataRef();
    }
    void print (ostream &os) {print(m_pHead,os);}
    iterator begin() { iterator iter(this, m_pHead); return iter;    }
    iterator end()   { iterator iter(this, nullptr);    return iter;    }
    void push_front(value_type elem,LinkedValueType elem2)
    {
        Node *pNew = CreateNode(elem,elem2);
        pNew->setpNext(m_pHead);
        m_pHead = pNew;
        m_size++;
        if(m_size == 1)
          m_pTail = pNew;
    } 
    void push_back(value_type elem,LinkedValueType elem2)
    {   Node *pNew = CreateNode(elem,elem2,nullptr);
        if(m_pTail)
          m_pTail->setpNext(pNew);
        m_pTail = pNew;
        if(!m_pHead)
          m_pHead = pNew;
        m_size++;
    }

  protected:
    void print(Node* pNode, std::ostream& os){
      if(pNode){
        os<<pNode->getDataRef()<<"->";
        print(pNode->getpNext(),os);
      }
    }
    Node **findPrev(value_type &elem) {
      return findPrev(m_pHead, elem);
    }
    Node **findPrev(Node *&rpPrev, value_type &elem){   
      if(!rpPrev || Compfn(elem, rpPrev->getData()) )
        return &rpPrev; // Retorna la direccion del puntero que me apunta
      return findPrev((Node *&)rpPrev->getpNextRef(), elem);
    }
    Node *CreateNode(value_type &val_1, LinkedValueType &val_2 ,Node *pNext=nullptr){
       return new Node(val_1,val_2 ,pNext); 
    }
    Node **insert_forward(value_type &elem,LinkedValueType &elem_2)
    {
        Node **pParent = findPrev(elem);
        Node *pNew = CreateNode(elem,elem_2);
        ::CreateBridge(*pParent, pNew, &Node::m_pNext);
        if(!pNew->getpNext() )
          m_pTail = pNew;
        return pParent;
    }
    Node **insert_forward_2(value_type &elem,LinkedValueType &elem_2)
    {
        Node **pParent = findPrev(elem);
        Node *pNew = CreateNode(elem,elem_2);
        ::CreateBridge(*pParent, pNew, &Node::m_pNext);
        if(!pNew->getpNext() )
          m_pTail = pNew;
        return pParent;
    }
  public:
    value_type PopHead(){
        if(m_pHead){
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
  
};
template <typename T>
ostream &operator<<(ostream &os, LinkedList<T> &obj){
    obj.print(os);
    return os;
}
template <typename T>
T convertFromString(const string& str) {
    T rs;
    stringstream ss(str);
    ss >> rs;
    return rs;
}
template <typename T>
istream & operator>>(istream &is, LinkedList<T> &obj){
    string tmp_flow,num;
    size_t count;
    using vt      = typename T::value_type;
    using lvt     = typename T::LinkedValueType;
    vt  value;
    lvt kvalue;
    while (getline(is,tmp_flow)) { 
        istringstream iss(tmp_flow);
        count = 0;
        while (iss >> num && count < 2) {
            if(count==0){
                vt num_f = convertFromString<vt>(num);
                value=num_f;
            } 
            else{
                lvt num_f = convertFromString<lvt>(num);
                kvalue=num_f; 
            }      
            count++;
        }
        obj.insert(value,kvalue);
    }
    return is;    
}

// TODO
// template <typename T>
// istream & operator>>(istream &is, LinkedList<T> &obj){
//     // TODO
//     return is;
// }

// TODO add operator<<

// TODO add operator>>

#endif
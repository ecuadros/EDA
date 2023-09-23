#ifndef __DOUBLE_LINKEDLIST_H__  
#define __DOUBLE_LINKEDLIST_H__ 
#include <utility>
#include <algorithm>
#include <cassert>
#include "iterator.h"
#include "types.h"
#include "xtrait.h"
#include <sstream> 
//#include "linkedlist.h"
template<typename Container>
class iterator_DLLlist{
    public:
        typedef typename Container::Node Node;
        typedef typename Node::value_type     value_type;
        typedef iterator_DLLlist<Container> myself;
    protected:
        Container *m_pContainer;
        Node      *m_pNode;
        size_t    m_dir;  
    public:
        iterator_DLLlist(Container *pContainer, Node *pNode,size_t dir)
        : m_pContainer(pContainer), m_pNode(pNode),m_dir(dir){}
        iterator_DLLlist(myself &other)
        : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode),m_dir(other.m_dir){}
        iterator_DLLlist(myself &&other) {   
            m_pContainer = move(other.m_pContainer);
            m_pNode      = move(other.m_pNode);
            m_dir        =move(other.m_dir);
          }
        bool operator==(myself iter)   { return m_pNode == iter.m_pNode; }
        bool operator!=(myself iter)   { return !(*this == iter);        }
        value_type &operator*() { return m_pNode->getDataRef();}
        iterator_DLLlist & operator++() {
          if(m_dir==1)
            m_pNode=m_pNode->getpNext();
          else
            m_pNode=m_pNode->getpPrev();
          return *this;
        }
};
// template <typename Container>
// class backward_iterator : public general_iterator<Container,  class backward_iterator<Container> > // 
// { public: 
//     // TODO: subir al padre  
//     typedef typename Container::Node                                         Node;
//     typedef class general_iterator<Container, backward_iterator<Container> > Parent;  // 
//     typedef backward_iterator<Container>                                     myself;

//   public:
//     backward_iterator(Container *pContainer, Node *pNode) : Parent (pContainer,pNode) {}
//     backward_iterator(myself &other)  : Parent (other) {}
//     backward_iterator(myself &&other) : Parent(other) {}

//   public:
//     backward_iterator operator++() { Parent::m_pNode = ((Node *)Parent::m_pNode)->getpPrev();  
//                                      return *this;
//                                    }
// };

// 
template <typename Traits>
class NodeDLL{
  public:
    using KeyNode = typename Traits::Node;
    using value_type= typename Traits::value_type;
    using LinkedValueType= typename Traits::LinkedValueType;
  private:
    using Node = NodeDLL<Traits>;
  public: 
    KeyNode m_data;
    Node   *m_pPrev;//
    Node   *m_pNext;//
  public:
    NodeDLL(value_type el1,LinkedValueType el2, Node *pNext = nullptr,Node *pPrev = nullptr) 
        : m_data(el1,el2), m_pNext(pNext), m_pPrev(pPrev)
    {};
    value_type getData(){ return m_data.getData();}
    value_type &getDataRef(){return m_data.getDataRef();}
    void      setpPrev(Node *pPrev)  {   m_pPrev = pPrev;  }
    Node     *getpPrev()             {   return getpPrevRef();   }
    Node    *&getpPrevRef()          {   return m_pPrev;   }
    void      setpNext(Node *pNext)  {   m_pNext = pNext;  }
    Node     *getpNext()               {   return getpNextRef();   }
    Node    *&getpNextRef()            {   return m_pNext;   }
};

template <typename Traits>
struct DLLAscTraits
{
    using  value_type = typename Traits::value_type;
    using  LinkedValueType= typename Traits::LinkedValueType;
    using  Node      = NodeDLL<Traits>;
    using  CompareFn = less<value_type>;
};

template <typename Traits>
struct DLLDescTraits
{
     using  value_type = typename Traits::value_type;
    using  LinkedValueType= typename Traits::LinkedValueType;
    using  Node      = NodeDLL<Traits>;
    using  CompareFn = greater<value_type>;
};
//Declaration of Traits in order to use in my LinkedList
using TraitDLLIntInt= XTrait<int,int>;
using Traits_DLLAsc= DLLAscTraits<TraitDLLIntInt>;
using Traits_DLLDesc= DLLDescTraits<TraitDLLIntInt>;

template <typename Traits>
class DoubleLinkedList{
 public:
    using value_type= typename Traits::value_type;
    using LinkedValueType= typename Traits::LinkedValueType;
    using Node= typename Traits::Node;//At this context Node is NodeLinkList
    using CompareFn = typename Traits::CompareFn;
    using myself = DoubleLinkedList<Traits>;
    typedef iterator_DLLlist<myself>  iterator;
  protected:
    Node    *m_pHead = nullptr, 
            *m_pTail = nullptr;
    size_t   m_size  = 0;
    CompareFn Compfn;
  public: 
    size_t  size()  const       { return m_size;       }
    bool    empty() const       { return size() == 0;  }  
  public:
    DoubleLinkedList() {}
    void print (ostream &os) {print(m_pHead,os);}
    void r_print (ostream &os) {r_print(m_pTail,os);}
    void insert(value_type &val_1,LinkedValueType &val_2){ 
      insert_forward(val_1,val_2);  
    }
    Node **insert_forward(value_type &elem,LinkedValueType &elem_2){
      Node **pParent = findPrev(elem);
      Node *pNew = CreateNode(elem,elem_2);
      m_size++; 
      pNew->m_pNext = *pParent;
      if(pNew->getpNext()){
        if(((Node*)*pParent)->getpPrev()){
            Node *tmp=((Node*)*pParent)->getpPrev();
            Node *tmp2=(Node*)*pParent;
            pNew->setpPrev(tmp);
            tmp->setpNext(pNew);
            tmp2->setpPrev(pNew);
            *pParent = pNew;
        }
        else{
            Node *tmp2=(Node*)*pParent;
            tmp2->setpPrev(pNew);
            *pParent = pNew;
        }
      }
      else{
        *pParent = pNew;
        if(m_size>1){
          ((Node*)*pParent)->setpPrev(m_pTail);
        }
      }
      if(!pNew->getpNext())
        m_pTail = pNew;
      // cout<<"Tail:"<<m_size << "--"<<m_pTail->getData()<<endl;
      // cout<<"Heap:"<<m_size << "--"<<m_pHead->getData()<<endl;
      // Node *pTmp= m_pHead;
      // while(pTmp){
      //    cout<<"Node:"<<pTmp->getData()<<" Prev Node: "<<(pTmp->getpPrev() ? (pTmp->getpPrev())->getData():-99)<< " Next Node: "<<(pTmp->getpNext() ? (pTmp->getpNext())->getData():-99)<<endl;
      //    pTmp=pTmp->getpNext();
      // }
     
      return pParent;
    }
    Node **findPrev(value_type &elem) {
      return findPrev(m_pHead, elem);
    }
    Node **findPrev(Node *&rpPrev, value_type &elem){   
      if(!rpPrev || Compfn(elem, rpPrev->getData()) )
        return &rpPrev; // Retorna la direccion del puntero que me apunta
      return findPrev((Node *&)rpPrev->getpNextRef(), elem);
    }
    iterator begin() { iterator iter(this,m_pHead,1);     return iter;    }
    iterator end()   { iterator iter(this,nullptr,1);     return iter;    }
    iterator rbegin() { iterator iter(this,m_pTail,-1);     return iter;    }
    iterator rend()   { iterator iter(this,nullptr,-1);     return iter;    }
    
    value_type &operator[](size_t pos){
      assert(pos <= size());
      Node *pTmp = m_pHead;
      for(auto i= 0 ; i < pos ; i++)
        pTmp = pTmp->getpNext();
      return pTmp->getDataRef();
    }
    void Write(){
      print(m_pHead,cout);
    }
    void r_Write(){
      r_print(m_pTail,cout);
    }

    void Read(const std::string& filename){
      std::ifstream is(filename); 
      if (!is.is_open()) { 
          std::cerr << "Error opening file: " << filename << std::endl;
          return;
      }
      string tmp_flow,num;
      size_t count;
      value_type  value;
      LinkedValueType kvalue;
      while (getline(is,tmp_flow)) { 
          std::istringstream iss(tmp_flow);
          count = 0;
          while (iss >> num && count < 2) {
              if(count==0)
                  value = convertFromString<value_type>(num);
              else
                  kvalue = convertFromString<LinkedValueType>(num);  
              count++;
          }
          insert(value,kvalue);
      } 

      is.close();
    }
    void push_front(value_type elem,LinkedValueType elem2){
        Node *pNew = CreateNode(elem,elem2);
        pNew->setpNext(m_pHead);
        m_pHead->setpPrev(pNew);
        m_pHead = pNew;
        m_size++;
        if(m_size == 1)
          m_pTail = pNew;
    }
    void push_back(value_type elem,LinkedValueType elem2){
      Node *pNew = CreateNode(elem,elem2,nullptr,nullptr);
        if(m_pTail){
           m_pTail->setpNext(pNew);
           pNew->setpPrev(m_pTail);
        }
        m_pTail = pNew;
        if(!m_pHead)
          m_pHead = pNew;
        m_size++;
    }
    Node *CreateNode(value_type &val_1, LinkedValueType &val_2 ,Node *pNext=nullptr,Node *pPrev=nullptr){
       return new Node(val_1,val_2 ,pNext,pPrev); 
    }
    protected:
    void print(Node* pNode, std::ostream& os){
      if(pNode){
        os<<pNode->getDataRef()<<"->";
        print(pNode->getpNext(),os);
      }
    }
    void r_print(Node* pNode, std::ostream& os){
      if(pNode){
        os<<pNode->getDataRef()<<"->";
        r_print(pNode->getpPrev(),os);
      }
    }
    
};
template <typename T>
ostream &operator<<(ostream &os, DoubleLinkedList<T> &obj){
    obj.print(os);
    return os;
}
template <typename T>
istream & operator>>(istream &is, DoubleLinkedList<T> &obj){
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


#endif
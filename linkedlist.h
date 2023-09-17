#ifndef __LINKEDLIST_H__  
#define __LINKEDLIST_H__ 
#include <utility>
#include <algorithm>
#include <cassert>
#include "iterator.h"
#include "types.h"
#include "IteratorList.h"
#include "xtrait.h"
#include <iostream>
#include <ostream>
#include<fstream>
#include "foreach.h"
#include <sstream>
using namespace std;

// Iterador en otro .h

template <typename Node, typename MemberType>
void CreateBridge(Node *&rParent, Node *pNew, MemberType _pMember){
    Node *Node::*pMember = (Node *Node::*)_pMember;
    pNew->*pMember = rParent;
    rParent = pNew; 
}
//========== Nodo de Lista simple (KeyNode adicionado) ===========
template <typename xTraits>

class NodeLinkedList{

  public:
    using value_type		    = typename xTraits::value_type;
    using LinkedValueType	    = typename xTraits::LinkedValueType;
    using Data	 		        = typename xTraits::Node;
  
  private:
    using Node = NodeLinkedList<xTraits>;

  public:
 
    Data	   m_data;
    Node   	*m_pNext;

  public:

    NodeLinkedList(value_type key,LinkedValueType value, Node *pNext = nullptr) : m_data(key,value), m_pNext(pNext){};
    
    value_type       getData() 	    { return m_data.getData();}
    value_type       &getDataRef()	{return m_data.getDataRef();}
    LinkedValueType  getValue()     { return m_data.getValue(); }

    void      setpNext(Node *pNext)  	 {   m_pNext = pNext;  }
    Node     *getpNext()               {   return getpNextRef();   }
    Node    *&getpNextRef()            {   return m_pNext;   }
};

//========= Traits para LinkedList ===================
template <typename xTraits>

struct LLTraitAsc
{
    using  value_type 		  	= typename xTraits::value_type;
    using  LinkedValueType	    = typename xTraits::LinkedValueType;
    using  Node      		    = NodeLinkedList<xTraits>;
    using  CompareFn 		    = less<value_type>;
};

template <typename xTraits>

struct LLTraitDesc
{
     using  value_type 		  	= typename xTraits::value_type;
    using  LinkedValueType		= typename xTraits::LinkedValueType;
    using  Node      		    = NodeLinkedList<xTraits>;
    using  CompareFn 		    = greater<value_type>;
};

using LLAsc  = LLTraitAsc<XTrait<IX, IX>>;
using LLDesc = LLTraitDesc<XTrait<IX,IX>>;

//========= Conteiner LinkedList ===================
template <typename LinkedListTrait>

class LinkedList
{
    public:

      using value_type          = typename LinkedListTrait::value_type;
      using LinkedValueType     = typename LinkedListTrait::LinkedValueType;
      using Node                = typename LinkedListTrait::Node;
      using CompareFn           = typename LinkedListTrait::CompareFn;
      using myself              = LinkedList<LinkedListTrait>;
      using iteratorList        = LinkedList_iterator<myself>;

    protected:
      Node    *m_pHead = nullptr, *m_pTail = nullptr;
      size_t   m_size  = 0;
      CompareFn Compfn;

    public: 
      size_t  size()  const       { return m_size;       }
      bool    empty() const       { return size() == 0;  }

    public:
      LinkedList() {}

      void insert(const value_type &key,const LinkedValueType &value){ 
          insert_forward(key,value);
          m_size++;
      }

      value_type &operator[](size_t pos){
          assert(pos <= size());
          Node *pTmp = m_pHead;
          for(auto i= 0 ; i < pos ; i++)
            pTmp = pTmp->getpNext();
          return pTmp->getDataRef();
      }

      void push_front(const value_type elem,const LinkedValueType elem2){
          Node *pNew = CreateNode(elem,elem2);
          pNew->setpNext(m_pHead);
          m_pHead = pNew;
          m_size++;
          if(m_size == 1)
            m_pTail = pNew;
      } 

      void push_back(const value_type elem, const LinkedValueType elem2){   
          Node *pNew = CreateNode(elem,elem2,nullptr);
          if(m_pTail)
            m_pTail->setpNext(pNew);
          m_pTail = pNew;
          if(!m_pHead)
            m_pHead = pNew;
          m_size++;
      }

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
          throw "hola excepcion"; 
      }

    protected:
    
      Node **findPrev(const value_type &key) { return findPrev(m_pHead, key); }

      Node **findPrev(Node *&rpPrev, const value_type &key){   

        if(!rpPrev || Compfn(key, rpPrev->getData()) )
          return &rpPrev; // Retorna la direccion del puntero que me apunta
        return findPrev((Node *&)rpPrev->getpNextRef(), key);
      }

      Node *CreateNode(const value_type &key, const LinkedValueType &value ,Node *pNext=nullptr){ return new Node(key,value ,pNext); }

      Node **insert_forward(const value_type &key,const LinkedValueType &value){

        Node **pParent = findPrev(key);
        Node *pNew = CreateNode(key,value);
        ::CreateBridge(*pParent, pNew, &Node::m_pNext);
        if(!pNew->getpNext() )
          m_pTail = pNew;
        return pParent;
      }

    public:
    
	// ============ iteradores de la lista simle ===================
		
		iteratorList begin() { iteratorList iter(this, m_pHead); 	return iter;    }
	    iteratorList end()   { iteratorList iter(this, nullptr);    return iter;    }
    
    //============= se adicionó la función print ==================
    
	    void print (ostream &os){
	    	
	          Node *pNode = m_pHead;
	            for(size_t i = 0; i < m_size ; ++i )
	                {os << "<"<< pNode->getData() << ":" << pNode->getValue() << ">"<<"  ";
	                pNode=pNode->getpNext();}
	            cout<<endl;
	    }
	    
	//============= se adicionó la función read para leer un txt ==================
	
		void read (istream &is){
	 		assert(is);
			value_type key;
			LinkedValueType value;
			string line;
			
			while(getline(is, line)){
				istringstream iss(line);
				char colon;
				iss >> key;
				iss >> colon;
				iss >> value;
				insert(key,value);

			}     
	    }
 
}; // Fin de la clase

//=============  operador << ========================
template <typename T>
ostream &operator<<(ostream &os, LinkedList<T> &obj){
    obj.print(os);
    return os;
}

//============  operador >> ========================

template <typename T>
istream & operator>>(istream &is, LinkedList<T> &obj){
	    obj.read(is);
	    return is;
	}




#endif
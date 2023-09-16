#ifndef __DOUBLE_LINKEDLIST_H__  
#define __DOUBLE_LINKEDLIST_H__ 

#include <utility>
#include <algorithm>
#include <cassert>
#include "types.h"
#include "IteratorDoubleList.h"
#include "IteratorList.h"
#include "xtrait.h"
#include <iostream>
#include <ostream>
#include<fstream>
#include "foreach.h"
#include <sstream>
using namespace std;


template <typename Node, typename MemberType>

void CreateBridge(Node *&rParent, Node *pNew, MemberType _pMember){
    Node *Node::*pMember = (Node *Node::*)_pMember;
    pNew->*pMember = rParent;
    rParent = pNew; 
}

//===================  Nodo de Lista Doble (sin herencia)  ===================
template <typename xTraits>

class NodeDoubleLL {
	
public:

	using value_type		    = typename xTraits::value_type;
    using LinkedValueType	    = typename xTraits::LinkedValueType;
    using Data	 		        = typename xTraits::Node;
    
private:
	
   using Node = NodeDoubleLL<xTraits>;
  
  
  public: 
  
  	Data	   	 m_data;
    Node   		*m_pNext;
    Node   		*m_pPrev;
    
public:
    NodeDoubleLL(value_type key,LinkedValueType value, Node *pNext = nullptr, Node *pPrev = nullptr) 
        		: m_data(key,value),m_pNext(pNext), m_pPrev(pPrev){}
    
    
    value_type       getData() 	    { return m_data.getData();}
    value_type       &getDataRef()	{return m_data.getDataRef();}
    LinkedValueType  getValue()     { return m_data.getValue(); }

   
    void      setpPrev(Node *pPrev)  	{   m_pPrev = pPrev;  }
    Node      *getpPrev()             	{   return getpPrevRef();}
    Node      *&getpPrevRef()          	{   return m_pPrev;   }
    
    void      setpNext(Node *pNext)  	 {   m_pNext = pNext;  }
    Node     *getpNext()               	 {   return getpNextRef(); }
    Node     *&getpNextRef()            	 {   return m_pNext;   }
};

//===================  Traits para Lista Doble ===================

template <typename xTraits>

struct DLLDescTraits
{
    using  value_type 		  	= typename xTraits::value_type;
    using  LinkedValueType	    = typename xTraits::LinkedValueType;
    using  Node      		    = NodeDoubleLL<xTraits>;
    using  CompareFn 		    = less<value_type>;
};

template <typename xTraits>
struct DLLAscTraits
{
    using  value_type 		  	= typename xTraits::value_type;
    using  LinkedValueType	    = typename xTraits::LinkedValueType;
    using  Node      		    = NodeDoubleLL<xTraits>;
    using  CompareFn 		    = greater<value_type>;
};

//===================  Container Lista Doble (sin herencia) ===================

using DLAsc  	= DLLAscTraits<XTrait<IX, IX>>;
using DLDesc	= DLLDescTraits<XTrait<IX,IX>>;

template <typename DoubleLLTrait>

class DoubleLinkedList {
	
 public:
 	
 	using value_type          = typename DoubleLLTrait::value_type;
    using LinkedValueType     = typename DoubleLLTrait::LinkedValueType;
    using Node                = typename DoubleLLTrait::Node;
    using CompareFn           = typename DoubleLLTrait::CompareFn;
    using myself              = DoubleLinkedList<DoubleLLTrait>;
    using iterator        	  = LinkedList_iterator<myself>;
    using riterator           = Double_backward_LinkedList_iterator<myself>;
    
protected:
	
    Node    	*m_pHead = nullptr, *m_pTail = nullptr;
    size_t   	m_size  = 0;
    CompareFn 	Compfn;

public: 

    size_t  size()  const       { return m_size;       }
    bool    empty() const       { return size() == 0;  } 
	DoubleLinkedList() {}   
     
public:
	
    void insert(value_type &key,LinkedValueType &value){
    	
    	Node *pPrevTail = m_pTail;
        Node *pNew =*insert_forward(key,value);
        
        if( pNew != m_pTail )
            ::CreateBridge( ((Node *)pNew->getpNext())->getpPrevRef(), pNew, &Node::m_pPrev);
        else
            pNew->setpPrev(pPrevTail);
            
        m_size++;	
	}
	
	 value_type &operator[](size_t pos){
	 	
          assert(pos <= size());
          Node *pTmp = m_pHead;
          for(auto i= 0 ; i < pos ; i++)
            pTmp = pTmp->getpNext();
          return pTmp->getDataRef();
      }
    
protected:
	
	Node **findPrev(value_type &key) { return findPrev(m_pHead, key); }

    Node **findPrev(Node *&rpPrev, value_type &key){   

        if(!rpPrev || Compfn(key, rpPrev->getData()) )
          return &rpPrev; 
        return findPrev((Node *&)rpPrev->getpNextRef(), key);
      }

    Node *CreateNode(value_type &key, LinkedValueType &value ,Node *pNext=nullptr,Node *pPrev=nullptr){ return new Node(key,value,pNext,pPrev); }
    
	Node **insert_forward(value_type &key,LinkedValueType &value){ 
	
	  	  Node **pParent = findPrev(key);
          Node *pNew = CreateNode(key,value);
          ::CreateBridge(*pParent, pNew, &Node::m_pNext);
          if( !pNew->getpNext() )
            m_pTail = pNew;
          return pParent;
          
        }
     //============= Función print ==================
    public:
	    void print (ostream &os){
	    	 
	    	  cout<< "°°°°°°°°Forward printing°°°°°°°°"<<endl;
	    
	          Node *pNode = m_pHead;
	          for(size_t i = 0; i < m_size ; ++i )
	                {os <<" -> "<<"<"<< pNode->getData() << ":" << pNode->getValue() << ">";
	                pNode=pNode->getpNext();}
	         cout << endl;
	          cout<< "°°°°°°°°Backward printing°°°°°°°°"<<endl;
	         
	          pNode = m_pTail;
	          for(size_t i = 0; i < m_size ; ++i )
	                {os <<" -> "<< "<"<< pNode->getData() << ":" << pNode->getValue() << ">";
	                pNode=pNode->getpPrev();}
	          cout<<endl;
	    }
	    
	    	//============= Función Read ==================
	
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
	   
    //============== Iteradores ===================================
    
	    iterator begin() 		{ iterator iter(this, m_pHead);    return iter;    }
	    iterator end()   		{ iterator iter(this, nullptr);    return iter;    }
	    riterator rbegin() 		{ riterator iter(this, m_pTail);   return iter;    }
	    riterator rend()   		{ riterator iter(this, nullptr);   return iter;    }
        
    
};

//=============  operador << ========================
template <typename T>
ostream &operator<<(ostream &os, DoubleLinkedList<T> &obj){
    obj.print(os);
    return os;
}

//============  operador >> ========================

template <typename T>
istream & operator>>(istream &is, DoubleLinkedList<T> &obj){
	    obj.read(is);
	    return is;
	}


#endif
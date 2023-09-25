//Class Queue with templates for diferentes type of elements
#ifndef __QUEUE_H__  
#define __QUEUE_H__
#include<iostream>
#include "types.h"
#include "xtrait.h"
namespace QueueNamespace {
using namespace std;
// template <typename Traits> 
// struct NodeQueueT{
// 	Traits dato;
// 	NodeQueueT<Traits> *next;
// };


//Node Class
template <typename Traits>
class NodeQ{
  public:
    using KeyNode = typename Traits::Node;
    using value_type= typename Traits::value_type;
    using LinkedValueType= typename Traits::LinkedValueType;
  private:
    using Node = NodeQ<Traits>;
  public:
    KeyNode m_data;
    Node   *m_pNext;//
  public:
    NodeQ(value_type el1,LinkedValueType el2, Node *pNext = nullptr) 
        : m_data(el1,el2), m_pNext(pNext)
    {};
    value_type getData(){ return m_data.getData();}
    value_type &getDataRef(){return m_data.getDataRef();}
    LinkedValueType getValue(){ return m_data.getValue();}
    LinkedValueType &getValueRef(){return m_data.getValueRef();}

    void      setpNext(Node *pNext)  {   m_pNext = pNext;  }
    Node     *getpNext()               {   return getpNextRef();   }
    Node    *&getpNextRef()            {   return m_pNext;   }
};
//Traits
template <typename Traits>
struct Data_Key{
    using  value_type = typename Traits::value_type;
    using  LinkedValueType= typename Traits::LinkedValueType;
    using  Node      = NodeQ<Traits>;
    using  CompareFn = greater<value_type>;
};

//Declaration of Traits in order to use in my LinkedList
using TraitIntInt= XTrait<int,int>;
using Traits_Data_Key= Data_Key<TraitIntInt>;
//Queue Class
template <typename Traits>
class QueueT{
	public:
		using value_type= typename Traits::value_type;
    using LinkedValueType= typename Traits::LinkedValueType;
    using Node= typename Traits::Node;//At this context Node is NodeQ
	public:
		//typedef NodeQueueT<Traits> *pNode;
		QueueT();
		~QueueT();
		void inQueue(const value_type & val_1, const LinkedValueType & val_2);
		Node* CreateNode(const value_type &val_1, const LinkedValueType &val_2 ,Node *pNext=nullptr){
       return new Node(val_1,val_2 ,pNext);}
		value_type dQueue(){
			Node *p=pTop;
			if(pTop != nullptr){
				auto x= pTop->getData();
				p=pTop->getpNext();
				delete pTop;
				pTop=p;
				return x;
			}
			else{
				pTop= nullptr;
				pDown= nullptr;
				return -1;
			}
		}
		void dnQueue();
		void Print();
		void PrintS();
		bool IsEmpty();
		bool ElementExist(value_type x);
		bool TElement_Exist(Node *x);
		int QCount();
	private:
		Node *pTop;
		Node *pDown;		
};
template <typename Traits>
QueueT<Traits>::QueueT(){
	pTop=nullptr;
	pDown=nullptr;
}
template <typename Traits>
QueueT<Traits>::~QueueT(){
	Node *p= pTop;
	while(p!= nullptr){
		pTop=p->getpNext();
		delete p;
		p= pTop;
	}
	//cout<<"\nFreed Memory"<<endl;
}
template <typename Traits>
bool QueueT<Traits>::IsEmpty() {
	if (pTop== nullptr)
		return true;
	else
		return false;		
}
template <typename Traits>
int QueueT<Traits>::QCount(){
	Node *p=pTop;
	int n=0;
	while(p!=nullptr){
		n+=1;
		p=p->getpNext();
	}
	return n;
}

template <typename Traits>
void QueueT<Traits>::inQueue(const value_type & val_1, const LinkedValueType& val_2){
	//p = new NodeQueueT<Traits>;
	Node *pNew = CreateNode(val_1,val_2);
	if(pTop==nullptr){
		pTop=pNew;
	}
	else
		pDown->setpNext(pNew);
	pDown=pNew;	
}
template <typename Traits>
bool QueueT<Traits>::ElementExist(value_type x){
	Node *p=pTop;
	while(p!= nullptr){
		if (p->getData()==x){
			return true;
		}
		p=p->getpNext();
	}
	return false;
	
}
template <typename Traits>
void QueueT<Traits>::Print(){
	Node *p=pTop;
	while(p!= nullptr){
		cout<<p->getData()<<"->";
		p=p->getpNext();
	}
}
template <typename Traits>
void QueueT<Traits>::PrintS(){
	Node *p= pTop;
	while(p!= nullptr){
		cout<<"["<<p->getData()<<","<<p->getValue()<<"]"<<"->";
		p=p->getpNext();
	}
}
template <typename Traits>
bool QueueT<Traits>::TElement_Exist(Node *x){
	if (x == nullptr) {
        return false;
    }
	Node *p=pTop;
	while(p!= nullptr){
		if (p->getData()==x->getData() && p->getValue()==x->getValue()){
			return true;
		}
		p=p->getpNext();
	}
	return false;
}

template <typename Traits>
void QueueT<Traits>::dnQueue(){
	Node *p=pTop;
	if(pTop != nullptr){
		p=pTop->getpNext();
		delete pTop;
		pTop=p;
	}
	else{
		pTop= nullptr;
		pDown= nullptr;
	}
}
}
#endif

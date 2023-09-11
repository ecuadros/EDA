#ifndef __POSORDER_H__
#define __POSORDER_H__

#include <cassert>
#include <cstddef>
#include<iostream>
#include <string>
#include "iteratorTree.h"
using namespace std;

template <typename Container>

class posorder_iterator : public general_iterator_Tree< Container,  class posorder_iterator<Container> >
	{
			_DEFTREE(Container, posorder_iterator);
		public:
			posorder_iterator(Container *pContainer, Node *pNode) : Parent (pContainer,pNode) {}
			posorder_iterator(myself &other)  : Parent (other) {}
			posorder_iterator(myself &&other) : Parent(other) {} 

			Node &operator*(){return *Parent::m_pNode;}
			
			
			posorder_iterator operator++(){
					Node *root = Parent::m_pContainer->getRoot(); // raiz 
					Node *temp = Parent::m_pNode;  // nodo actual
					Node * current=PrintPostOrder(root,temp);
					Parent::m_pNode=current;
					return * this;	
			        			
			   }
			        			
			        			
			Node* PrintPostOrder(Node* root , Node* temp){
			    	
				if(temp != nullptr && !temp->IsVisited()){
					temp->NodeVisited(true);
					temp=root;
					return aux(root,temp);
							
					}
					
				if(temp != nullptr && temp->IsVisited()){
					temp=root;
					return aux(root ,  temp);
				}
				
					if(temp == nullptr && !temp->IsVisited()){
					temp->NodeVisited(true);
					temp=root;
					return aux(root ,  temp);
				}	
				
			  }
				   													
			Node* aux(Node* root , Node* temp){
					   	
				while (temp != nullptr && !temp->IsVisited()) {
					if (temp->getChild(1) != nullptr && !temp->getChild(1)->IsVisited()) {
						temp = temp->getChild(1);
						} else if (temp->getChild(0) != nullptr && !temp->getChild(0)->IsVisited()) {  // tiene hijo derecho distinto de null
						                temp = temp->getChild(0);
						} else {
						            temp->NodeVisited(true);
						            return temp;
						            }
			        			}
				   	
				}  
				
			
			//10 4 23 12 37 40 6 28


	};

#endif

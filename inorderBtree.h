#ifndef __INORDER_H__
#define __INORDER_H__

#include <cassert>
#include <cstddef>
#include<iostream>
#include <string>
#include "iteratorTree.h"
using namespace std;

template <typename Container>

class inorder_iterator : public general_iterator_Tree< Container,  class inorder_iterator<Container> >
	{
			_DEFTREE(Container, inorder_iterator);
		public:
			inorder_iterator(Container *pContainer, Node *pNode) : Parent (pContainer,pNode) {}
			inorder_iterator(myself &other)  : Parent (other) {}
			inorder_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

			Node &operator*()
			{
				return *Parent::m_pNode;
			}
			
			
				inorder_iterator operator++(){
					Node *current = Parent::m_pNode;
					Node* newNode=printInorder(current);
					Parent::m_pNode=newNode;
					return *this;
					
				}
			
				Node * printInorder(Node *current){
					
					
					if(current->getChild(0)!=nullptr){
						current=current->getChild(0);
						while(current->getChild(1)!=nullptr){
							current=current->getChild(1);
						}
						return current;
					}
					if(current->getChild(0)==nullptr && current->getParent()->getChild(1)==current){
						return current=current->getParent();
					}
					
					if(current->getChild(0)==nullptr && current->getParent()->getChild(0)==current){
							while(current->getParent()!=nullptr &&  current->getParent()->getChild(0)==current){
							current=current->getParent();
						}
						return current=current->getParent();
					}
			
				}
			

			//10 4 23 12 37 40 6 28


	};

#endif

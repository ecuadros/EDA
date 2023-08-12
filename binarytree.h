#ifndef __BINARY_TREE_H__  
#define __BINARY_TREE_H__ 
//#include <utility>
#include <iostream>
#include <algorithm>
#include <cassert>
#include "types.h"
#include "keynode.h"
#include "xtrait.h"
#include "btrait.h"
#include "NodeBinaryTree.h"
//#include "foreach.h"
//#include "iterator.h"
//#include "util.h"
using namespace std;

#define _DEF(_Container, _iter)  \
public: \
    typedef class general_iterator<_Container, _iter<Container> > Parent;     \
    typedef typename _Container::Node                             Node;       \
    typedef _iter<_Container>                                     myself;

template <typename Container>
class binary_tree_iterator : public general_iterator<Container,  class binary_tree_iterator<Container> > // 
{  _DEF(Container, binary_tree_iterator); // TODO: llevar esta misma idea a todos container ya existentes

  public:
    binary_tree_iterator(Container *pContainer, Node *pNode) : Parent (pContainer,pNode) {}
    binary_tree_iterator(myself &other)  : Parent (other) {}
    binary_tree_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

public:
    binary_tree_iterator operator++() { //Parent::m_pNode = (Node *)Parent::m_pNode->getpNext();  
                                        return *this;
                                  }
};



//°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°°
using dataT=XTrait<int,int>;
using BDescTree=BDescTrait<dataT>;
template <typename Traits>
class BinaryTree
{
  public:
    using value_type = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using NodeTree = typename Traits::NodeTree;
    using CompareFn = typename Traits::CompareFn;
    using myself    = BinaryTree<Traits>;
    typedef binary_tree_iterator<myself>    iterator;

protected:
    NodeTree    *m_pRoot = nullptr;
    size_t   m_size  = 0;
    CompareFn Compfn;
public: 
    size_t  size()  const       { return m_size;       }
    bool    empty() const       { return size() == 0;  }
    // TODO: insert must receive two paramaters: elem and LinkedValueType value
    
    
    void  insert(const value_type &element, const  LinkedValueType &value ) { 
        internal_insert(element,value, nullptr, m_pRoot);  }
   

protected:
    NodeTree *CreateNode(NodeTree *pParent, const value_type &element, const LinkedValueType &value){
         return new NodeTree(pParent,element,value); }

    NodeTree *internal_insert( const value_type &element, const LinkedValueType &value , NodeTree *pParent, NodeTree *&rpOrigin)
 //  referencia a un puntero
    {
        if( !rpOrigin ) //  llegué al fondo de una rama
        {   ++m_size;
            return (rpOrigin = CreateNode(pParent, element,value));
        }
        size_t branch = Compfn(element, rpOrigin->getDataRef() );
        return internal_insert(element,value, rpOrigin, rpOrigin->getChildRef(branch));
    }

  
public:

    iterator preorderBegin(NodeTree* root){
        if(root != nullptr){
            NodeTree* nodoCurrent = root;
        }


    }

   // void preorder (ostream &os)    {   preorder (m_pRoot, os, 0);  }
    //void inorder  (ostream &os)    {   inorder  (m_pRoot, os, 0);  }
    //void postorder(ostream &os)    {   postorder(m_pRoot, os, 0);  }
    //  void inorder(void (*visit) (value_type& item)){   inorder(m_pRoot, visit);    }
    void print    (ostream &os)    {   print    (m_pRoot, os, 0);  }
  
/*
protected:

     // TODO: generalize this function by using iterators and apply any function
    // Create a new iterator to walk in postorder
    void preorder(Node  *pNode, ostream &os, size_t level){
        //foreach(preorderbegin(), preorderend(), fn)
        if( pNode ){   
            os << " --> " << pNode->getDataRef();
            preorder(pNode->getChild(0), os, level+1);
            preorder(pNode->getChild(1), os, level+1);            
        }
    }
      */
    // TODO: generalize this function by using iterators and apply any function
    void print(NodeTree  *pNode, ostream &os, size_t level)
    {
        // foreach(begin(), end(), print);
        if( pNode ){
            NodeTree *pParent = pNode->getParent();
            print(pNode->getChild(1), os, level+1);
            //os << string(" | ") * level << pNode->getDataRef() << "(" << (pParent?(pNode->getBranch()?"R-":"L-") + to_string(pParent->getData()):"Root") << ")" <<endl;
            os << string(" | ") * level << pNode->getDataRef() << "(" << (pParent?to_string(pParent->getData()):"Root") << ")" <<endl;
            print(pNode->getChild(0), os, level+1);
        }
    }
  
  /*
    void inorder(Node  *pNode, ostream &os, size_t level)
    {
        // foreach(*this, fn);
        // foreach(begin(), end(), fn);
        if( pNode )
        {   Node *pParent = pNode->getParent();
            inorder(pNode->getChild(0), os, level+1);
            os << " --> " << pNode->getDataRef();
            inorder(pNode->getChild(1), os, level+1);
        }
    }

    // TODO: generalize this function by using iterators and apply any function
    // Create a new iterator to walk in postorder
    void postorder(Node  *pNode, ostream &os, size_t level){
        //foreach(postorderbegin(), postorderend(), fn)
        if( pNode ){   
            postorder(pNode->getChild(0), os, level+1);
            postorder(pNode->getChild(1), os, level+1);
            os << " --> " << pNode->getDataRef();
        }
    }


    // TODO: generalize this function by using iterators and apply any function
    void inorder(Node  *pNode, void (*visit) (value_type& item))
    {
        if( pNode )
        {   
            inorder(pNode->getChild(0), *visit);
            (*visit)(pNode->getDataRef());
            inorder(pNode->getChild(1), *visit);
        }
    }
    */
};

#endif
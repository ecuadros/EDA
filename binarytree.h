#ifndef __BINARY_TREE_H__  
#define __BINARY_TREE_H__ 
#include <cstddef>
#include <functional>
#include <ios>
#include <utility>
#include <algorithm>
#include <cassert>
#include "types.h"
#include "keynode.h"
#include <vector>
#include"inorderBtree.h"
#include"postorderBtree.h"
#include "preorderBtree.h"
#include<iostream>
#include "foreach.h"



//=================  NODO DE UN ARBOL==================================
//======================================================================
template <typename T, typename V>
// Adding KeyNode
class NodeBinaryTree: public KeyNode<T,V>{

public:
    // TODO: Change T by KeyNode
    using value_type =T;
    using LinkedValueType=V;
    using  Type = T;
    using Data = KeyNode<T,V>;

private:
  using Node=NodeBinaryTree<T,V>;
 
public:
    Node *  m_pParent = nullptr;
    vector< Node *> m_pChild = {nullptr, nullptr}; // 2 hijos inicializados en nullptr
    // Extras
    size_t      m_level=0;
    bool        m_visited=false; 
    
public:

    NodeBinaryTree(Node *pParent,value_type Key,LinkedValueType Val,size_t level,bool Visit, Node *p0 = nullptr, Node *p1 = nullptr):
     m_pParent(pParent), Data(Key,Val){ m_level = level;m_visited=Visit, m_pChild[0] = p0;   m_pChild[1] = p1;   }

    value_type getData() { return Data::getData();}
    value_type &getDataRef() {return Data::getDataRef();}

 // TODO: review if these functions must remain public/private
 public:
    void      setpChild(const Node *pChild, size_t pos)  {
                     m_pChild[pos] = pChild;  }
    Node    * getChild(size_t branch){ 
                    return m_pChild[branch];  }
    Node   *&getChildRef(size_t branch){
                   return m_pChild[branch];  }
    Node    * getParent() { 
                  return m_pParent;   }
    //Added some functions
    size_t  level() { return m_level;}
    bool IsVisited()  {return m_visited;}
    bool NodeVisited(bool visited)  {return m_visited=visited;}

};



//============ TRAITS  ===============================
template <typename _K, typename _V, typename _CompareFn = std::less< _K >>

struct BinaryTreeTrait
{
    using  value_type      = _K;
    using  LinkedValueType = _V;
    using  Node      = NodeBinaryTree<_K, _V>;
    using  CompareFn = _CompareFn;

};

//======================================================================
//  CLASE BINARY TREE
//======================================================================

using BTreeIntInt=BinaryTreeTrait<IX,IX>;

template <typename BinaryTreeTrait>
class BinaryTree
{   public:
        using value_type        =typename BinaryTreeTrait::value_type;
        using LinkedValueType   = typename BinaryTreeTrait::LinkedValueType;
        using Node          	=typename BinaryTreeTrait::Node;
        using CompareFn         =typename BinaryTreeTrait::CompareFn;
        using myself            =BinaryTree<BinaryTreeTrait>;
        using inorderIter       =inorder_iterator<myself>;
        using posorderIter      =posorder_iterator<myself>;
        using preorderIter		=preorder_iterator<myself>;
        //using printIter         =print_iterator<myself>;

protected:
    Node        *m_pRoot = nullptr;
    size_t      m_size  = 0;
    CompareFn   Compfn;

public: 

    size_t  size()  const       { return m_size;       }
    bool    empty() const       { return size() == 0;  }
    Node *getRoot() { return m_pRoot;}


    // TODO: insert must receive two paramaters: elem and LinkedValueType value
    void    insert(value_type &elem, LinkedValueType &value) { internal_insert1(elem, value, nullptr, m_pRoot,0,false);  }

protected:

    Node *CreateNode(Node *pParent, value_type &elem,LinkedValueType &value,size_t level,bool visit){ return new Node(pParent, elem,value,level,visit); }

    Node *internal_insert1(value_type &elem,LinkedValueType &value, Node *pParent, Node *&rpOrigin,size_t level,bool visit)
    {
        if( !rpOrigin ) //  nodo actual es nulo?
        {   ++m_size;
            return (rpOrigin = CreateNode(pParent, elem,value,level,visit));
        }
        size_t branch = Compfn(elem, rpOrigin->getDataRef() );
        return internal_insert1(elem, value, rpOrigin, rpOrigin->getChildRef(branch),level+1,visit);
    }
public:
//0 : derecha  1: Izquierda
 Node* left(Node *pNode){
    while(pNode->getChild(1)!=nullptr){
        pNode=pNode->getChild(1);
    }
    return pNode;
 }
 Node* rigth(Node *pNode){

    while(pNode->getChild(0)!=nullptr){
        pNode=pNode->getChild(0);
    }
    return pNode;
 }
   Node* beginPostorder(Node*pNode ){
    	if( left(pNode)->getChild(0)!=nullptr){
    		return left(pNode)->getChild(0);
		}
		else{
			return left(pNode);
		}
	}

 	/*				
		void * printPostorder(){
					Node *current=m_pRoot;				
					Node *temp = current;
				
					while(temp!=nullptr && temp->IsVisited()==false){
						if(temp->getChild(1)!=nullptr && temp->getChild(1)->IsVisited()==false ){
							 temp = temp->getChild(1);
						}
						else if(temp->getChild(0)!=nullptr && temp->getChild(0)->IsVisited()==false ){
							 temp=temp->getChild(0);
						}
						else{
							cout<<temp->getData()<<" ";
							temp->NodeVisited(true);
							temp=current;
							//return temp;
						}
					}
					
					
				}
			
*/
public:
    // Impresion de forma estándar de un arbol
    // void print    (ostream &os)    {   print    (m_pRoot, os, 0);  }  // para imprimir el arbol
    //------------------------------------------------------------------------------------
  
    //printIter begin_print() { printIter iter(this, rigth(m_pRoot));    return iter;    }
    //printIter end_print()   { printIter iter(this, left(m_pRoot));    return iter;    }
    //--------------------------------------------------------------------------------------


    inorderIter  begin_inorder() { inorderIter  iter(this, left(m_pRoot));    return iter;    }
    inorderIter  end_inorder()   { inorderIter  iter(this, nullptr);    return iter;    }
    //---------------------------------------------------------------------------------------------

    posorderIter  begin_Postorder() { posorderIter  iter(this,beginPostorder(m_pRoot));    return iter;    }
    posorderIter  end_Postorder()   { posorderIter  iter(this, nullptr);    return iter;    }
    
    //----------------------------------------------------------------------------------------------
    
    preorderIter  begin_Preorder() { preorderIter  iter(this,m_pRoot);    return iter;    }
    preorderIter  end_Preorder()   { preorderIter  iter(this, nullptr);    return iter;    }



public:

// TODO: generalize this function by using iterators and apply any function
       
        
     //template<F>
     // void printTree(Node  *pNode, F ShowTree ){foreach( begin_print(), end_print(), ShowTree);}  
	  
	// ===== left-root-right ==========      
    void inorder( void (*func) (Node& node)) { foreach(begin_inorder(), end_inorder(),  func);}
    
    // ===== left-right-root ==========   
	void postorder( void (*func) (Node& node)) { foreach(begin_Postorder(), end_Postorder(),  func);}	
	
	//  ====== root-left-right  =================
	void preorder(void (*func) (Node& node)){foreach( begin_Preorder(), end_Preorder(),  func); }
  

/*
  std::string printNTimes(const std::string &c, size_t n){ 
    std::string result = " ";
    for (size_t i = 0; i < n; ++i) {
        result += c;
    }
    return result;
    
    }
void print(Node* pNode, std::ostream& os, size_t level) {
    if (pNode) {
        Node* pParent = pNode->getParent(); 
        std::string c = printNTimes("--|--", level);
        print(pNode->getChild(1), os, level + 1);
        os <<c << pNode->getDataRef() << " (p: " << (pParent?to_string(pParent->getData()):"Root") << ") " <<endl;
        print(pNode->getChild(0), os, level + 1);
    }
}


*/

};  ///  END OF THE CLASS

#endif
#ifndef __BINARY_TREE_H__  
#define __BINARY_TREE_H__ 
//#include <utility>
//#include <algorithm>
#include <iostream>
#include <string>
#include <cassert>
#include "types.h"
#include "keynode.h"
//#include "util.h"
#include "xtrait.h"
#include <vector>
using namespace std;

template <typename Traits>
class NodeBinaryTree
{
    public:
        //Do Change T by KeyNode
        //typedef T         Type;
        using KeyNode         = typename Traits::Node;
        using value_type      = typename Traits::value_type;
        using LinkedValueType = typename Traits::LinkedValueType;
    private:
        //typedef NodeBinaryTree<KeyNode> Node;
        using Node = NodeBinaryTree<Traits>;
    public:
        KeyNode  m_data;
        Node *m_pParent = nullptr;
        bool m_status=false;
        vector<Node *> m_pChild = {nullptr, nullptr}; // 2 hijos inicializados en nullptr
    public:
        NodeBinaryTree(Node *pParent, value_type elem,LinkedValueType elem2, Node *p0 = nullptr, Node *p1 = nullptr) 
        : m_pParent(pParent),m_data(elem, elem2),m_status(false)
         {   
            m_pChild[0] = p0;   
            m_pChild[1] = p1;
         }
        // DO: Keynode 
        value_type getData()  { return m_data.getData();    }
        value_type &getDataRef()  { return m_data.getDataRef();    }
 
        // TODO: review if these functions must remain public/private
        void      setpChild(const Node *pChild, size_t pos)  {   m_pChild[pos] = pChild;}
        void      setp_status(bool esp){m_status=esp;}
        bool      get_status(){return m_status;}
        Node    * getChild(size_t branch){ return m_pChild[branch];  }
        Node    *&getChildRef(size_t branch){ return m_pChild[branch];  }
        Node    * getParent() { return m_pParent;   }
};

#define _DEF(_Container, _iter)  \
public: \
    typedef class general_iterator<_Container, _iter<Container> > Parent;     \
    typedef typename _Container::Node                             Node;       \
    typedef _iter<_Container>                                     myself;

template <typename Container>
class binary_tree_iterator : public general_iterator<Container,  class binary_tree_iterator<Container>> // 
{  _DEF(Container, binary_tree_iterator); // TODO: llevar esta misma idea a todos container ya existentes

  private:
    Node *m_pRoot=nullptr;
  public:
    binary_tree_iterator(Container *pContainer, Node *pNode,Node *pRoot) : Parent (pContainer,pNode),m_pRoot(pRoot) {}
    binary_tree_iterator(myself &other)  : Parent (other),m_pRoot(other.m_pRoot) {}
    binary_tree_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

  private:
    Node *in_order(Node *pNode){
        if(pNode){
            pNode->setp_status(true);
            if(pNode->getChild(0) && pNode->getChild(0)->get_status()==false){
                while(pNode->getChild(0)&&pNode->getChild(0)->get_status()==false)
                    pNode= pNode->getChild(0);   
                pNode->setp_status(true);         
                return pNode;
            }
            else{
                if(pNode->getChild(1)&& pNode->getChild(1)->get_status()==false){
                    pNode=pNode->getChild(1);
                    if(!pNode->getChild(0)||pNode->getChild(0)->get_status()==true){
                        pNode->setp_status(true);
                        return pNode;
                    }
                    pNode= in_order(pNode);
                    return pNode;
                }
                else{
                    Node *pParent = pNode->getParent();
                    Node *Child_0= pParent->getChild(0);
                    if(Child_0==pNode){
                        pNode=pParent;
                        pNode->setp_status(true);
                        return pNode;
                    }
                    else{
                        while(pNode==pNode->getParent()->getChild(1)){
                            pNode=pNode->getParent();
                        } 
                        pNode=pNode->getParent();
                        pNode->setp_status(true);
                        return pNode;
                    }
                }      
            }
        }
        else{
            std::cout<<"Ingreso null"<<endl;
            return nullptr;
        }
    }    
    
    void fill_status_i(Node *pNode){
        if(pNode) {
            Node *pParent = pNode->getParent();
            fill_status_i(pNode->getChild(0));
            pNode->setp_status(false);
            fill_status_i(pNode->getChild(1));
        }
    }
    public:
    void fill_status(){
        Node *pNode=m_pRoot; 
        fill_status_i(pNode);
    }
    binary_tree_iterator operator++() {
        Node *NextNode= in_order(Parent::m_pNode);
        if(NextNode){
            Parent::m_pNode= NextNode;
        }
        return *this;
    }

    
};


template <typename Traits>
struct BinaryTreeAscTraits
{
    using  value_type              = typename Traits::value_type;
    using  LinkedValueType         = typename Traits::LinkedValueType;
    using  Node                    = NodeBinaryTree<Traits>;
    using  CompareFn = greater<value_type>;
};

template <typename Traits>
struct BinaryTreeDescTraits
{
    using  value_type              = typename Traits::value_type;
    using  LinkedValueType         = typename Traits::LinkedValueType;
    using  Node                    = NodeBinaryTree<Traits>;
    using  CompareFn = less<value_type>;
};
using TraitBTreeIntInt   = XTrait<int,int>;
using Traits_BNTAsc= BinaryTreeAscTraits<TraitBTreeIntInt> ;
using Traits_BNTDesc= BinaryTreeDescTraits<TraitBTreeIntInt> ;
template <typename Traits>
class BinaryTree
{
  public:
    using value_type = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using Node= typename Traits::Node;// In this context Node= NodeBinaryTree<Traits>
    using CompareFn = typename Traits::CompareFn;
    using myself = BinaryTree<Traits>;
    //typedef typename Traits::T         value_type;
    //typedef typename Traits::Node       Node;
    //typedef typename Traits::CompareFn      CompareFn;
    //typedef BinaryTree<Traits>              myself;
    typedef binary_tree_iterator<myself>    iterator;

  protected:
    Node    *m_pRoot = nullptr;
    size_t   m_size  = 0;
    CompareFn Compfn;
  public: 
    size_t  size()  const       { return m_size;       }
    bool    empty() const       { return size() == 0;  }
    // DO: insert must receive two paramaters: elem and LinkedValueType value
    void insert(value_type elem, LinkedValueType elem2) { 
        internal_insert1(elem,elem2,nullptr, m_pRoot); 
    }

  protected:
    Node *CreateNode(Node *pParent, value_type &elem,LinkedValueType &elem2){
         return new Node(pParent,elem,elem2); }
    Node *internal_insert1(value_type &elem, LinkedValueType &elem2, Node *pParent, Node *&rpOrigin){
        if(!rpOrigin ) //  llegué al fondo de una rama
        {   ++m_size;
            return (rpOrigin = CreateNode(pParent,elem,elem2));
        }
        size_t branch = Compfn(elem, rpOrigin->getDataRef() );
        return internal_insert1(elem,elem2,rpOrigin,rpOrigin->getChildRef(branch));
    }
public:
    void inorder  (ostream &os)    {   inorder  (m_pRoot, os, 0);  }
    void postorder(ostream &os)    {   postorder(m_pRoot, os, 0);  }
    void preorder (ostream &os)    {   preorder (m_pRoot, os, 0);  }
    void print    (ostream &os)    {   print    (m_pRoot, os, 0);  }
    void inorder(void (*visit) (value_type& item))
    {   inorder(m_pRoot, visit);    }

protected:
    void inorder(Node *pNode, ostream &os, size_t level)
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
    // Create a new iterator to walk in postorder
    void preorder(Node  *pNode, ostream &os, size_t level){
        //foreach(preorderbegin(), preorderend(), fn)
        if( pNode ){   
            os << " --> " << pNode->getDataRef();
            preorder(pNode->getChild(0), os, level+1);
            preorder(pNode->getChild(1), os, level+1);            
        }
    }
    
    // TODO: generalize this function by using iterators and apply any function
    // void print(Node  *pNode, ostream &os, size_t level)
    // {
    //     // foreach(begin(), end(), print);
    //     if( pNode ){
    //         Node *pParent = pNode->getParent();
    //         print(pNode->getChild(1), os, level+1);
    //         //os << string(" | ") * level << pNode->getDataRef() << "(" << (pParent?(pNode->getBranch()?"R-":"L-") + to_string(pParent->getData()):"Root") << ")" <<endl;
    //         //os << string(" | ") * level << pNode->getDataRef() << "(" << (pParent?to_string(pParent->getData()):"Root") << ")" <<endl;
    //         print(pNode->getChild(0), os, level+1);
    //     }
    // }
    void print(Node* pNode, std::ostream& os, size_t level){
        if (pNode) {
            Node* pParent = pNode->getParent();
            for (size_t i = 0; i < level; ++i) {
                os << " | ";
            }
            os << pNode->getDataRef() << "(" << (pParent ? std::to_string(pParent->getData()) : "Root") << ")" << std::endl;
            print(pNode->getChild(1), os, level + 1);
            print(pNode->getChild(0), os, level + 1);
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
    public:
   
    Node* find_last_node(Node* pNode,size_t dir) {
        while (pNode->getChild(dir)) {
            pNode = pNode->getChild(dir);
        }
        return pNode;
    }

    iterator begin() { 
        Node* lnode_0 = find_last_node(m_pRoot,0);
        Node* lnode_1 = find_last_node(m_pRoot,1);
        if(m_pRoot==lnode_1){
            iterator iter(this,lnode_0,m_pRoot); 
            return iter;
        }
        else{
            if(m_pRoot==lnode_0){
                iterator iter(this,m_pRoot,m_pRoot); 
                return iter;
            }
            else{
                iterator iter(this,lnode_0,m_pRoot);   
                return iter; 
            }
        }    
    }
    iterator end() {
        Node* lnode_0 = find_last_node(m_pRoot,0);
        Node* lnode_1 = find_last_node(m_pRoot,1);
        if(m_pRoot==lnode_1){
            iterator iter(this,lnode_1,m_pRoot); 
            return iter;
        }else{
            if(m_pRoot==lnode_0){
                iterator iter(this,lnode_1,m_pRoot); 
                return iter;
            }
            else{
                iterator iter(this, lnode_1,m_pRoot);   
                return iter; 
            }
        }       
     }
};

#endif
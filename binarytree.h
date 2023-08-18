#ifndef __BINARY_TREE_H__  
#define __BINARY_TREE_H__ 
#include <iostream>
#include <string>
#include <cassert>
#include "types.h"
#include "keynode.h"
#include "xtrait.h"
#include <vector>
#include "iterator_btree.h"
//#include "avl.h"
using namespace std;

template <typename Traits>
class NodeBinaryTree
{
    public:
        using KeyNode         = typename Traits::Node;
        using value_type      = typename Traits::value_type;
        using LinkedValueType = typename Traits::LinkedValueType;
    protected:
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
    typedef bt_iter_inorder<myself>    in_iterator;
    typedef bt_iter_postorder<myself>  post_iterator;
    typedef bt_iter_preorder<myself> pre_iterator;

  protected:
    Node    *m_pRoot = nullptr;
    size_t   m_size  = 0;
    CompareFn Compfn;
  public:
    void    set_m_pRoot(Node *Root){m_pRoot=Root;}
    size_t  size()  const       { return m_size;       }
    bool    empty() const       { return size() == 0;  }
    // DO: insert must receive two paramaters: elem and LinkedValueType value
    virtual void insert(value_type elem, LinkedValueType elem2) { 
        internal_insert1(elem,elem2,nullptr, m_pRoot); 
    }

  protected:
    Node *CreateNode(Node *pParent, value_type &elem,LinkedValueType &elem2){
         return new Node(pParent,elem,elem2); }
    virtual Node *internal_insert1(value_type &elem, LinkedValueType &elem2, Node *pParent, Node *&rpOrigin){
        if(!rpOrigin ) //  llegué al fondo de una rama
        {   ++m_size;
            return (rpOrigin = CreateNode(pParent,elem,elem2));
        }
        //size_t branch = Compfn(rpOrigin->getDataRef() ,elem);
        size_t branch = Compfn(elem,rpOrigin->getDataRef());
        return internal_insert1(elem,elem2,rpOrigin,rpOrigin->getChildRef(branch));
    }
public:
    void inorder  (ostream &os)    {   inorder  (m_pRoot, os, 0);  }
    void postorder(ostream &os)    {   postorder(m_pRoot, os, 0);  }
    void preorder (ostream &os)    {   preorder (m_pRoot, os, 0);  }
    virtual void print    (ostream &os)    {   print    (m_pRoot, os, 0);  }
    void inorder(void (*visit) (value_type& item))
    {   inorder(m_pRoot, visit);    }

public:
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

    virtual void print(Node* pNode, std::ostream& os, size_t level){
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
     Node* find_last_node_pre(Node* pNode,size_t dir) {
        if(dir==0){
            while (pNode->getChild(0)) {
                pNode = pNode->getChild(0);
            }
            return pNode; 
        }
        else{
            Node *tmp=pNode;
            while(pNode->getChild(1)) {
                pNode = pNode->getChild(1);
            }
            if(pNode!=tmp && pNode->getChild(0)==nullptr){
                return pNode;
            }
            else{
                if(pNode->getChild(0)){
                    pNode=pNode->getChild(0);
                    pNode=find_last_node_pre(pNode,dir);
                    return pNode;
                }
                else{
                    return pNode;
                }
        
            }
        }
    }
    Node* find_last_node_pos(Node* pNode,size_t dir) {
        if(dir==1){
            while (pNode->getChild(dir)) {
                pNode = pNode->getChild(dir);
            }
            return pNode;
        }
        else{
             while (pNode->getChild(dir)) {
                pNode = pNode->getChild(dir);
            }
            if(pNode->getChild(1)==nullptr){
                return pNode;
            }
            else{
                pNode=pNode->getChild(1);
                pNode= find_last_node_pos(pNode,dir);
                return pNode;
            }
        }
    }
    // Iterator inorder
    in_iterator begin_in() { 
        Node* lnode_0 = find_last_node(m_pRoot,0);
        Node* lnode_1 = find_last_node(m_pRoot,1);
        if(m_pRoot==lnode_1){
            in_iterator iter(this,lnode_0,m_pRoot); 
            return iter;
        }
        else{
            if(m_pRoot==lnode_0){
                in_iterator iter(this,m_pRoot,m_pRoot); 
                return iter;
            }
            else{
                in_iterator iter(this,lnode_0,m_pRoot);   
                return iter; 
            }
        }    
    }
    in_iterator end_in() {
        Node* lnode_0 = find_last_node(m_pRoot,0);
        Node* lnode_1 = find_last_node(m_pRoot,1);
        if(m_pRoot==lnode_1){
            in_iterator iter(this,lnode_1,m_pRoot); 
            return iter;
        }else{
            if(m_pRoot==lnode_0){
                in_iterator iter(this,lnode_1,m_pRoot); 
                return iter;
            }
            else{
                in_iterator iter(this, lnode_1,m_pRoot);   
                return iter; 
            }
        }       
     }
     // Iterator postorder
    post_iterator begin_post() { 
        Node* lnode_0 = find_last_node_pos(m_pRoot,0);
        Node* lnode_1 = find_last_node_pos(m_pRoot,1);
        if(m_pRoot==lnode_1){
            post_iterator iter(this,lnode_0,m_pRoot); 
            return iter;
        }
        else{
            if(m_pRoot==lnode_0){
                post_iterator iter(this,lnode_1,m_pRoot); 
                return iter;
            }
            else{
                post_iterator iter(this,lnode_0,m_pRoot);   
                return iter; 
            }
        }    
    }
    post_iterator end_post() {
        Node* lnode_0 = find_last_node_pos(m_pRoot,0);
        Node* lnode_1 = find_last_node_pos(m_pRoot,1);
        if(m_pRoot==lnode_1){
            post_iterator iter(this,lnode_1,m_pRoot); 
            return iter;
        }else{
            if(m_pRoot==lnode_0){
                post_iterator iter(this,m_pRoot,m_pRoot); 
                return iter;
            }
            else{
                post_iterator iter(this, m_pRoot,m_pRoot);   
                return iter; 
            }
        }       
     }
     // Iterator preorder
    pre_iterator begin_pre() { 
        pre_iterator iter(this,m_pRoot,m_pRoot);
        return iter; 

    }
    pre_iterator end_pre() {
        Node* lnode_0 = find_last_node_pre(m_pRoot,0);
        Node* lnode_1 = find_last_node_pre(m_pRoot,1);
        pre_iterator iter(this,lnode_1,m_pRoot); 
        return iter;
     }
};

#endif
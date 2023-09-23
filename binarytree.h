#ifndef __BINARY_TREE_H__  
#define __BINARY_TREE_H__ 
#include <iostream>
#include <string>
#include <cassert>
#include "types.h"
#include "keynode.h"
#include "xtrait.h"
#include <vector>
namespace BinaryTree_Namespace {
using namespace std;
#define _DEF(_Container,_iter)  \
public: \
    typedef class general_iterator<_Container, _iter<Container> > Parent;     \
    typedef typename _Container::Node                             Node;       \
    typedef _iter<_Container>                                     myself;

// Iterator in order
template <typename Container>
class bt_iter_inorder : public general_iterator<Container,  class bt_iter_inorder<Container>> // 
{  _DEF(Container, bt_iter_inorder); // TODO: llevar esta misma idea a todos container ya existentes

  private:
    Node *m_pRoot=nullptr;
    size_t count=0;
    size_t m_count;
  public:
    bt_iter_inorder(Container *pContainer, Node *pNode,Node *pRoot,size_t N_count) : Parent (pContainer,pNode),m_pRoot(pRoot),m_count(N_count) {}
    bt_iter_inorder(myself &other)  : Parent (other),m_pRoot(other.m_pRoot),m_count(other.m_count) {}
    bt_iter_inorder(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

  private:
    Node *in_order(Node *pNode){
        if(pNode){
            if(count==1)
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
        count=0;
    }
    bt_iter_inorder operator++() {
        count=count+1;
        Node *NextNode=nullptr;
        if(count<m_count)
            NextNode= in_order(Parent::m_pNode);
        if(NextNode && count<m_count)
            Parent::m_pNode= NextNode;
        else{
            Parent::m_pNode= nullptr;
            fill_status();
        }
        return *this;  
    }

    
};
// Iterator in pos order
template <typename Container>
class bt_iter_postorder : public general_iterator<Container,  class bt_iter_postorder<Container>> // 
{  _DEF(Container, bt_iter_postorder); // TODO: llevar esta misma idea a todos container ya existentes

  private:
    Node *m_pRoot=nullptr;
    Node *m_pNodei=nullptr;
    size_t count=0;
    size_t m_count=0;
  public:
    bt_iter_postorder(Container *pContainer, Node *pNode,Node *pRoot,size_t N_count) : Parent (pContainer,pNode),m_pRoot(pRoot),m_count(N_count),m_pNodei(pNode) {}
    bt_iter_postorder(myself &other)  : Parent (other),m_pRoot(other.m_pRoot),m_count(other.m_count),m_pNodei(other.m_pNodei) {}
    bt_iter_postorder(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

  private:
    Node *post_order(Node *pNode){
        if(pNode){
            if(count==1)
                pNode->setp_status(true); 
            //Direction 0;    
            if(pNode->getChild(0) && pNode->getChild(0)->get_status()==false){
                while(pNode->getChild(0)&&pNode->getChild(0)->get_status()==false)
                    pNode= pNode->getChild(0); 
                if(!pNode->getChild(1)){
                    pNode->setp_status(true);         
                    return pNode;
                }
                else{
                    pNode=pNode->getChild(1);
                    pNode= post_order(pNode);
                    return pNode;
                }    
            }
            else{
                //Direction 1;
                if(pNode->getChild(1)&& pNode->getChild(1)->get_status()==false){
                    pNode=pNode->getChild(1);
                    pNode= post_order(pNode);
                    return pNode;
                }
                else{
                    //Does not have childs
                    Node *pParent = pNode->getParent();
                    Node *Child_0= pParent->getChild(0);
                    if(Child_0==pNode){
                        pNode=pParent;
                        if(pNode->getChild(1)){
                            if(pNode->getChild(1)->getChild(0)==nullptr && pNode->getChild(1)->getChild(1)==nullptr){
                                pNode=pNode->getChild(1);
                                pNode->setp_status(true);
                                return pNode;
                            }
                            else{
                                pNode=pNode->getChild(1);
                                pNode= post_order(pNode);
                                return pNode;
                            }
                        }
                        else{
                            pNode->setp_status(true);
                            return pNode;
                        }  
                    }
                    else{
                        //Go to dir 1
                        //cout<<"\n"<<pNode<<"-"<<m_pNodei<<endl;
                        if(count==1){
                            if(pNode==m_pNodei)
                                pNode=pParent;
                            //cout<<"\nLeave0: "<<pNode->getData()<<endl;
                            pNode->setp_status(true);
                            return pNode;
                        } 
                        else{
                            if(!pNode->get_status()){
                                //cout<<"\nLeave1: "<<pNode->getData()<<endl;
                                pNode->setp_status(true);
                                return pNode; 
                            }
                            else{
                              pNode=pParent;
                              //cout<<"\nLeave2 : "<<pNode->getData()<<endl;
                              pNode->setp_status(true);
                              return pNode;  
                            }
                        }  
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
        count=0;
    }
    bt_iter_postorder operator++() {
        count=count+1;
        Node *NextNode=nullptr;
        if(m_count>count)
            NextNode= post_order(Parent::m_pNode);
        if(NextNode && m_count>count)
            Parent::m_pNode= NextNode;
        else{
            Parent::m_pNode= nullptr;
            fill_status();
        }
        return *this;
    }
};
// Iterator in pre order
template <typename Container>
class bt_iter_preorder : public general_iterator<Container,  class bt_iter_preorder<Container>> // 
{  _DEF(Container, bt_iter_preorder); // TODO: llevar esta misma idea a todos container ya existentes

  private:
    Node *m_pRoot=nullptr;
    size_t count=0;
    size_t m_count;
  public:
    bt_iter_preorder(Container *pContainer, Node *pNode,Node *pRoot,size_t N_count) : Parent (pContainer,pNode),m_pRoot(pRoot),m_count(N_count) {}
    bt_iter_preorder(myself &other)  : Parent (other),m_pRoot(other.m_pRoot),m_count(other.m_count) {}
    bt_iter_preorder(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

  private:
    Node *pre_order(Node *pNode){
        if(pNode){
            if(count==1)
               pNode->setp_status(true);
            if(pNode->getChild(0) && pNode->getChild(0)->get_status()==false){
                pNode=pNode->getChild(0);
                pNode->setp_status(true);
                return pNode;
               }
            else{
                if(pNode->getChild(1) && pNode->getChild(1)->get_status()==false){
                    pNode=pNode->getChild(1);
                    pNode->setp_status(true);
                    return pNode;
                }
                else{
                    Node *pParent=pNode->getParent();
                    if(pParent->getChild(0)==pNode){
                        if(pNode->getParent()->getChild(1)){
                            pNode=pNode->getParent()->getChild(1);
                            pNode->setp_status(true);
                            return pNode;
                        }
                        else{
                            pNode=pre_order(pParent);
                            return pNode;
                        } 
                    }
                    else{
                        while(pNode->getParent()->getChild(1)==pNode){
                            pNode=pNode->getParent();
                        }
                        pNode=pNode->getParent();
                        pNode= pre_order(pNode);
                        return pNode;
                    }

                }  
            }   

        }
        else{
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
        count=0;
    }
    bt_iter_preorder operator++() {
        count=count+1;
        Node *NextNode=nullptr;
        //cout<<"\nData: "<<m_count<<"-"<<count<<endl;
        if(m_count>count)
            NextNode= pre_order(Parent::m_pNode);
        //cout<<"\n"<<NextNode->getData()<<endl;    
        if(NextNode && m_count>count)
            Parent::m_pNode= NextNode;
        else{
            Parent::m_pNode= nullptr;
            fill_status();
        }
        return *this;
    }

    
};

template <typename Traits>
class NodeBinaryTree
{
    public:
        using KeyNode         = typename Traits::Node;
        using value_type      = typename Traits::value_type;
        using LinkedValueType = typename Traits::LinkedValueType;
    private:
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
    size_t  size()  const       { return m_size;       }
    bool    empty() const       { return size() == 0;  }
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
         //size_t branch = Compfn(rpOrigin->getDataRef() ,elem);
         size_t branch =Compfn(elem,rpOrigin->getDataRef());
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
        if( pNode )
        {   Node *pParent = pNode->getParent();
            inorder(pNode->getChild(0), os, level+1);
            os << " --> " << pNode->getDataRef();
            inorder(pNode->getChild(1), os, level+1);
        }
    }

    void postorder(Node  *pNode, ostream &os, size_t level){
        //foreach(postorderbegin(), postorderend(), fn)
        if( pNode ){   
            postorder(pNode->getChild(0), os, level+1);
            postorder(pNode->getChild(1), os, level+1);
            os << " --> " << pNode->getDataRef();
        }
    }

    void preorder(Node  *pNode, ostream &os, size_t level){
        //foreach(preorderbegin(), preorderend(), fn)
        if( pNode ){   
            os << " --> " << pNode->getDataRef();
            preorder(pNode->getChild(0), os, level+1);
            preorder(pNode->getChild(1), os, level+1);            
        }
    }
    
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
            if(!pNode->getChild(1)){
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
            in_iterator iter(this,lnode_0,m_pRoot,m_size); 
            return iter;
        }
        else{
            if(m_pRoot==lnode_0){
                in_iterator iter(this,m_pRoot,m_pRoot,m_size); 
                return iter;
            }
            else{
                in_iterator iter(this,lnode_0,m_pRoot,m_size);   
                return iter; 
            }
        }    
    }
    in_iterator end_in() {
        Node* lnode_0 = find_last_node(m_pRoot,0);
        Node* lnode_1 = find_last_node(m_pRoot,1);
        if(m_pRoot==lnode_1){
            in_iterator iter(this,nullptr,m_pRoot,m_size); 
            return iter;
        }else{
            if(m_pRoot==lnode_0){
                in_iterator iter(this,nullptr,m_pRoot,m_size); 
                return iter;
            }
            else{
                in_iterator iter(this, nullptr,m_pRoot,m_size);   
                return iter; 
            }
        }       
     }
     // Iterator postorder
    post_iterator begin_post() { 
        Node* lnode_0 = find_last_node_pos(m_pRoot,0);
        Node* lnode_1 = find_last_node_pos(m_pRoot,1);
        if(m_pRoot==lnode_1){
            post_iterator iter(this,lnode_0,m_pRoot,m_size); 
            return iter;
        }
        else{
            if(m_pRoot==lnode_0){
                post_iterator iter(this,lnode_1,m_pRoot,m_size); 
                return iter;
            }
            else{
                post_iterator iter(this,lnode_0,m_pRoot,m_size);   
                return iter; 
            }
        }    
    }
    post_iterator end_post() {
        Node* lnode_0 = find_last_node_pos(m_pRoot,0);
        Node* lnode_1 = find_last_node_pos(m_pRoot,1);
        if(m_pRoot==lnode_1){
            post_iterator iter(this,nullptr,m_pRoot,m_size); 
            return iter;
        }else{
            if(m_pRoot==lnode_0){
                post_iterator iter(this,nullptr,m_pRoot,m_size); 
                return iter;
            }
            else{
                post_iterator iter(this,nullptr,m_pRoot,m_size);   
                return iter; 
            }
        }       
     }
     // Iterator preorder
    pre_iterator begin_pre() { 
        pre_iterator iter(this,m_pRoot,m_pRoot,m_size);
        return iter; 
    }
    pre_iterator end_pre() {
        Node* lnode_0 = find_last_node_pre(m_pRoot,0);
        Node* lnode_1 = find_last_node_pre(m_pRoot,1);
        pre_iterator iter(this,nullptr,m_pRoot,m_size); 
        return iter;
     }
};
}
#endif
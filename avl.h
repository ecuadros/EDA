#ifndef __AVL_TREE_H__  
#define __AVL_TREE_H__
#include <iostream>
#include <string>
#include <cassert>
#include "types.h"
#include "keynode.h"
#include "xtrait.h"
#include <vector>
namespace AVL_Namespace {
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
class NodeAVL
{
    public:
        using KeyNode         = typename Traits::Node;
        using value_type      = typename Traits::value_type;
        using LinkedValueType = typename Traits::LinkedValueType;
    private:
        using Node = NodeAVL<Traits>;
    public:
        KeyNode  m_data;
        Node *m_pParent = nullptr;
        size_t m_depth=1;
        bool m_status=false;
        vector<Node *> m_pChild = {nullptr, nullptr}; // 2 hijos inicializados en nullptr
    public:
        NodeAVL(Node *pParent, value_type elem,LinkedValueType elem2, Node *p0 = nullptr, Node *p1 = nullptr) 
        : m_pParent(pParent),m_data(elem, elem2),m_status(false),m_depth(1)
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
        void      setpChild_nc(Node *pChild, size_t pos)  {   m_pChild[pos] = pChild;}
        size_t    get_height(){return m_depth;}
        void      setp_height(size_t h){m_depth=h;}
        void      setParent(Node *pParent){m_pParent=pParent;}
        Node    *&getParentRef() { return m_pParent;   }
};

template <typename Traits>
struct CAVLAscTraits
{
    using  value_type              = typename Traits::value_type;
    using  LinkedValueType         = typename Traits::LinkedValueType;
    using  Node                    = NodeAVL<Traits>;
    using  CompareFn = greater<value_type>;
};

template <typename Traits>
struct CAVLDescTraits
{
    using  value_type              = typename Traits::value_type;
    using  LinkedValueType         = typename Traits::LinkedValueType;
    using  Node                    = NodeAVL<Traits>;
    using  CompareFn = less<value_type>;
};
using TraitBTreeIntInt   = XTrait<int,int>;
using Traits_AVLAsc= CAVLAscTraits<TraitBTreeIntInt> ;
using Traits_AVLDesc= CAVLDescTraits<TraitBTreeIntInt> ;
template <typename Traits>
class CAVL
{
  public:
    using value_type = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using Node= typename Traits::Node;// In this context Node= NodeAVL<Traits>
    using CompareFn = typename Traits::CompareFn;
    using myself = CAVL<Traits>;
    typedef bt_iter_inorder<myself>    in_iterator;
    typedef bt_iter_postorder<myself>  post_iterator;
    typedef bt_iter_preorder<myself> pre_iterator;
    Node *pNode_route=nullptr;
  protected:
    Node    *m_pRoot = nullptr;
    size_t   m_size  = 0;
    CompareFn Compfn;
  public: 
    void    set_m_pRoot(Node *Root){m_pRoot=Root;}
    size_t  size()  const       { return m_size;       }
    bool    empty() const       { return size() == 0;  }
    void insert(value_type elem, LinkedValueType elem2) { 
        internal_insert1(elem,elem2,nullptr, m_pRoot); 
        Update_AVL_Tree(pNode_route);// Actualiza la altura del arbol al insertar un Node
    }

  protected:
    Node *CreateNode(Node *pParent, value_type &elem,LinkedValueType &elem2){
         return new Node(pParent,elem,elem2); }
    Node *internal_insert1(value_type &elem, LinkedValueType &elem2, Node *pParent, Node *&rpOrigin){
        if(!rpOrigin ) //  llegué al fondo de una rama
        {   ++m_size;
            rpOrigin = CreateNode(pParent,elem,elem2);
            pNode_route=rpOrigin;
            return rpOrigin;
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
    void Update_height(Node *pNode){
      while(pNode->getParent()){
          size_t h=1+max(height(pNode->getParent()->getChild(0)), height(pNode->getParent()->getChild(1)));
          pNode->getParent()->setp_height(h);
          pNode=pNode->getParent();
        }
    }
    int height(Node* pNode) {
        if (pNode == nullptr)
            return 0;
        return pNode->get_height();
    }
    int balance(Node* pNode) {
       if (!pNode)
          return 0;
       return height(pNode->getChild(0)) - height(pNode->getChild(1));
    }
    void Update_AVL_Tree(Node *pNode){
      Node *pNode_tmp=nullptr;
      while(pNode->getParent()){
        int balanceFactor = balance(pNode->getParent());// Determina el Balance del nodo insetado
        if(balanceFactor>1){
           pNode=general_rotation_(pNode,pNode_tmp,1,0);
        }
        else{
          if(balanceFactor<-1){
            pNode=general_rotation_(pNode,pNode_tmp,0,1);
          }
          else{
            size_t h=1+max(height(pNode->getParent()->getChild(0)), height(pNode->getParent()->getChild(1)));
            pNode->getParent()->setp_height(h);
            pNode_tmp=pNode;//Guardamos una copia para para preguntar luego la dirección izq o der
            pNode=pNode->getParent();
          }
       }  
      }
    }
    Node* general_rotation_(Node *pNode,Node *&pNode_tmp,int dir_x,int dir_y){
      Node *pParent=pNode->getParent();
      if(!pParent->getParent()){
            //cout<<"IZ"<< pNode->getData()<<endl;
            if(!pNode->getChild(dir_x)){
                pNode->setpChild_nc(pParent,dir_x);
                pNode->setParent(nullptr);
                set_m_pRoot(pNode);
                pParent->setpChild_nc(nullptr,dir_y);
                pParent->setParent(pNode);
                //Update Height
                pParent->setp_height(1+max(height(pParent->getChild(0)), height(pParent->getChild(1))));
                pNode->setp_height(1+max(height(pNode->getChild(0)), height(pNode->getChild(1))));
                pNode_tmp=pNode;//Guardo el pNode
                return pNode;
            }
            else{
                // Preguntar si el pNode vien de una rama 0 
                if(pNode->getChild(dir_y)==pNode_tmp){
                  Node *pChild1=pNode->getChild(dir_x);
                  pNode->setpChild_nc(pParent,dir_x);
                  pNode->setParent(nullptr);
                  set_m_pRoot(pNode);
                  pParent->setpChild_nc(pChild1,dir_y);
                  pParent->setParent(pNode);
                  pChild1->setParent(pParent);
                  //Update Height
                  pChild1->setp_height(1+max(height(pChild1->getChild(0)), height(pChild1->getChild(1))));
                  pParent->setp_height(1+max(height(pParent->getChild(0)), height(pParent->getChild(1))));
                  pNode->setp_height(1+max(height(pNode->getChild(0)), height(pNode->getChild(1))));
                  pNode_tmp=pNode;//Guardo el pNode
                  return pNode;
                }
                else{
                  Node *pChild1=pNode->getChild(dir_x);
                  Node *pNieto_x= pChild1->getChild(dir_x);
                  Node *pNieto_y= pChild1->getChild(dir_y);
                  pChild1->setpChild_nc(pNode,dir_y);
                  pChild1->setpChild_nc(pParent,dir_x);
                  pParent->setParent(pChild1);
                  pNode->setParent(pChild1);
                  pNode->setpChild_nc(pNieto_y,dir_x);
                  pParent->setpChild_nc(pNieto_x,dir_y);
                  pChild1->setParent(nullptr);
                  set_m_pRoot(pChild1);
                  if(pNieto_y){
                    pNieto_y->setParent(pNode);
                    pNieto_y->setp_height(1+max(height(pNieto_y->getChild(0)), height(pNieto_y->getChild(1))));
                  }
                  if(pNieto_x){
                    pNieto_x->setParent(pParent); 
                    pNieto_x->setp_height(1+max(height(pNieto_x->getChild(0)), height(pNieto_x->getChild(1)))); 
                  }
                  pNode->setp_height(1+max(height(pNode->getChild(0)), height(pNode->getChild(1))));
                  pParent->setp_height(1+max(height(pParent->getChild(0)), height(pParent->getChild(1))));
                  pChild1->setp_height(1+max(height(pChild1->getChild(0)), height(pChild1->getChild(1))));
                  pNode_tmp=pChild1;//Guardo el pNode
                  return pChild1;
                }
      
            }
      }
      else{
          Node *pGParent=pNode->getParentRef()->getParentRef();
          Node *pGPChild_y= pGParent->getChildRef(dir_y);
          Node *pGPChild_x= pGParent->getChildRef(dir_x);
          Node *pParent=pNode->getParentRef();//
          Node *pPChild_y= pParent->getChildRef(dir_y);
          Node *pPChild_x= pParent->getChildRef(dir_x);
          Node *pChild_y= pNode->getChildRef(dir_y);
          Node *pChild_x= pNode->getChildRef(dir_x);
          bool c1= (pGPChild_y==pParent)&&(pPChild_y==pNode)&&(pChild_y==pNode_tmp);//dir_y/dir_y/dir_y
          bool c1_e= (pGPChild_x==pParent)&&(pPChild_x==pNode)&&(pChild_x==pNode_tmp);//dir_x/dir_x/dir_x
          bool c2= (pGPChild_y==pParent)&&(pPChild_y==pNode)&&(pChild_x==pNode_tmp);//dir_y/dir_y/dir_x
          bool c2_e= (pGPChild_x==pParent)&&(pPChild_x==pNode)&&(pChild_y==pNode_tmp);//dir_x/dir_x/dir_y
          bool c3= (pGPChild_y==pParent)&&(pPChild_x==pNode)&&(pChild_x==pNode_tmp);//dir_y/dir_x/dir_x
          bool c3_e= (pGPChild_x==pParent)&&(pPChild_y==pNode)&&(pChild_y==pNode_tmp);//dir_x/dir_y/dir_y
          bool c4= (pGPChild_y==pParent)&&(pPChild_x==pNode)&&(pChild_y==pNode_tmp);//dir_y/dir_x/dir_y
          bool c4_e= (pGPChild_x==pParent)&&(pPChild_y==pNode)&&(pChild_x==pNode_tmp);//dir_x/dir_y/dir_x
          //cout<<"\nDirections : "<<c1<<c1_e<<c2<<c2_e<<c3<<c3_e<<c4<<c4_e<<endl;
          //Case 1
          if(c1||c1_e){
              //cout<<"\nHere I am C1"<<endl;
              if(!pNode->getChild(dir_x)){
                  // Case 1a:No tiene hijo en dir_x
                  pNode->setpChild_nc(pParent,dir_x);
                  pParent->setParent(pNode);
                  pGParent->setpChild_nc(pNode,dir_y);
                  pNode->setParent(pGParent);
                  pParent->setpChild_nc(nullptr,dir_y);
                  //Update Height
                  pParent->setp_height(1+max(height(pParent->getChild(0)), height(pParent->getChild(1))));
                  Node *pTemp=pNode;
                  while(pTemp){
                    pTemp->setp_height(1+max(height(pTemp->getChild(0)), height(pTemp->getChild(1))));
                    pTemp=pTemp->getParent();
                  }
                  //Go to Next pNode
                  pNode_tmp=pNode;//Guardo el pNode
                  pNode=pNode->getParent();
                  return pNode;
              }
              else{
                  //Case 1b:Tiene hijo en dir_x
                  Node *pChild1=pNode->getChild(dir_x);
                  pNode->setpChild_nc(pParent,dir_x);
                  pNode->setParent(pGParent);
                  pParent->setpChild_nc(pChild1,dir_y);
                  pParent->setParent(pNode);
                  pGParent->setpChild_nc(pNode,dir_y);
                  pChild1->setParent(pParent);
                  pChild1->setp_height(1+max(height(pChild1->getChild(0)), height(pChild1->getChild(1))));
                  pParent->setp_height(1+max(height(pParent->getChild(0)), height(pParent->getChild(1))));
                  Node *pTemp=pNode;
                  while(pTemp){
                    pTemp->setp_height(1+max(height(pTemp->getChild(0)), height(pTemp->getChild(1))));
                    pTemp=pTemp->getParent();
                  }
                  pNode_tmp=pNode;
                  pNode=pNode->getParent();
                  return pNode;

              }
          }
          //Case 2
          if(c2||c2_e){
                //cout<<"\nHere I am C2"<<endl;
                Node *pChild1=pNode->getChild(dir_x);
                Node *pChild1_x=pChild1->getChild(dir_x);
                Node *pChild1_y=pChild1->getChild(dir_y);
                Node *pChild1_z=nullptr;
                Node *pChild1_z2=nullptr;
                if(pChild1_x && !pChild1_y)
                  pChild1_z=pChild1_x;
                else{
                  if(pChild1_y && !pChild1_x)
                    pChild1_z=pChild1_y;
                  else{
                    if(!pChild1_y && !pChild1_x)
                        pChild1_z=nullptr;
                    else{
                        pChild1_z=pChild1_x;
                        pChild1_z2=pChild1_y;
                    }    
                  }
                     
                }
                if(!pChild1_z){
                  // There are not Child
                  pChild1->setpChild_nc(pNode,dir_y);
                  pChild1->setpChild_nc(pParent,dir_x);
                  pGParent->setpChild_nc(pChild1,dir_y);
                  pChild1->setParent(pGParent);
                  pNode->setParent(pChild1);
                  pParent->setParent(pChild1);
                  pParent->setpChild_nc(nullptr,dir_y);
                  pNode->setpChild_nc(nullptr,dir_x);
                  pNode->setp_height(1+max(height(pNode->getChild(0)), height(pNode->getChild(1))));
                  pParent->setp_height(1+max(height(pParent->getChild(0)), height(pParent->getChild(1))));
                  Node *pTemp=pChild1;
                  while(pTemp){
                    pTemp->setp_height(1+max(height(pTemp->getChild(0)), height(pTemp->getChild(1))));
                    pTemp=pTemp->getParent();
                  }
                  pNode_tmp=pChild1;
                  pNode=pChild1->getParent();
                  return pNode;
                }
                else{
                  pChild1->setpChild_nc(pNode,dir_y);//ok
                  pNode->setParent(pChild1);
                  pChild1->setpChild_nc(pParent,dir_x);//ok
                  pParent->setParent(pChild1);
                  pGParent->setpChild_nc(pChild1,dir_y);//ok
                  pChild1->setParent(pGParent);
                  pParent->setpChild_nc(pChild1_z,dir_y);
                  pChild1_z->setParent(pParent);
                  pNode->setpChild_nc(pChild1_z2,dir_x);//ok
                  if(pChild1_z2)
                    pChild1_z2->setParent(pNode);
                  pChild1_z->setp_height(1+max(height(pChild1_z->getChild(0)), height(pChild1_z->getChild(1))));
                  if(pChild1_z2)
                    pChild1_z2->setp_height(1+max(height(pChild1_z2->getChild(0)), height(pChild1_z2->getChild(1))));
                  pNode->setp_height(1+max(height(pNode->getChild(0)), height(pNode->getChild(1))));
                  pParent->setp_height(1+max(height(pParent->getChild(0)), height(pParent->getChild(1))));
                  Node *pTemp=pChild1;
                  while(pTemp){
                    pTemp->setp_height(1+max(height(pTemp->getChild(0)), height(pTemp->getChild(1))));
                    pTemp=pTemp->getParent();
                  }
                  pNode_tmp=pChild1;
                  pNode=pChild1->getParent();
                  return pNode;

                }
                
          }
          // Case 3
          if(c3||c3_e){
              //cout<<"\nHere I am C3"<<endl;
              Node *pTmp=pNode->getChild(dir_x);
              pParent->setpChild_nc(pTmp,dir_y);
              if(pTmp)
                pTmp->setParent(pParent);
              pParent->setParent(pNode);
              pNode->setpChild_nc(pParent,dir_x);
              pNode->setParent(pGParent);
              pGParent->setpChild_nc(pNode,dir_x);
              if(pTmp)
                pTmp->setp_height(1+max(height(pTmp->getChild(0)), height(pTmp->getChild(1))));
              pParent->setp_height(1+max(height(pParent->getChild(0)), height(pParent->getChild(1))));
              Node *pTemp=pNode;
              while(pTemp){
                pTemp->setp_height(1+max(height(pTemp->getChild(0)), height(pTemp->getChild(1))));
                pTemp=pTemp->getParent();
              }
              pNode_tmp=pNode;
              pNode=pNode->getParent();
              return pNode;
          }
          //Case 4
          if(c4||c4_e){
              Node *pChild1=pNode_tmp;
              Node *pChild1x=pChild1->getChild(dir_x);
              Node *pChild1y=pChild1->getChild(dir_y);
              pChild1->setpChild_nc(pNode,dir_y);
              pChild1->setpChild_nc(pParent,dir_x);
              pGParent->setpChild_nc(pChild1,dir_x);
              pChild1->setParent(pGParent);
              pNode->setParent(pChild1);
              pParent->setParent(pChild1);
              pParent->setpChild_nc(pChild1x,dir_y);
              pNode->setpChild_nc(pChild1y,dir_x);
              if(pChild1x)
                pChild1x->setParent(pParent);
              if(pChild1y)
                pChild1y->setParent(pNode);
              pNode->setp_height(1+max(height(pNode->getChild(0)), height(pNode->getChild(1))));
              pParent->setp_height(1+max(height(pParent->getChild(0)), height(pParent->getChild(1))));
              Node *pTemp=pChild1;
              while(pTemp){
                pTemp->setp_height(1+max(height(pTemp->getChild(0)), height(pTemp->getChild(1))));
                pTemp=pTemp->getParent();
              }
              pNode_tmp=pChild1;
              pNode=pChild1->getParent();
              return pNode;
          }
          else{
            cout<<endl;
            cout<<pNode_tmp->getData()<<endl;
            cout<<"Nos falta más casos"<<endl;
            return nullptr;
          }            
       }
   } 
};
}

#endif
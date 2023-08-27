#ifndef __AVL_TREE_H__  
#define __AVL_TREE_H__ 
#include <iostream>
#include <cassert>
#include "types.h"
#include "binarytree.h"
#include "iterator_btree.h"
using namespace std;

template <typename Traits>
class NodeAVL
{
    public:
        using KeyNode         = typename Traits::Node;
        using value_type      = typename Traits::value_type;
        using LinkedValueType = typename Traits::LinkedValueType;
    protected:
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
 
        // TODO: review if these functions must remain public/private
        void      setpChild(const Node *pChild, size_t pos)  {   m_pChild[pos] = pChild;}
        void      setpChild_nc(Node *pChild, size_t pos)  {   m_pChild[pos] = pChild;}
        void      setp_status(bool esp){m_status=esp;}
        size_t    get_height(){return m_depth;}
        void      setp_height(size_t h){m_depth=h;}
        bool      get_status(){return m_status;}
        Node    * getChild(size_t branch){ return m_pChild[branch];  }
        Node    *&getChildRef(size_t branch){ return m_pChild[branch];  }
        Node    * getParent() { return m_pParent;   }
        void      setParent(Node *pParent){m_pParent=pParent;}
        Node    *&getParentRef() { return m_pParent;   }
};

template <typename Traits>
struct AVLAscTraits
{
    using  value_type              = typename Traits::value_type;
    using  LinkedValueType         = typename Traits::LinkedValueType;
    using  Node                    = NodeAVL<Traits>;
    using  CompareFn = greater<value_type>;
};

template <typename Traits>
struct AVLDescTraits
{
    using  value_type              = typename Traits::value_type;
    using  LinkedValueType         = typename Traits::LinkedValueType;
    using  Node                    = NodeAVL<Traits>;
    using  CompareFn = less<value_type>;
};
using TraitBTreeIntInt   = XTrait<int,int>;
using Traits_AVLAsc= AVLAscTraits<TraitBTreeIntInt> ;
using Traits_AVLDesc= AVLDescTraits<TraitBTreeIntInt> ;

// TODO: AVL
template <typename Traits>
class CAVL: public BinaryTree<Traits>
{
  public:
    using value_type = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using Node= typename Traits::Node;// In this context Node= NodeAVL<Traits>
    using CompareFn = typename Traits::CompareFn;
    using Base = BinaryTree<Traits>;
    typedef CAVL<Traits>                myself;
    typedef bt_iter_inorder<myself>    in_iterator;
    typedef bt_iter_postorder<myself>  post_iterator;
    typedef bt_iter_preorder<myself> pre_iterator;

public:
  Node *pNode_route=nullptr;
public:
    CAVL():Base(){
          cout<<"Instancie Clase CAVL"<<endl;
        }    
public: 
    virtual void  insert(value_type elem, LinkedValueType elem2) override{ 
        internal_insert1(elem,elem2,nullptr, Base::m_pRoot); 
        Update_AVL_Tree(pNode_route);// Actualiza la altura del arbol al insertar un Node
    }
    virtual Node *internal_insert1(value_type &elem, LinkedValueType &elem2, Node *pParent, Node *&rpOrigin)override{
        if(!rpOrigin ) //  llegué al fondo de una rama
        {   ++Base::m_size;
            rpOrigin = Base::CreateNode(pParent,elem,elem2);
            pNode_route=rpOrigin;
            return rpOrigin;
        }
        //size_t branch = Base::Compfn(rpOrigin->getDataRef() ,elem);
        size_t branch = Base::Compfn(elem,rpOrigin->getDataRef());
        return internal_insert1(elem,elem2,rpOrigin,rpOrigin->getChildRef(branch));
    }  

    virtual void print(ostream &os)override{print(Base::m_pRoot, os, 0);}
    virtual void print(Node* pNode, std::ostream& os, size_t level)override{
        if (pNode) {
            Node* pParent = pNode->getParent();
            for (size_t i = 0; i < level; ++i) {
                os << " | ";
            }
            os << pNode->getDataRef() << "(" << (pParent ? std::to_string(pParent->getData()) : "Root") << ")" << std::to_string(pNode->get_height()) <<std::endl;
            print(pNode->getChild(1), os, level + 1);
            print(pNode->getChild(0), os, level + 1);
        }
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
                Base::set_m_pRoot(pNode);
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
                  Base::set_m_pRoot(pNode);
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
                  Base::set_m_pRoot(pChild1);
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
          //Case 1
          if(c1||c1_e){
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
                Node *pChild1=pNode->getChild(dir_x);
                Node *pChild1_x=pChild1->getChild(dir_x);
                Node *pChild1_y=pChild1->getChild(dir_y);
                Node *pChild1_z=nullptr;
                if(pChild1_x)
                  pChild1_z=pChild1_x;
                else{
                  if(pChild1_y)
                    pChild1_z=pChild1_y;
                  else
                    pChild1_z=nullptr; 
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
                  pNode->setpChild_nc(nullptr,dir_x);//ok
                  pChild1_z->setp_height(1+max(height(pChild1_z->getChild(0)), height(pChild1_z->getChild(1))));
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
              pNode->setpChild_nc(pParent,dir_x);
              pNode->setParent(pGParent);
              pParent->setpChild_nc(nullptr,dir_y);
              pParent->setParent(pNode);
              pGParent->setpChild_nc(pNode,dir_x);
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
              pChild1->setpChild_nc(pNode,dir_y);
              pChild1->setpChild_nc(pParent,dir_x);
              pGParent->setpChild_nc(pChild1,dir_x);
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
            cout<<endl;
            cout<<pNode_tmp->getData()<<endl;
            cout<<"Nos falta más casos"<<endl;
            return nullptr;
          }            
       }
   }
  
};

#endif
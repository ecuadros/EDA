#ifndef __ITERATOR_TREE_H__  
#define __ITERATOR_TREE_H__ 
#include <iostream>
#include <string>
using namespace std;
#define _DEF(_Container,_iter)  \
public: \
    typedef class general_iterator<_Container, _iter<Container> > Parent;     \
    typedef typename _Container::Node                             Node;       \
    typedef _iter<_Container>                                     myself;

template <typename Container>
class bt_iter_inorder : public general_iterator<Container,  class bt_iter_inorder<Container>> // 
{  _DEF(Container, bt_iter_inorder); // TODO: llevar esta misma idea a todos container ya existentes

  private:
    Node *m_pRoot=nullptr;
    size_t count=0;
  public:
    bt_iter_inorder(Container *pContainer, Node *pNode,Node *pRoot) : Parent (pContainer,pNode),m_pRoot(pRoot) {}
    bt_iter_inorder(myself &other)  : Parent (other),m_pRoot(other.m_pRoot) {}
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
        Node *NextNode= in_order(Parent::m_pNode);
        if(NextNode){
            Parent::m_pNode= NextNode;
        }
        return *this;
    }

    
};
// 
template <typename Container>
class bt_iter_postorder : public general_iterator<Container,  class bt_iter_postorder<Container>> // 
{  _DEF(Container, bt_iter_postorder); // TODO: llevar esta misma idea a todos container ya existentes

  private:
    Node *m_pRoot=nullptr;
    size_t count=0;
  public:
    bt_iter_postorder(Container *pContainer, Node *pNode,Node *pRoot) : Parent (pContainer,pNode),m_pRoot(pRoot) {}
    bt_iter_postorder(myself &other)  : Parent (other),m_pRoot(other.m_pRoot) {}
    bt_iter_postorder(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

  private:
    Node *post_order(Node *pNode){
        if(pNode){
            if(count==1)
               pNode->setp_status(true); 
            if(pNode->getChild(0) && pNode->getChild(0)->get_status()==false){
                while(pNode->getChild(0)&&pNode->getChild(0)->get_status()==false)
                    pNode= pNode->getChild(0); 
                if(pNode->getChild(1)==nullptr){
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
                if(pNode->getChild(1)&& pNode->getChild(1)->get_status()==false){
                    pNode=pNode->getChild(1);
                    pNode= post_order(pNode);
                    return pNode;
                }
                else{
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
                        if(count==1){
                            pNode=pParent;
                            pNode->setp_status(true);
                            return pNode;
                        }
                        else{
                            if(!pNode->get_status()){
                                pNode->setp_status(true);
                                return pNode; 
                            }
                            else{
                              pNode=pParent;
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
        Node *NextNode= post_order(Parent::m_pNode);
        if(NextNode){
            Parent::m_pNode= NextNode;
        }
        return *this;
    }

    
};
#endif
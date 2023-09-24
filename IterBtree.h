#ifndef ___ITERATOR_BTREE_H__
#define ___ITERATOR_BTREE_H__

template <typename Container>

class forward_iterator_Btree  {

public:

    typedef typename Container::ObjectInfo     Node ;  // de tipo Object INfo
    typedef typename  Node::  valuetype    value_type;
    typedef forward_iterator_Btree<Container> myself;


protected:
    Container *m_pContainer;
    Node  *m_pNode;
 

public:
    forward_iterator_Btree(Container *pContainer, Node *pNode): m_pContainer(pContainer), m_pNode(pNode) {}
    forward_iterator_Btree(myself &other): m_pContainer(other.m_pContainer), m_pNode(other.m_pNode){}
    forward_iterator_Btree(myself &&other) {  
                 m_pContainer = move(other.m_pContainer);
                m_pNode      = move(other.m_pNode);
               
            }

    myself operator=(myself &iter){  
              m_pContainer = move(iter.m_pContainer);
              m_pNode      = move(iter.m_pNode);
              return *(myself *)this; 
          }

    bool operator==(myself iter)   { return m_pNode == iter.m_pNode; }
    bool operator!=(myself iter)   { return !(*this == iter);        }
    //...............................................................
  
    value_type &operator*() { return m_pNode->getKeyRef();}
    myself operator++() { 

          m_pNode=m_pNode->next;
        
          return *this; 
          }
};

template <typename Container>

class backward_iterator_Btree {

public:

    typedef typename Container::ObjectInfo     Node ; 
    typedef typename  Node::  valuetype    value_type;
    typedef backward_iterator_Btree<Container> myself;


protected:
    Container *m_pContainer;
    Node  *m_pNode;
 

public:
    backward_iterator_Btree(Container *pContainer, Node *pNode): m_pContainer(pContainer), m_pNode(pNode) {}
    backward_iterator_Btree(myself &other): m_pContainer(other.m_pContainer), m_pNode(other.m_pNode){}
    backward_iterator_Btree(myself &&other) {  
                 m_pContainer = move(other.m_pContainer);
                m_pNode      = move(other.m_pNode);
               
            }

    myself operator=(myself &iter){  
              m_pContainer = move(iter.m_pContainer);
              m_pNode      = move(iter.m_pNode);
              return *(myself *)this; 
          }

    bool operator==(myself iter)   { return m_pNode == iter.m_pNode; }
    bool operator!=(myself iter)   { return !(*this == iter);        }
    //...............................................................
  
    value_type &operator*() { return m_pNode->getKeyRef();}
    myself operator++() { 

          m_pNode=m_pNode->prev;
        
          return *this; 
          }
};

#endif
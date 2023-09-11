#ifndef __ITERATORTREE_H__
#define __ITERATORTREE_H__
#include <algorithm>
#include <utility>

template <typename Container, class IteratorBase> // 
class general_iterator_Tree
{public:
    typedef typename Container::Node    Node;
    typedef typename Node::Type         Type;
    typedef general_iterator_Tree<Container, IteratorBase> myself; 
    
protected:
    Container *m_pContainer;
    Node      *m_pNode;
public:
    general_iterator_Tree(Container *pContainer, Node *pNode)
        : m_pContainer(pContainer), m_pNode(pNode) {}
    general_iterator_Tree(myself &other) 
          : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode){}
    general_iterator_Tree(myself &&other) 
          {   m_pContainer = move(other.m_pContainer);
              m_pNode      = move(other.m_pNode);
          }
    IteratorBase operator=(IteratorBase &iter)
          {   m_pContainer = move(iter.m_pContainer);
              m_pNode      = move(iter.m_pNode);
              return *(IteratorBase *)this; // Pending static_cast?
          }

    bool operator==(IteratorBase iter)   { return m_pNode == iter.m_pNode; }
    bool operator!=(IteratorBase iter)   { return !(*this == iter);        }
    
    Node &operator*(){return *m_pNode;}
   // Type &operator*()                    { return m_pNode->getDataRef();   }
 

};
#define _DEFTREE(_Container, _iter)  \
public: \
    typedef class general_iterator_Tree<_Container, _iter<Container> > Parent;     \
    typedef typename _Container::Node                             Node;       \
    typedef _iter<_Container>                                     myself;
#endif
 
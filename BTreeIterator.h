#ifndef __BTREEITERATOR_H__
#define __BTREEITERATOR_H__

//Se utiliza como plantilla la clase 
//iterator realizada en LinkedList

template <typename Container>
class forward_iterator
{
public:
  //Dentro de la clase BtreeTrait
  typedef typename Container::keyType Node; 
  //typedef typename Node::Type         Type;
  typedef forward_iterator<Container> myself;

protected:
    Container *m_pContainer;
    Node      *m_pNode;

public:
  forward_iterator(Container *pContainer, Node *pNode)
  {
    m_pContainer = pContainer;
    m_pNode = pNode;
  }
  forward_iterator(myself &other)
  {
    m_pContainer = other.m_pContainer;
    m_pNode = other.m_pNode;
  }
  forward_iterator(myself &&other)
  {
    m_pContainer = move(other.m_pContainer);
    m_pNode = move(other.m_pNode);
  } 
public:
  myself operator++()
  {
    m_pNode = m_pNode->getpNext();
    return *this;
  }

  myself operator=(myself &iter)
  {
    m_pContainer = move(iter.m_pContainer);
    m_pNode = move(iter.m_pNode);
    return *(myself *) this;
  }
  bool operator==(myself iter)
  {
      return m_pNode == iter.m_pNode;
  }

  bool operator!=(myself iter)
  {
    return m_pNode != iter.m_pNode;
  }

//  Type &operator*()
  Node &operator*()
  {
    return m_pNode -> getDataRef();
  }
};



template <typename Container>
class backward_iterator
{
public:
  //Dentro de la clase BtreeTrait
  typedef typename Container::keyType Node; 
  //typedef typename Node::Type         Type;
  typedef backward_iterator<Container> myself;

protected:
    Container *m_pContainer;
    Node      *m_pNode;

public:
  backward_iterator(Container *pContainer, Node *pNode)
  {
    m_pContainer = pContainer;
    m_pNode = pNode;
  }
  backward_iterator(myself &other)
  {
    m_pContainer = other.m_pContainer;
    m_pNode = other.m_pNode;
  }
  backward_iterator(myself &&other)
  {
    m_pContainer = move(other.m_pContainer);
    m_pNode = move(other.m_pNode);
  } 
public:
  myself operator++()
  {
    m_pNode = m_pNode->getpPrev();
    return *this;
  }

  myself operator=(myself &iter)
  {
    m_pContainer = move(iter.m_pContainer);
    m_pNode = move(iter.m_pNode);
    return *(myself *) this;
  }
  bool operator==(myself iter)
  {
      return m_pNode == iter.m_pNode;
  }

  bool operator!=(myself iter)
  {
    return m_pNode != iter.m_pNode;
  }

//  Type &operator*()
  Node &operator*()
  {
    return m_pNode -> getDataRef();
  }
};






template <typename _T, typename _V>
struct LLTraitAsc
{
  using T = _T;
  using V = _V;
  using Node = NodeLinkedList<T, V>;
  using CompareFn = less<T>;
};

template <typename _T, typename _V>
struct LLTraitDesc
{
  using T = _T;
  using V = _V;
  using Node = NodeLinkedList<T, V>;
  using CompareFn = greater<T>;
};

#endif
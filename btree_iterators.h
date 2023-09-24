#ifndef ___BTREE_ITERATOR_H__
#define ___BTREE_ITERATOR_H__

template <typename Container>
class forward_iterator
{
public:
  typedef typename Container::BTNode BTNode;
  typedef forward_iterator<Container> myself;

private:
  Container *m_pContainer;
  BTNode *m_pNode;

public:
  forward_iterator(Container *pContainer, BTNode *pNode) : m_pContainer(pContainer), m_pNode(pNode) {}
  forward_iterator(myself &other)
  {
    m_pContainer = other.m_pContainer;
    m_pNode = other.m_pNode;
  }
  forward_iterator(myself &&other)
  {
    m_pContainer = std::move(other.m_pContainer);
    m_pNode = std::move(other.m_pNode);
  }
  forward_iterator operator=(forward_iterator &iter)
  {
    m_pContainer = std::move(iter.m_pContainer);
    m_pNode = std::move(iter.m_pNode);
  }
  bool operator==(forward_iterator iter)
  {
    return m_pNode == iter.m_pNode;
  }
  bool operator!=(forward_iterator iter)
  {
    return !(*this == iter);
  }
  BTNode &operator*()
  {
    return *m_pNode;
  }
  myself operator++()
  {
    return *this;
  }
  
};

// template <typename Container>
// class backward_iterator
// {
// public:
//   typedef typename Container::Node Node;
//   typedef typename Node::Type Type;
//   typedef backward_iterator<Container> myself;

// private:
//   Container *m_pContainer;
//   Node *m_pNode;

// public:
//   backward_iterator(Container *pContainer, Node *pNode) : m_pContainer(pContainer), m_pNode(pNode) {}
//   backward_iterator(myself &other)
//   {
//     m_pContainer = other.m_pContainer;
//     m_pNode = other.m_pNode;
//   }
//   backward_iterator(myself &&other)
//   {
//     m_pContainer = std::move(other.m_pContainer);
//     m_pNode = std::move(other.m_pNode);
//   }
//   backward_iterator operator=(backward_iterator &iter)
//   {
//     m_pContainer = std::move(iter.m_pContainer);
//     m_pNode = std::move(iter.m_pNode);
//   }
//   bool operator==(backward_iterator iter)
//   {
//     return m_pNode == iter.m_pNode;
//   }
//   bool operator!=(backward_iterator iter)
//   {
//     return !(*this == iter);
//   }
//   Node &operator*()
//   {
//     return *m_pNode;
//   }
//   backward_iterator operator++()
//   {
//     return *this;
//   }
// };

#endif
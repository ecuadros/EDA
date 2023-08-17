#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__
//#include <utility>
//#include <algorithm>
#include <cassert>
#include "types.h"
#include "binarytree.h"
//#include "util.h"
using namespace std;

template <typename T>
class NodeAVL : public NodeBinaryTree<T>
{
public:
  typedef T Type;
private:
  using Parent = NodeBinaryTree<T>;
public:
  size_t m_depth = 1;
public:
  NodeAVL(Node *pParent, T data, Node *p0 = nullptr, Node *p1 = nullptr)
      : Parent(pParent, p0, p1)
  {}
};

template <typename Traits>
class CAVL : public BinaryTree<typename Traits::T, typename Traits::Node>
{
public:
  typedef typename Traits::T value_type;
  typedef typename Traits::Node Node;

  typedef typename Traits::CompareFn CompareFn;
  typedef CAVL<Traits> myself;
  // typedef binary_tree_iterator<myself> iterator;

protected:
public:
  virtual void insert(value_type elem, LinkedValueType value)
  {
    Node *newNode = new NodeAVL<Node>(nullptr, elem);
    if (this->m_pRoot == nullptr)
    {
      this->m_pRoot = newNode;
    }
    else
    {
      Node *current = this->m_pRoot;
      while (true)
      {
        if (Traits::CompareFn()(elem, current->m_data))
        {
          if (current->m_pLeft == nullptr)
          {
            current->m_pLeft = newNode;
            break;
          }
          else
          {
            current = current->m_pLeft;
          }
        }
        else
        {
          if (current->m_pRight == nullptr)
          {
            current->m_pRight = newNode;
            break;
          }
          else
          {
            current = current->m_pRight;
          }
        }
      }
    }
  }
};

#endif
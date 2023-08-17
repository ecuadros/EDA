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
class NodeAVL : public NodeBinaryTree
{
public:
  typedef T         Type;
private:
  using Parent = NodeBinaryTree<T> Node;
  public:
    size_t m_depth = 1; 
  public:
    NodeAVL(Node *pParent, T data, Node *p0 = nullptr, Node *p1 = nullptr) 
        : Parent(pParent, p0, p1)
    {}

};

// template <typename _T>
// struct BinaryTreeAscTraits
// {
//     using  T         = _T;
//     using  Node      = NodeBinaryTree<T>;
//     using  CompareFn = less<T>;
// };

// template <typename _T>
// struct BinaryTreeDescTraits
// {
//     using  T         = _T;
//     using  Node      = NodeBinaryTree<T>;
//     using  CompareFn = greater<T>;
// };

// TODO: AVL
template <typename Traits>
class CAVL: public BinaryTree
{
  public:
    typedef typename Traits::T          value_type;
    typedef typename Traits::Node       Node;
    
    typedef typename Traits::CompareFn      CompareFn;
    typedef CAVL<Traits>                myself;
    // typedef binary_tree_iterator<myself>    iterator;

protected:
public: 
    // TODO: insert must receive two paramaters: elem and LinkedValueType value
    virtual void    insert(value_type &elem, LinkedValueType value){ 
        Parent::internal_insert(elem, nullptr, m_pRoot); 
    }
};

#endif
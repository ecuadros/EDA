#ifndef __BINARY_TREE_AVL_H__
#define __BINARY_TREE_AVL_H__
#include <cassert>
#include "types.h"
#include "binarytree.h"

using namespace std;

// *************************************************************************** //
// ****************************** AVL NODE *********************************** //
// *************************************************************************** //

template <typename Traits>
class NodeAVL : public NodeBinaryTree<Traits>
{
public:
    using Type = typename Traits::value_type;
    using value_type = typename Traits::value_type;
    using KeyType = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using InnerNode = typename Traits::Node;
    typedef NodeBinaryTree<Traits> Parent;
    typedef NodeBinaryTree<Traits> Node;

    size_t m_height = 1;

    NodeAVL(Node *pParent, value_type data, LinkedValueType val, Node *p0 = nullptr, Node *p1 = nullptr) : Parent(pParent, data, val, p0, p1)
    {
    }

    NodeAVL(const Parent& node) : Parent(node)
    {
    }
};

// *************************************************************************** //
// ********************************* AVL ************************************* //
// *************************************************************************** //

// DONE: AVL
template <typename Traits>
class CAVL : public BinaryTree<Traits>
{
public:
    typedef typename Traits::value_type value_type;
    typedef typename Traits::CompareFn CompareFn;
    typedef typename Traits::LinkedValueType LinkedValueType;
    typedef CAVL<Traits> myself;
    typedef binary_tree_iterator<myself> iterator;
    using Parent = BinaryTree<Traits>;
    using Node = NodeBinaryTree<Traits>;
    Node *m_pRoot = nullptr;

    size_t depth(Node *pNode)
    {
        if (pNode == nullptr)
            return 0;
        return pNode->m_height;
    }

    int balanceFactor(Node *pNode)
    {
        if (pNode == nullptr)
            return 0;
        int depthLeft = depth(pNode->getChildRef(0));
        int depthRight = depth(pNode->getChildRef(1));
        return depthLeft - depthRight;
    }

    Node *rotateLeft(Node *tree)
    {
        Node *a = tree->getChildRef(1);
        Node *b = a->getChildRef(0);
        a->getChildRef(0) = tree;
        tree->getChildRef(1) = b;

        tree->m_height = max(depth(tree->getChildRef(0)), depth(tree->getChildRef(1))) + 1;
        a->m_height = max(depth(a->getChildRef(0)), depth(a->getChildRef(1))) + 1;

        return a;
    }

    Node *rotateRight(Node *tree)
    {
        Node *a = tree->getChildRef(0);
        Node *b = a->getChildRef(1);
        a->getChildRef(1) = tree;
        tree->getChildRef(0) = b;

        tree->m_height = max(depth(tree->getChildRef(0)), depth(tree->getChildRef(1))) + 1;
        a->m_height = max(depth(a->getChildRef(0)), depth(a->getChildRef(1))) + 1;

        return a;
    }

    Node *balance(Node *tree)
    {
        if (tree == nullptr)
            return nullptr;
        tree->m_height = max(depth(tree->getChildRef(0)), depth(tree->getChildRef(1))) + 1;
        int bf = balanceFactor(tree);
        Node* left = tree->getChildRef(0);
        Node* right = tree->getChildRef(1);

        if(bf > 1)
        {
            if(balanceFactor(tree->getChildRef(0)) >= 0)
            {
                return rotateRight(tree);
            }
            else
            {
                tree->getChildRef(0) = rotateLeft(tree->getChildRef(0));
                return rotateRight(tree);
            }
        }

        if(bf < -1)
        {
            if(balanceFactor(tree->getChildRef(1)) <= 0)
            {
                return rotateLeft(tree);
            }
            else
            {
                tree->getChildRef(1) = rotateRight(tree->getChildRef(1));
                return rotateLeft(tree);
            }
        }
        // cout<<"Here";

        return tree;
    }

    // DONE: insert must receive two paramaters: elem and LinkedValueType value
    virtual void insert(value_type &elem, LinkedValueType value)
    {
        Parent::insert(elem, value);
        Parent::m_pRoot = balance(Parent::m_pRoot);
    }

    void print(ostream &os)
    {
        Parent::print(os);
    }
};

#endif
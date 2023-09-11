#ifndef __BINARY_TREE_H__  
#define __BINARY_TREE_H__ 
#include <cassert>
#include <vector>
#include "types.h"
#include "tree_iterators.h"
using namespace std;

string operator*(string text, size_t n)
{
    for(size_t i = 0;i<n;i++){
        text += text;
    }
    return text;
}

template <typename K, typename V>
class NodeBinaryTree : public KeyNode<K,V>{
public:
    using Type = K;
    using value_type   = K;
    using LinkedValueType = V;
    using Parent = class KeyNode<K,V>;
private:
  typedef NodeBinaryTree<K,V> Node;
  public:
    Node *  m_pParent = nullptr;
    vector<Node *> m_pChild = {nullptr, nullptr}; // 2 hijos inicializados en nullptr
    size_t m_level = 0;
    bool m_visited = false;
  public:
    NodeBinaryTree(value_type key, LinkedValueType value, Node *pParent = nullptr, size_t level = 0, Node *p0 = nullptr, Node *p1 = nullptr) 
        : m_pParent(pParent), m_level(level), Parent(key, value){
        m_pChild[0] = p0;   m_pChild[1] = p1;
    }

    NodeBinaryTree(value_type key, Node *pParent = nullptr, size_t level = 0, Node *p0 = nullptr, Node *p1 = nullptr) 
        : m_pParent(pParent), m_level(level){
        Parent::m_key =  key; m_pChild[0] = p0; m_pChild[1] = p1;
    }

    NodeBinaryTree(){}

    void      setChild(const Node *pChild, size_t pos)  {   m_pChild[pos] = pChild;  }
    Node    * getChild(size_t branch){ return m_pChild[branch];  }
    Node    *&getChildRef(size_t branch){ return m_pChild[branch];  }
    Node    * getParent() { return m_pParent;   }
    
    size_t getLevel(){return m_level;}
    bool getVisited(){return m_visited;}
    void setVisited(bool visited){m_visited = visited;}

};

template <typename _K, typename _V, typename _CompareFn = std::less< NodeBinaryTree<_K, _V> & >>
struct BinaryTreeTrait{
    using  value_type      = _K;
    using  LinkedValueType = _V;
    using  Node      = NodeBinaryTree<_K,_V>;
    using  CompareFn = _CompareFn;
};

using BinaryTreeTraitIntIntAsc      = BinaryTreeTrait<int  , int   , std::less<NodeBinaryTree<int, int > >>;
using BinaryTreeTraitFloatStringAsc = BinaryTreeTrait<float, string, std::greater<NodeBinaryTree<float, string > >>;

template <typename Traits>
class BinaryTree
{
  public:
    typedef typename Traits::value_type      value_type;
    typedef typename Traits::LinkedValueType LinkedValueType;
    typedef typename Traits::Node       Node;
    
    typedef typename Traits::CompareFn      CompareFn;
    typedef BinaryTree<Traits>              myself;
    typedef inorder_iterator<myself>    in_iterator;
    typedef postorden_iterator<myself>    post_iterator;
    typedef print_iterator<myself>    prints_iterator;
    typedef preorden_iterator<myself>    pre_iterator;

protected:
    Node    *m_pRoot = nullptr;
    size_t   m_size  = 0;
    CompareFn Compfn;
public: 
    size_t  size()  const       { return m_size;       }
    bool    empty() const       { return size() == 0;  }
    void    insert(value_type &key, LinkedValueType value) { internal_insert(key, value, nullptr, m_pRoot,0);}

    in_iterator inbegin(bool printed = 0) { in_iterator iter(this,leftMost(m_pRoot),printed); return iter;    }
    in_iterator inend(bool printed = 0)   { in_iterator iter(this, rightMost(m_pRoot),printed);return iter;    }

    post_iterator postbegin(bool printed = 0) { post_iterator iter(this, leftMost(m_pRoot),printed);    return iter;    }
    post_iterator postend(bool printed = 0)   { post_iterator iter(this, m_pRoot,printed);    return iter;    }

    pre_iterator prebegin(bool printed = 0) { pre_iterator iter(this, m_pRoot,printed);    return iter;    }
    pre_iterator preend(bool printed = 0)   { pre_iterator iter(this, rightMost(m_pRoot),printed);    return iter;    }
    prints_iterator printbegin(bool printed = 0) { prints_iterator iter(this, rightMost(m_pRoot),printed);    return iter;    }
    prints_iterator printend(bool printed = 0)   { prints_iterator iter(this, leftMost(m_pRoot),printed);    return iter;    }

protected:
    Node *CreateNode(value_type &key, LinkedValueType value, Node *pParent, size_t level){ return new Node(key, value, pParent, level); }
    Node *internal_insert(
        value_type &key, LinkedValueType &value, Node *pParent,
        Node *&rpOrigin, size_t level = 0)
    {
        if( !rpOrigin )
        {   
            ++m_size;
            return (rpOrigin = CreateNode(key, value, pParent, level));
        }

        size_t siblingBranch = Compfn(key, rpOrigin->getDataRef() );
        Node* nextNode = internal_insert(key, value, rpOrigin, rpOrigin->getChildRef(siblingBranch), level+1);
        return nextNode;
    }

    Node* sideMost(int side){
        //Node *m_pRoot = nullptr;
        while(m_pRoot != nullptr && m_pRoot->getChild(side) != nullptr)
			m_pRoot = m_pRoot->getChild(side);
        return m_pRoot;
    }

    Node* leftMost(Node *pNode){
        assert(pNode != nullptr);
        while(pNode != nullptr && pNode->getChild(0) != nullptr)
			pNode = pNode->getChild(0);
        return pNode;
    }

    Node* rightMost(Node *pNode){
        assert(pNode != nullptr);
        while(pNode != nullptr && pNode->getChild(1) != nullptr)
			pNode = pNode->getChild(1);
        return pNode;
    }

public:
    void inorder(ostream &os, void (*func) (Node& node, ostream &os)){
        foreach(inbegin(),inend(), func, os);
        foreach(inbegin(1),inend(1), [](Node& node){});
    }

    // TODO: generalize this function by using iterators and apply any function
    // Create a new iterator to walk in postorder
    void postorder(ostream &os, void (*func) (Node& node, ostream &os)){
        foreach(postbegin(),postend(), func,os);
        foreach(inbegin(1),inend(1), [](Node& node){});
    }

    // TODO: generalize this function by using iterators and apply any function
    // Create a new iterator to walk in postorder
    void preorder(ostream &os, void (*func) (Node& node, ostream &os)){
        foreach(prebegin(),preend(), func,os);
        foreach(inbegin(1),inend(1), [](Node& node){});
    }
    
    // TODO: generalize this function by using iterators and apply any function
    void print(ostream &os, void (*func) (Node& node, ostream &os)){
        foreach(printbegin(),printend(), func, os);
        foreach(inbegin(1),inend(1), [](Node& node){});
    }

    // TODO: generalize this function by using iterators and apply any function
    void inorder(void (*func) (Node& Node, LinkedValueType& value),LinkedValueType& value){
        foreach(inbegin(),inend(), func, value);
        foreach(inbegin(1),inend(1), [](Node& node){});
    }
};

#endif
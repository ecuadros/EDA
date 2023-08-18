#ifndef __BINARY_TREE_H__  
#define __BINARY_TREE_H__ 
//#include <utility>
//#include <algorithm>
#include <cassert>
#include "types.h"
//#include "util.h"
#include "tree_iterators.h"
using namespace std;

template <typename T>
class NodeBinaryTree
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
    // TODO: Change T by KeyNode
  typedef T         Type;
    using Type = K;
    using value_type   = K;
    using LinkedValueType = V;
    using Parent = class KeyNode<K,V>;
private:
  typedef NodeBinaryTree<T> Node;
  typedef NodeBinaryTree<K,V> Node;
  private : bool m_printed = 0;
  public:
    T       m_data;
    //T       m_data;
    Node *  m_pParent = nullptr;
    vector<Node *> m_pChild = {nullptr, nullptr}; // 2 hijos inicializados en nullptr
    size_t m_level;
  public:
    NodeBinaryTree(Node *pParent, T data, Node *p0 = nullptr, Node *p1 = nullptr) 
        : m_pParent(pParent), m_data(data)
    {   m_pChild[0] = p0;   m_pChild[1] = p1;   }

// TODO: Keynode 
    T         getData()                {   return m_data;    }
    T        &getDataRef()             {   return m_data;    }
    NodeBinaryTree(value_type key, // θɪŋk ðɪs ɪz juːst baɪ kəmˈper
                   Node *pParent= nullptr,                   
                   Node *p0 = nullptr,
                   Node *p1 = nullptr) 
        : m_pParent(pParent){
        Parent::m_key = key;
        m_pChild[0] = p0;   m_pChild[1] = p1;
    }

    NodeBinaryTree(Node *pParent,
                   value_type key,
                   LinkedValueType value,
                   size_t level,
                   Node *p0 = nullptr,
                   Node *p1 = nullptr) 
        : m_pParent(pParent), Parent(key,value){
        m_level = level;
        m_pChild[0] = p0;
        m_pChild[1] = p1;
    }

 // TODO: review if these functions must remain public/private
    void      setpChild(const Node *pChild, size_t pos)  {   m_pChild[pos] = pChild;  }
    Node    * getChild(size_t branch){ return m_pChild[branch];  }
    Node    *&getChildRef(size_t branch){ return m_pChild[branch];  }
    Node    * getParent() { return m_pParent;   }
};

#define _DEF(_Container, _iter)  \
public: \
    typedef class general_iterator<_Container, _iter<Container> > Parent;     \
    typedef typename _Container::Node                             Node;       \
    typedef _iter<_Container>                                     myself;
    bool getPrinted(){return m_printed;}
    void setPrinted(bool printed){m_printed = printed;}
    size_t getLevel(){return m_level;}

template <typename Container>
class binary_tree_iterator : public general_iterator<Container,  class binary_tree_iterator<Container> > // 
{  _DEF(Container, binary_tree_iterator); // TODO: llevar esta misma idea a todos container ya existentes

  public:
    binary_tree_iterator(Container *pContainer, Node *pNode) : Parent (pContainer,pNode) {}
    binary_tree_iterator(myself &other)  : Parent (other) {}
    binary_tree_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

public:
    binary_tree_iterator operator++() { //Parent::m_pNode = (Node *)Parent::m_pNode->getpNext();  
                                        return *this;
                                  }
};

template <typename _T>
struct BinaryTreeAscTraits
{
    using  T         = _T;
    using  Node      = NodeBinaryTree<T>;
    using  CompareFn = less<T>;
};

template <typename _T>
struct BinaryTreeDescTraits
{
    using  T         = _T;
    using  Node      = NodeBinaryTree<T>;
    using  CompareFn = greater<T>;
template <typename _K,
          typename _V,
          typename _CompareFn = std::less< NodeBinaryTree<_K, _V> & >>
struct BinaryTreeTrait{
    using  value_type      = _K;
    using  LinkedValueType = _V;
    using  Node      = NodeBinaryTree<_K,_V>;
    using  CompareFn = _CompareFn;
};

template <typename Traits>
class BinaryTree
{
  public:
    typedef typename Traits::T          value_type;
    typedef typename Traits::value_type      value_type;
    typedef typename Traits::LinkedValueType LinkedValueType;
    typedef typename Traits::Node       Node;

    typedef typename Traits::CompareFn      CompareFn;
    typedef BinaryTree<Traits>              myself;
    typedef binary_tree_iterator<myself>    iterator;
    typedef binary_tree_inorder_iterator<myself>    in_iterator; //inorder iterator
    typedef binary_tree_postorden_iterator<myself>    post_iterator; //postorden iterator
    typedef binary_tree_preorden_iterator<myself>    pre_iterator; //preorden iterator
    typedef binary_tree_print_iterator<myself>    print_iterator; //print as tree iterator

protected:
    Node    *m_pRoot = nullptr;
@@ -91,84 +95,73 @@ class BinaryTree
    size_t  size()  const       { return m_size;       }
    bool    empty() const       { return size() == 0;  }
    // TODO: insert must receive two paramaters: elem and LinkedValueType value
    void    insert(value_type &elem) { internal_insert1(elem, nullptr, m_pRoot);  }
    void    insert(value_type &key, LinkedValueType value) { internal_insert(key, value, nullptr, m_pRoot,0);  }

    in_iterator inbegin(bool printed = 0) { in_iterator iter(this, leftMost(m_pRoot),printed); return iter;    }
    in_iterator inend(bool printed = 0)   { in_iterator iter(this, rightMost(m_pRoot),printed);return iter;    }
    post_iterator postbegin(bool printed = 0) { post_iterator iter(this, leftMost(m_pRoot),printed);    return iter;    }
    post_iterator postend(bool printed = 0)   { post_iterator iter(this, m_pRoot,printed);    return iter;    }
    pre_iterator prebegin(bool printed = 0) { pre_iterator iter(this, m_pRoot,printed);    return iter;    }
    pre_iterator preend(bool printed = 0)   { pre_iterator iter(this, rightMost(m_pRoot),printed);    return iter;    }
    print_iterator printbegin(bool printed = 0) { print_iterator iter(this, rightMost(m_pRoot),printed);    return iter;    }
    print_iterator printend(bool printed = 0)   { print_iterator iter(this, leftMost(m_pRoot),printed);    return iter;    }

protected:
    Node *CreateNode(Node *pParent, value_type &elem){ return new Node(pParent, elem); }
    Node *internal_insert1(value_type &elem, Node *pParent, Node *&rpOrigin)
    Node *CreateNode(Node *pParent, value_type &key, LinkedValueType value, size_t level){ return new Node(pParent, key, value, level); }
    Node *internal_insert(value_type &key, LinkedValueType value, Node *pParent, Node *&rpOrigin, size_t level)
    {
        if( !rpOrigin ) //  llegué al fondo de una rama
        {   ++m_size;
            return (rpOrigin = CreateNode(pParent, elem));
            return (rpOrigin = CreateNode(pParent, key, value,level));
        }
        size_t branch = Compfn(elem, rpOrigin->getDataRef() );
        return internal_insert1(elem, rpOrigin, rpOrigin->getChildRef(branch));
        size_t branch = Compfn(key, rpOrigin->getDataRef() );
        return internal_insert(key, value, rpOrigin, rpOrigin->getChildRef(branch), level+1);
    }
public:
    void inorder  (ostream &os)    {   inorder  (m_pRoot, os, 0);  }
    void postorder(ostream &os)    {   postorder(m_pRoot, os, 0);  }
    void preorder (ostream &os)    {   preorder (m_pRoot, os, 0);  }
    void print    (ostream &os)    {   print    (m_pRoot, os, 0);  }
    void inorder(void (*visit) (value_type& item))
    {   inorder(m_pRoot, visit);    }

protected:
    void inorder(Node  *pNode, ostream &os, size_t level)
    {
        // foreach(*this, fn);
        // foreach(begin(), end(), fn);
        if( pNode )
        {   Node *pParent = pNode->getParent();
            inorder(pNode->getChild(0), os, level+1);
            os << " --> " << pNode->getDataRef();
            inorder(pNode->getChild(1), os, level+1);
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
    void postorder(Node  *pNode, ostream &os, size_t level){
        //foreach(postorderbegin(), postorderend(), fn)
        if( pNode ){   
            postorder(pNode->getChild(0), os, level+1);
            postorder(pNode->getChild(1), os, level+1);
            os << " --> " << pNode->getDataRef();
        }
    void postorder(ostream &os, void (*func) (Node& node, ostream &os)){
        foreach(postbegin(),postend(), func,os);
        foreach(inbegin(1),inend(1), [](Node& node){});
    }

    // TODO: generalize this function by using iterators and apply any function
    // Create a new iterator to walk in postorder
    void preorder(Node  *pNode, ostream &os, size_t level){
        //foreach(preorderbegin(), preorderend(), fn)
        if( pNode ){   
            os << " --> " << pNode->getDataRef();
            preorder(pNode->getChild(0), os, level+1);
            preorder(pNode->getChild(1), os, level+1);            
        }
    void preorder(ostream &os, void (*func) (Node& node, ostream &os)){
        foreach(prebegin(),preend(), func,os);
        foreach(inbegin(1),inend(1), [](Node& node){});
    }

    // TODO: generalize this function by using iterators and apply any function
    void print(Node  *pNode, ostream &os, size_t level)
    {
        // foreach(begin(), end(), print);
        if( pNode ){
            Node *pParent = pNode->getParent();
            print(pNode->getChild(1), os, level+1);
            //os << string(" | ") * level << pNode->getDataRef() << "(" << (pParent?(pNode->getBranch()?"R-":"L-") + to_string(pParent->getData()):"Root") << ")" <<endl;
            os << string(" | ") * level << pNode->getDataRef() << "(" << (pParent?to_string(pParent->getData()):"Root") << ")" <<endl;
            print(pNode->getChild(0), os, level+1);
        }
    void print(ostream &os, void (*func) (Node& node, ostream &os)){
        foreach(printbegin(),printend(), func, os);
        foreach(inbegin(1),inend(1), [](Node& node){});
    }

    // TODO: generalize this function by using iterators and apply any function
    void inorder(Node  *pNode, void (*visit) (value_type& item))
    {
        if( pNode )
        {   
            inorder(pNode->getChild(0), *visit);
            (*visit)(pNode->getDataRef());
            inorder(pNode->getChild(1), *visit);
        }
    void inorder(void (*func) (Node& Node, LinkedValueType& value),LinkedValueType& value){
        foreach(inbegin(),inend(), func, value);
        foreach(inbegin(1),inend(1), [](Node& node){});
    }
};
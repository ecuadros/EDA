#ifndef __BINARY_TREE_H__  
#define __BINARY_TREE_H__ 
#include <utility>
//#include <algorithm>
#include <cassert>
#include <stack>
#include <vector>
#include <queue>
#include "types.h"
//#include "util.h"
using namespace std;

//*****************************************************************************
//*                         NodeBinaryTree                                   *
//*****************************************************************************
template <typename Traits>
class NodeBinaryTree
{
public:
    // DONE: Change T by KeyNode (aka Traits::Node aka InnerNode)
    using Type = typename Traits::value_type;
    using value_type = typename Traits::value_type;
    using KeyType = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using InnerNode = typename Traits::Node;
private:
  typedef NodeBinaryTree<Traits> Node;
  public:
    InnerNode       m_data;
    Node *  m_pParent = nullptr;
    size_t m_height = 1;
    vector<Node *> m_pChild = {nullptr, nullptr}; // 2 hijos inicializados en nullptr
  public:
    NodeBinaryTree(Node *pParent, value_type data, LinkedValueType val, Node *p0 = nullptr, Node *p1 = nullptr, size_t height = 1)
        : m_pParent(pParent), m_data(data, val), m_height(height)
    {   m_pChild[0] = p0;   m_pChild[1] = p1;   }

    // DONE: Keynode (Ahora las funciones son solo wrappers que llaman a las funciones de InnerNode (aka KeyNode))
    value_type         getData()                {   return m_data.getData();    }
    value_type        &getDataRef()             {   return m_data.getDataRef();    }
 
    // DONE: review if these functions must remain public/private
    // Se mantienen publicas porque son utilizadas desde afuera
    void      setpChild(const Node *pChild, size_t pos)  {   m_pChild[pos] = pChild;  }
    Node    * getChild(size_t branch){ return m_pChild[branch];  }
    Node    *&getChildRef(size_t branch){ return m_pChild[branch];  }
    Node    * getParent() { return m_pParent;   }
};

//*****************************************************************************
//*                               Iterators                                   *
//*****************************************************************************
// DONE: Removed Inheritance
template <typename Container>
class binary_tree_iterator
{
    _ITER_TYPEDEFS(Container, binary_tree_iterator); // DONE: llevar esta misma idea a los demas container ya existentes (iterator.h _ITERTYPEDEFS)
    _ITER_MEMBERS();

public:
    binary_tree_iterator(Container *pContainer, Node *pNode) : Parent(pContainer, pNode) {}
    binary_tree_iterator(myself &other) : Parent(other) {}
    binary_tree_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante
private:
    binary_tree_iterator operator++()
    {
        return *this;
    }
};

// DONE: Removed Inheritance
template <typename Container>
class binary_tree_iterator_preorder
{
    _ITER_TYPEDEFS(Container, binary_tree_iterator_preorder);
protected:
    Container *m_pContainer;
    Node      *m_pNode;
public:
    binary_tree_iterator_preorder(Container *pContainer, Node *pNode) : m_pContainer(pContainer), m_pNode(pNode)
    {
        if (!pNode)
            return;
        Node *node = pNode;
        m_stack.push(node);
    }
    binary_tree_iterator_preorder(myself &other) : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode) {}
    binary_tree_iterator_preorder(myself &&other)
    {
        m_pContainer = move(other.m_pContainer);
        m_pNode = move(other.m_pNode);
    }

    Type &operator*()                    { return m_pNode->getDataRef();   }

    binary_tree_iterator_preorder operator++()
    { 
        if(m_stack.empty()) return *this;
        Node* node = m_stack.top();
        m_stack.pop();
        if(!m_stack.empty()) this->m_pNode = m_stack.top();
        Node* right = node->getChild(1);
        Node* left = node->getChild(0);
        if(right) {
            m_stack.push(right);
            this->m_pNode = right;
        }
        if(left) {
            m_stack.push(left);
            this->m_pNode = left;
        }

        return *this;
    }
    bool operator==(const myself &other) const
    {
        return this->m_stack == other.m_stack;
    }
    bool operator !=(const myself &other) const
    {
        return this->m_stack != other.m_stack;
    }
private:
    stack<Node *> m_stack;
};

// DONE: Removed Inheritance
template <typename Container>
class binary_tree_iterator_postorder
{
    _ITER_TYPEDEFS(Container, binary_tree_iterator_postorder);
protected:
    Container *m_pContainer;
    Node      *m_pNode;
public:
    binary_tree_iterator_postorder(Container *pContainer, Node *pNode): m_pContainer(pContainer), m_pNode(pNode) {
        if(!pNode) return;
        stack<Node *> tmpStack;
        Node* curr = pNode;
        Node* prev = nullptr;
        bool first = true;
        while(curr || !tmpStack.empty()) {
            if(curr) {
                tmpStack.push(curr);
                curr = curr->getChild(0);
            } else {
                curr = tmpStack.top();
                Node* right = curr->getChild(1);
                if(right == nullptr || right == prev) {
                    if(first) {
                        this->m_pNode = curr;
                        first = false;
                    }
                    m_stack.push(curr);
                    tmpStack.pop();
                    prev = curr;
                    curr = nullptr;
                }
                else {
                    curr = right;
                }
            }
        }
    }
    binary_tree_iterator_postorder(myself &other) : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode) {}
    binary_tree_iterator_postorder(myself &&other)
    {
        m_pContainer = move(other.m_pContainer);
        m_pNode = move(other.m_pNode);
    }

    binary_tree_iterator_postorder operator++()
    { 
        if(m_stack.empty()) return *this;
        m_stack.pop();
        this->m_pNode = m_stack.front();

        return *this;
    }
    bool operator==(const myself &other) const
    {
        return this->m_stack == other.m_stack;
    }
    bool operator !=(const myself &other) const
    {
        return this->m_stack != other.m_stack;
    }

    Type &operator*()                    { return m_pNode->getDataRef();   }
private:
    queue<Node *> m_stack;
};

// DONE: Removed Inheritance
template<typename Container>
class binary_tree_iterator_inorder
{
    _ITER_TYPEDEFS(Container, binary_tree_iterator_inorder);
protected:
    Container *m_pContainer;
    Node      *m_pNode;
public:
    binary_tree_iterator_inorder(Container *pContainer, Node *pNode) : m_pContainer(pContainer), m_pNode(pNode){
        pushStackNodes(pNode);
        if(!this->m_stack.empty()) {
            this->m_pNode = this->m_stack.top();
            m_stack.pop();
        }
    }
    binary_tree_iterator_inorder(myself &other) : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode) {}
    binary_tree_iterator_inorder(myself &&other)
    {
        m_pContainer = move(other.m_pContainer);
        m_pNode = move(other.m_pNode);
    }

    binary_tree_iterator_inorder operator++() { 
        if(m_stack.empty()) {
            this->m_pNode = nullptr;
            return *this;
        }
        Node* node = this->m_stack.top();
        this->m_stack.pop();
        this->m_pNode = node;
        pushStackNodes(node->getChild(1));
        return *this;
    }
    void pushStackNodes(Node* node) {
        Node* tmp = node;
        while(tmp) {
            this->m_stack.push(tmp);
            tmp = tmp->getChild(0);
        }
    }
    bool operator==(const myself &other) const
    {
        return this->m_stack == other.m_stack;
    }
    bool operator !=(const myself &other) const
    {
        return this->m_pNode != other.m_pNode;
    }
    
    Type &operator*()                    { return m_pNode->getDataRef();   }
private:
    stack<Node *> m_stack;
};

template<typename Node>
struct NodeInfo
{
    Node* first;
    size_t second;
    NodeInfo(Node* node, size_t level) : first(node), second(level) {}
};


// DONE: Removed Inheritance
template<typename Container>
class binary_tree_forward_iterator
{
    _ITER_TYPEDEFS(Container, binary_tree_forward_iterator);
protected:
    Container *m_pContainer;
    NodeInfo<Node>      *m_pNode;
public:
    binary_tree_forward_iterator(Container *pContainer, NodeInfo<Node> *pNode) : m_pContainer(pContainer), m_pNode(pNode){
        // cout<<"["<<m_pNode->first->getData()<<"]"<<endl;
        if(!pNode) return;
        pushStackNodes(pNode);
        if(!m_stack.empty()) {
            this->m_pNode = m_stack.top();
            m_stack.pop();
        }
    }
    binary_tree_forward_iterator(myself &other) : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode) {}
    binary_tree_forward_iterator(myself &&other)
    {
        m_pContainer = move(other.m_pContainer);
        m_pNode = move(other.m_pNode);
    }
    binary_tree_forward_iterator operator++() { 
        if(m_stack.empty()) {
            this->m_pNode = nullptr;
            return *this;
        }
        NodeInfo<Node>* node = m_stack.top();
        m_stack.pop();
        this->m_pNode = node;
        // cout<<"["<<node->getData()<<"]"<<endl;
        pushStackNodes(new NodeInfo<Node>( node->first->getChild(0), node->second + 1 ));
        return *this;
    }
    void pushStackNodes(NodeInfo<Node>* node) {
        NodeInfo<Node>* tmp = node;
        while(tmp && tmp->first) {
            // if(node) {
            //     cout<<tmp->first->getData() << tmp->second <<endl;
            // }
            m_stack.push(tmp);
            tmp = new NodeInfo<Node>(tmp->first->getChild(1), tmp->second + 1);
        }
    }
    bool operator==(const myself &other) const
    {
        return this->m_stack == other.m_stack;
    }
    bool operator !=(const myself &other) const
    {
        return this->m_pNode != other.m_pNode;
    }
    NodeInfo<Node>* &operator*()                    { return m_pNode; }
private:
    stack<NodeInfo<Node> *> m_stack;
};


//*****************************************************************************
//*                                  Traits                                  *
//*****************************************************************************
template <typename _T>
struct BinaryTreeAscTraits
{
    using  T = _T;
    using  Node      = NodeBinaryTree<T>;
    using  CompareFn = less<T>;
};

template <typename _T>
struct BinaryTreeDescTraits
{
    using  T         = _T;
    using  Node      = NodeBinaryTree<T>;
    using  CompareFn = greater<T>;
};


//*****************************************************************************
//*                               BinaryTree                                  *
//*****************************************************************************
template <typename Traits>
class BinaryTree
{
  public:
    typedef typename Traits::value_type         value_type;
    // typedef typename NodeBinaryTree<Traits>     Node;
    using Node = NodeBinaryTree<Traits>;
    typedef typename Traits::LinkedValueType LinkedValueType;
    
    typedef typename Traits::CompareFn      CompareFn;
    typedef BinaryTree<Traits>              myself;
    typedef binary_tree_iterator<myself>    iterator;

protected:
    Node    *m_pRoot = nullptr;
    size_t   m_size  = 0;
    CompareFn Compfn;
public: 
    BinaryTree() : m_pRoot(nullptr), m_size(0) {}
    size_t  size()  const       { return m_size;       }
    bool    empty() const       { return size() == 0;  }
    // DONE: insert must receive two paramaters: elem and LinkedValueType value
    virtual void    insert(value_type &elem, const LinkedValueType &value) { 
        Node* n = internal_insert(elem, value, nullptr, m_pRoot);  
    }
    binary_tree_iterator_postorder <myself> postorderbegin() { return binary_tree_iterator_postorder<myself>(this, m_pRoot); }
    binary_tree_iterator_postorder <myself> postorderend()   { return binary_tree_iterator_postorder<myself>(this, nullptr); }
    binary_tree_iterator_preorder <myself> preorderbegin() { return binary_tree_iterator_preorder<myself>(this, m_pRoot); }
    binary_tree_iterator_preorder <myself> preorderend()   { return binary_tree_iterator_preorder<myself>(this, nullptr); }
    binary_tree_iterator_inorder <myself> inorderbegin() { return binary_tree_iterator_inorder<myself>(this, m_pRoot); }
    binary_tree_iterator_inorder <myself> inorderend()   { return binary_tree_iterator_inorder<myself>(this, nullptr); }
    binary_tree_forward_iterator <myself> forwardbegin() { 
        NodeInfo<Node>* p = new NodeInfo<Node>(m_pRoot, 0);
        auto it = binary_tree_forward_iterator<myself>(this, p); 
        return it;
    }
    binary_tree_forward_iterator <myself> forwardend()   { return binary_tree_forward_iterator<myself>(this, nullptr); }
protected:
    Node *CreateNode(Node *pParent, value_type &elem, const LinkedValueType &val){ 
        return new Node(pParent, elem, val); 
    }
    Node *internal_insert(value_type &elem, const LinkedValueType &value, Node *pParent, Node *&rpOrigin)
    {
        if( !rpOrigin ) //  llegué al fondo de una rama
        {   ++m_size;
            // cout<<level;
            rpOrigin = CreateNode(pParent, elem, value);
            rpOrigin->m_height = 1;
            rpOrigin = balance(rpOrigin);
            return rpOrigin;
        }
        size_t branch = Compfn(elem, rpOrigin->getDataRef() );
        Node* ret = internal_insert(elem, value, rpOrigin, rpOrigin->getChildRef(branch));
        rpOrigin->m_height = max(rpOrigin->getChildRef(0)?rpOrigin->getChildRef(0)->m_height:0, rpOrigin->getChildRef(1)?rpOrigin->getChildRef(1)->m_height:0) + 1;
        rpOrigin = balance(rpOrigin);
        return ret;
    }
    // virtual Node* balance(Node* tree);
    virtual Node* balance(Node* tree) { return tree; };
public:
    // DONE: generalize this function by using iterators and apply any function
    template<typename F>
    void postorder(F func) {
        foreach(postorderbegin(), postorderend(), func);
    }

    // DONE: generalize this function by using iterators and apply any function
    template<typename F>
    void preorder(F func){
        foreach(preorderbegin(), preorderend(), func);
    }
    
    // DONE: generalize this function by using iterators and apply any function
    template<typename F>
    void inorder(F func) {
        foreach(inorderbegin(), inorderend(), func);
    }

    template<typename F>
    void forward(F func) {
        foreach(forwardbegin(), forwardend(), func);
    }

    // DONE: generalize this function by using iterators and apply any function
    // Ahora se tiene el forward iterator, que es la generalizacion de la 
    // funcion print, es mas, la funcion print solo hace uso del iterator,
    // pero se puede aplicar el iterator con cualquier funcion
    void print(ostream &os)
    {
        // Ahora se hace con un foreach
        foreach(forwardbegin(), forwardend(), [&os](NodeInfo<Node>* pair) {
            Node* pNode = pair->first;
            size_t level = pair->second;
            if( pNode ){
                Node *pParent = pNode->getParent();
                for(size_t i = 0; i < level; i++)
                    os << " | ";
                os << pNode->getDataRef() << "(" << (pParent?to_string(pParent->getData()):"Root") << ")" << "[" << pNode->m_height << "]" <<endl;
            }
        });
    }

protected:

    void read(istream &is)
    {
        using value_type = typename BinaryTree<Traits>::value_type;
        using LinkedValueType = typename BinaryTree<Traits>::LinkedValueType;
        value_type key;
        LinkedValueType value;

        is>>key;
        is>>value;
        insert(key, value);
    }

    template<typename T>
    friend ostream &operator<<(ostream &os, BinaryTree<T> &obj);

    template<typename T>
    friend istream &operator>>(istream &is, BinaryTree<T> &obj);
};

template<typename Traits>
ostream& operator<<(ostream& os, BinaryTree<Traits>& obj)
{
    obj.print(os);
    return os;
}

template<typename Traits>
istream& operator>>(istream& is, BinaryTree<Traits>& obj)
{
    obj.read(is);
    return is;
}

#endif
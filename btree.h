#ifndef __BTREE_H__
#define __BTREE_H__

#include <iostream>
#include "btreepage.h"
#define DEFAULT_BTREE_ORDER 3

const size_t MaxHeight = 5; 

//TODO: Tarea 36: BTree: forward iterator
template <typename Container>
class forward_iterator
{
       public: 
              typedef typename Container::Node Node;
              typedef typename Node::value_type Type;
              typedef forward_iterator<Container> myself;
       protected:
              Container *m_pContainer;
              Node *m_pNode;
       public:
              forward_iterator(Container *pContainer, Node *pNode) : m_pContainer(pContainer), m_pNode(pNode) {}
              forward_iterator(myself &other)  : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode) {}
              forward_iterator(myself &&other) {   
                     m_pContainer = move(other.m_pContainer);
                     m_pNode      = move(other.m_pNode);
              }
       public:
              myself operator++() 
              {
                     return *this;
              }

              forward_iterator operator=(forward_iterator &iter)
              {   
                     m_pContainer = move(iter.m_pContainer);
                     m_pNode      = move(iter.m_pNode);
                     return *(forward_iterator *)this;
              }

              bool operator==(forward_iterator iter)   { return m_pNode == iter.m_pNode; }
              bool operator!=(forward_iterator iter)   { return !(*this == iter); }
              Node *operator*() { return m_pNode;}
};

// TODO: Tarea 37: BTree: backward iterator
template <typename Container>
class backward_iterator
{
       public:
              typedef typename Container::Node Node;
              typedef typename Node::value_type Type;
              typedef backward_iterator<Container> myself;
       protected:
              Container *m_pContainer;
              Node *m_pNode;
       public:
              backward_iterator(Container *pContainer, Node *pNode) : m_pContainer(pContainer), m_pNode(pNode) {}
              backward_iterator(myself &other) : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode) {}
              backward_iterator(myself &&other) {   
                     m_pContainer = move(other.m_pContainer);
                     m_pNode      = move(other.m_pNode);
              }
       public:
       myself operator++()
       {
              m_pNode = m_pNode->getpPrev();
              return *this;
       }

       backward_iterator operator=(backward_iterator &iter)
       {
              m_pContainer = move(iter.m_pContainer);
              m_pNode      = move(iter.m_pNode);
              return *(backward_iterator *)this;
       }

       bool operator==(backward_iterator iter)   { return m_pNode == iter.m_pNode; }
       bool operator!=(backward_iterator iter)   { return !(*this == iter);        }
       Node *operator*() { return m_pNode;}
};

template <typename _T, typename _V>
struct BTreeTrait
{
       using T = _T;
       using LinkedValueType = _V;
       using Node = CBTreePage<_T, _V>;
       using CompareFn = std::greater<CBTreePage<_T, _V>>;
};

template <typename Traits>
class BTree // this is the full version of the BTree
{
public:
       typedef typename Traits::T value_type;
       typedef typename Traits::Node Node;
       typedef typename Traits::LinkedValueType LinkedValueType;
       typedef typename Traits::CompareFn CompareFn; 
       typedef BTree<Traits> myself;
       typedef forward_iterator<myself> iterator;
public:
       BTree(size_t order = DEFAULT_BTREE_ORDER, bool unique = true)
              : m_Order(order),
                m_Root(2 * order  + 1, unique),
                m_Unique(unique),
                m_NumKeys(0)
       {
              m_Root.SetMaxKeysForChilds(order);
              m_Height = 1;
       }
       ~BTree() {}
       bool Insert (const value_type key, const LinkedValueType value);
       bool Remove (const value_type key, const LinkedValueType value);

       iterator begin() { iterator iter(this, m_Root);    return iter;    }
       iterator end()   { iterator iter(this, nullptr);    return iter;    }

       LinkedValueType Search (const value_type key)
       {      LinkedValueType value = -1;
              m_Root.Search(key, value, m_compareFn);
              return value;
       }

       size_t            size()  { return m_NumKeys; }
       size_t            height() { return m_Height;      }
       size_t            GetOrder() { return m_Order;     }

       void Write (ostream &os)
       {
              m_Root.Print(os);
       }

       //TODO: Tarea 33: BTree: generalizar foreach
       template<typename Callable, typename... Args>
       void ForEach( Callable fn, Args&& ...args)
       {
              m_Root.ForEach(fn, 0, args...);
              //ForEach(&::Print<T, V>, 0, &os);
       }

       template<typename Callable, typename... Args>
       LinkedValueType* FirstThat( Callable fn, Args&& ...args)
       {
              return m_Root.FirstThat(fn, 0, args...);
       }

       void Read(istream &is)
       {
              int filas;
              is >> filas;
              value_type key;
              LinkedValueType value;
              string puntuacion;
              while (filas--)
              {
                     is >> key;
                     is >> puntuacion;
                     is >> value;
                     this->Insert(key, value);
              }
       }

protected:
       Node m_Root;
       size_t m_Height;  // height of tree
       size_t m_Order;   // order of tree
       size_t m_NumKeys; // number of keys
       bool m_Unique;  // Accept the elements only once ?
       CompareFn m_compareFn;
};     

// TODO change ObjID by LinkedValueType value
template <typename Traits>
bool BTree<Traits>::Insert(const value_type key, const LinkedValueType value){
       bt_ErrorCode error = m_Root.Insert(key, value, m_compareFn);
       if( error == bt_duplicate )
               return false;
       m_NumKeys++;
       if( error == bt_overflow )
       {
               m_Root.SplitRoot();
               m_Height++;
       }
       return true;
}

template <typename Traits>
bool BTree<Traits>::Remove (const value_type key, const LinkedValueType value)
{
       bt_ErrorCode error = m_Root.Remove(key, value, m_compareFn);
       if( error == bt_duplicate || error == bt_nofound )
               return false;
       m_NumKeys--;

       if( error == bt_rootmerged )
               m_Height--;
       return true;
}

//TODO: Tarea 35: BTree: write
template <typename Traits>
ostream &operator<<(ostream &os, BTree<Traits> &bt)
{
       bt.Write(os);
       return os;
}

//TODO: Tarea 34: BTree: read
template <typename Traits>
istream &operator>>(istream &is, BTree<Traits> &bt)
{
       bt.Read(is);
       return is;
}

#endif
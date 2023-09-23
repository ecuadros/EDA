#ifndef __BTREE_H__
#define __BTREE_H__

#include <iostream>
#include "btreepage.h"
#define DEFAULT_BTREE_ORDER 3

const size_t MaxHeight = 5; 

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
       typedef typename Traits::T value_type;
       typedef typename Traits::Node Node;
       typedef typename Traits::LinkedValueType LinkedValueType;
       typedef typename Traits::CompareFn CompareFn;

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

       LinkedValueType Search (const value_type key)
       {      LinkedValueType value = -1;
              m_Root.Search(key, value, m_compareFn);
              return value;
       }

       size_t            size()  { return m_NumKeys; }
       size_t            height() { return m_Height;      }
       size_t            GetOrder() { return m_Order;     }

       void Print (ostream &os)
       {
              m_Root.Print(os);
       }

       //TODO: Tarea 33: BTree: generalizar foreach
       template<typename Callable, typename... Args>
       void ForEach( Callable fn, Args&& ...args)
       {
              m_Root.ForEach(fn, 0, args...);
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
       bt.Print(os);
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
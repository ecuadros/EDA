#ifndef __BTREE_H__
#define __BTREE_H__

#include <iostream>
#include "btreepage.h"
#define DEFAULT_BTREE_ORDER 3

const size_t MaxHeight = 5; 

template <typename _value_type, typename _LinkedValueType>
struct BTreeTrait
{
       using value_type = _value_type;
       using LinkedValueType = _LinkedValueType;
};

template <typename Trait>
class BTree // this is the full version of the BTree
{
       typedef typename Trait::value_type    value_type;
       typedef typename Trait::LinkedValueType    LinkedValueType;
       
       typedef CBTreePage <Trait> BTNode;// useful shorthand

public:
       //typedef ObjectInfo iterator;
       // DONE replace thius functions by foreach
       // typedef typename BTNode::lpfnForEach2    lpfnForEach2;
       // typedef typename BTNode::lpfnForEach3    lpfnForEach3;
       // typedef typename BTNode::lpfnFirstThat2  lpfnFirstThat2;
       // typedef typename BTNode::lpfnFirstThat3  lpfnFirstThat3;

       typedef typename BTNode::ObjectInfo      ObjectInfo;

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
       //int           Open (char * name, int mode);
       //int           Create (char * name, int mode);
       //int           Close ();
       bool            Insert (const value_type key, const LinkedValueType ObjID);
       bool            Remove (const value_type key, const LinkedValueType ObjID);
       LinkedValueType       Search (const value_type key)
       {      LinkedValueType ObjID = -1;
              m_Root.Search(key, ObjID);
              return ObjID;
       }
       size_t            size()  { return m_NumKeys; }
       size_t            height() { return m_Height;      }
       size_t            GetOrder() { return m_Order;     }

       void            Print (ostream &os)
       {               
              m_Root.Print(os);                              
       }
       // void            ForEach( lpfnForEach2 lpfn, void *pExtra1 )
       // {               m_Root.ForEach(lpfn, 0, pExtra1);              }
       // void            ForEach( lpfnForEach3 lpfn, void *pExtra1, void *pExtra2)
       // {               m_Root.ForEach(lpfn, 0, pExtra1, pExtra2);     }
       // ObjectInfo*     FirstThat( lpfnFirstThat2 lpfn, void *pExtra1 )
       // {               return m_Root.FirstThat(lpfn, 0, pExtra1);     }
       // ObjectInfo*     FirstThat( lpfnFirstThat3 lpfn, void *pExtra1, void *pExtra2)
       // {               return m_Root.FirstThat(lpfn, 0, pExtra1, pExtra2);   }
       typedef               ObjectInfo iterator;

       template <typename Callable>
        void            ForEach(Callable f)
        {
              m_Root.ForEach(f);
        }

        template <typename Callable, typename... Args>
        ObjectInfo* FirstThat(Callable f, Args&& ...args) {
                return m_Root.FirstThat(f, args...);
        }

       BTNode          m_Root;
protected:
       size_t          m_Height;  // height of tree
       size_t          m_Order;   // order of tree
       size_t          m_NumKeys; // number of keys
       bool            m_Unique;  // Accept the elements only once ?

public:
       template <typename _Trait>
       friend ostream &operator<<(ostream &os, BTree<_Trait> &obj);

       template <typename _Trait>
       friend istream &operator>>(istream &is, BTree<_Trait> &obj);
};     

// DONE: change ObjID by LinkedValueType value
template <typename Trait>
bool BTree<Trait>::Insert(typename Trait::value_type key, typename Trait::LinkedValueType ObjID){
       bt_ErrorCode error = m_Root.Insert(key, ObjID);
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

template <typename Trait>
bool BTree<Trait>::Remove (typename Trait::value_type key, typename Trait::LinkedValueType ObjID)
{
       bt_ErrorCode error = m_Root.Remove(key, ObjID);
       if( error == bt_duplicate || error == bt_nofound )
               return false;
       m_NumKeys--;

       if( error == bt_rootmerged )
               m_Height--;
       return true;
}

// DONE: Add operator<<
template <typename Trait>
ostream &operator<<(ostream &os, BTree<Trait> &obj)
{
       obj.Print(os);
       return os;
}

// DONE: Add operator>>
template <typename Trait>
istream &operator>>(istream &is, BTree<Trait> &obj)
{
       // DONE: Read from stream
       using value_type = typename BTree<Trait>::value_type;
       using LinkedValueType = typename BTree<Trait>::LinkedValueType;
       value_type key;
       LinkedValueType val;
       is >> key >> val;
       obj.Insert(key, val);
       return is;
}

#endif
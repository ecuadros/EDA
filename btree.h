#ifndef __BTREE_H__
#define __BTREE_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>
#include "btreepage.h"
#include "btree_iterators.h"
#define DEFAULT_BTREE_ORDER 3

const size_t MaxHeight = 5;

template <typename _keyType, typename _ObjIDType>
struct BTreeTrait
{
       using keyType = _keyType;
       using ObjIDType = _ObjIDType;
};

template <typename Trait>
class BTree // this is the full version of the BTree
{
       typedef typename Trait::keyType keyType;
       typedef typename Trait::ObjIDType ObjIDType;

       typedef CBTreePage<Trait> BTNode; // useful shorthand

       typedef BTree<Trait> myself;
       typedef forward_iterator<myself> iterator;

public:
       typedef typename BTNode::ObjectInfo ObjectInfo;

public:
       BTree(size_t order = DEFAULT_BTREE_ORDER, bool unique = true)
           : m_Order(order),
             m_Root(2 * order + 1, unique),
             m_Unique(unique),
             m_NumKeys(0)
       {
              m_Root.SetMaxKeysForChilds(order);
              m_Height = 1;
       }
       ~BTree() {}
       // int           Open (char * name, int mode);
       // int           Create (char * name, int mode);
       // int           Close ();
       bool Insert(const keyType key, const long ObjID);
       bool Remove(const keyType key, const long ObjID);
       ObjIDType Search(const keyType key)
       {
              ObjIDType ObjID = -1;
              m_Root.Search(key, ObjID);
              return ObjID;
       }
       size_t size() { return m_NumKeys; }
       size_t height() { return m_Height; }
       size_t GetOrder() { return m_Order; }

       void Print(ostream &os)
       {
              m_Root.Print(os);
       }

       void print(void (*func)(BTNode &node, ostream &os), ostream &os)
       {
              foreach (begin(), end(), func, os)
                     ;
       }

       iterator begin()
       {
              iterator iter(this, m_Root);
              return iter;
       }
       iterator end()
       {
              iterator iter(this, nullptr);
              return iter;
       }

       template <typename lpfnForEachX, typename... Args>
       void ForEach(lpfnForEachX lpfn, Args... args)
       {
              m_Root.ForEach(lpfn, 0, args...);
       }

       // typedef               ObjectInfo iterator;
       void read(istream &is)
       {
              string str;
              string separador;
              float key;
              long value;
              getline(is, str); // skip the first line
              while (getline(is, str))
              {
                     istringstream iss(str);
                     iss >> key >> separador >> value;
                     Insert(key, value);
              }
       }

protected:
       BTNode m_Root;
       size_t m_Height;  // height of tree
       size_t m_Order;   // order of tree
       size_t m_NumKeys; // number of keys
       bool m_Unique;    // Accept the elements only once ?
       mutex mutex_;
};

// TODO change ObjID by LinkedValueType value
template <typename Trait>
bool BTree<Trait>::Insert(const keyType key, const long ObjID)
{
       lock_guard<mutex> lock(mutex_);
       bt_ErrorCode error = m_Root.Insert(key, ObjID);
       if (error == bt_duplicate)
              return false;
       m_NumKeys++;
       if (error == bt_overflow)
       {
              m_Root.SplitRoot();
              m_Height++;
       }
       return true;
}

template <typename Trait>
bool BTree<Trait>::Remove(const keyType key, const long ObjID)
{
       bt_ErrorCode error = m_Root.Remove(key, ObjID);
       if (error == bt_duplicate || error == bt_nofound)
              return false;
       m_NumKeys--;

       if (error == bt_rootmerged)
              m_Height--;
       return true;
}

// TODO Add operator<<
template <typename T>
inline ostream &operator<<(ostream &os, BTree<T> &obj)
{
       obj.Print(os);
       return os;
}

// TODO Add operator>>
template <typename T>
inline istream &operator>>(istream &is, BTree<T> &obj)
{
       obj.read(is);
       return is;
}

#endif
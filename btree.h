#ifndef __BTREE_H__
#define __BTREE_H__

#include <iostream>
#include "types.h"
#include "btreepage.h"
#include "xtrait.h"

#define DEFAULT_BTREE_ORDER 3

const size_t MaxHeight = 5; 
// =====================  Traits para BTree ============================
template <typename xTrait>

struct BTreeTrait
{
       using value_type      = typename xTrait::value_type;;
       using LinkedValueType   = typename xTrait::LinkedValueType;
       using BNode                   = CBTreePage<xTrait>;
};

using bTreeIntInt = BTreeTrait<XTrait<IX,IX>>;
//=======================  clase BTree ===============================
template <typename BTrait>

class BTree // this is the full version of the BTree

{     
       using value_type            = typename BTrait::value_type;
       using LinkedValueType         = typename BTrait::LinkedValueType;
       using BTNode                       = typename BTrait::BNode;
       
public:
  
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
       
       bool               Insert (const value_type key, const  LinkedValueType value);
       bool               Remove (const value_type key, const LinkedValueType value);
       
       LinkedValueType      Search (const value_type key) {      
     
          LinkedValueType value = -1;
              m_Root.Search(key, value);
              return value;
       }
       
       size_t            size()  { return m_NumKeys; }
       size_t            height() { return m_Height;      }
       size_t            GetOrder() { return m_Order;     }
       
    // funcion print (write)
       void Print (ostream &os) {  m_Root.Print(os);  }
       
       //------ funcion read ( lee desde un archivo)------
    
       void  Read (istream &is){
       assert(is);
     
       string line;
       while(getline(is, line)){
              istringstream iss(line);
              value_type key;
              LinkedValueType value;
              if(iss>>key>>value){
                     Insert(key,value);
              }
              }
       
       }     
       //==============  Forward y Backward iterator=======================

        auto begin(){return m_Root.begin();}
        auto end(){return m_Root.end();}
        auto rbegin(){return m_Root.rbegin();}
        auto rend(){return m_Root.rend();}

       // ========= Generalización del ForEach y FirstThat ================

  template <typename Callable, typename... Args>
  void ForEach(Callable lpfn, Args&&... args) {
      m_Root.ForEach(lpfn, std::forward<Args>(args)...);
  }

  template <typename Callable, typename... Args>
  void ForEachReverse(Callable lpfn, Args&&... args) {
      m_Root.ForEach(lpfn, std::forward<Args>(args)...);
  }


  template <typename Callable, typename... Args>
  ObjectInfo* FirstThat(Callable lpfn, Args&&... args) {
      return m_Root.FirstThat(lpfn, std::forward<Args>(args)...);
  }
     
protected:
  
       BTNode          m_Root;
       size_t          m_Height;  // height of tree
       size_t          m_Order;   // order of tree
       size_t          m_NumKeys; // number of keys
       bool            m_Unique;  // Accept the elements only once ?
};     

/// Hecho: change ObjID by LinkedValueType value

template <typename Trait>

bool BTree<Trait>::Insert(const value_type key, const LinkedValueType value){
  
       bt_ErrorCode error = m_Root.Insert(key, value);
       
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
bool BTree<Trait>::Remove (const value_type key, const LinkedValueType value)
{
       bt_ErrorCode error = m_Root.Remove(key, value);
       if( error == bt_duplicate || error == bt_nofound )
               return false;
       m_NumKeys--;

       if( error == bt_rootmerged )
               m_Height--;
       return true;
}

// Se adicionó el operador <<
template <typename T>
ostream &operator<<(ostream &os, BTree<T> &Ob)
{
  Ob.Print(os);
  return os;
}
// Se adicionó el operador >> 

template <typename T>
istream &operator>>(istream &is, BTree<T> &Ob)
{
  Ob.Read(is);
  return is;
}

#endif
#ifndef __CBTreePage_H__
#define __CBTreePage_H__

#include <iostream>
#include <vector>
#include <assert.h>
#include <functional>
#include "keynode.h"

// TODO: #1 Crear una function para agregarla al demo.cpp ( no trivial )
// TODO: #2 Agregarle un Trait (prueba git) ( no trivial )
// TODO: #3 crear un iterator ( no trivial )
//       Sugerencia: Tarea1 cada pagina debe tener un puntero al padre primero ( no trivial )
// TODO: #4 integrarlo al recorrer ( no trivial )


template <typename Trait>
class BTree;

using namespace std;
enum bt_ErrorCode {bt_ok, bt_overflow, bt_underflow, bt_duplicate, bt_nofound, bt_rootmerged};

template <typename Container, typename ObjType, typename CompareFn>
size_t binary_search(Container& container, size_t first, size_t last, ObjType &object, CompareFn compareFn)
{
       if( first >= last )
               return first;
       while( first < last )
       {
               size_t mid = (first+last)/2;
               if( object == (ObjType)container[mid ] )
                       return mid;
               if( compareFn( object, (ObjType)container[mid ]) )
                       first = mid+1;
               else
                       last  = mid;
       }
       if( !compareFn(object, (ObjType)container[first]) )
               return first;
       return last;
}

// Error al poner size_t
// Posible motivo: El i está disminuyendo
template <typename Container, typename ObjType>
void insert_at(Container& container, ObjType object, int pos)
{
        // TODO: #5 replace int, long by types such as size_t
       size_t size = container.size();
       for(int i = size-2 ; i >= pos ; i--)
               container[i+1] = container[i];
       container[pos] =  object;	
}

template <typename Container>
void remove(Container& container, size_t pos)
{
       size_t size = container.size();
       for(auto i = pos+1 ; i < size ; i++)
           container[i-1] = container[i];
}

template <typename value_type, typename LinkedValueType>
struct tagObjectInfo
{
  value_type key;
  LinkedValueType value;
  size_t UseCounter;
  tagObjectInfo(const value_type &_key, LinkedValueType _value)
      : key(_key), value(_value), UseCounter(0) {}
  tagObjectInfo(const tagObjectInfo &objInfo)
      : key(objInfo.key), value(objInfo.value), UseCounter(0) {}
  tagObjectInfo() {}
  operator value_type() { return key; }
  size_t GetUseCounter() { return UseCounter; }
};

template <typename T, typename V>
class CBTreePage : public KeyNode<T, V>
{
public:
  typedef T value_type;
  typedef T Type;
  typedef V LinkedValueType;
  using Parent = class KeyNode<T, V>;
private:
  typedef CBTreePage<T, V> BTPage;

public:
  typedef tagObjectInfo<value_type, LinkedValueType> ObjectInfo;
 public:
       CBTreePage(size_t maxKeys, bool unique = true);
       virtual ~CBTreePage();
       template <typename CompareFn>
       bt_ErrorCode    Insert (const value_type &key, const LinkedValueType value, CompareFn compareFn);
       template <typename CompareFn>
       bt_ErrorCode    Remove (const value_type &key, const LinkedValueType value, CompareFn compareFn);
       template <typename CompareFn>
       bool            Search (const value_type &key, LinkedValueType &value, CompareFn compareFn);
       void            Print  (ostream &os);
       void            Read   (istream &is);
       template <typename Callable, typename Iter, typename... Args>
       void ForEach(Callable fn, Iter level, Args&&... args);
       template <typename Callable, typename Iter, typename... Args>
       void ForEachReverse(Callable fn, Iter level, Args&&... args);
       template <typename Callable, typename Iter, typename... Args>
       ObjectInfo* FirstThat(Callable fn, Iter level, Args&&... args);

protected:
       size_t  m_MinKeys; // minimum number of keys in a node
       size_t  m_MaxKeys; // maximum number of keys in a node
       size_t m_MaxKeysForChilds; // just to distinguish the root
       bool m_Unique;
       bool m_isRoot;
       //size_t           NextNode; // address of next node at same level
       //size_t RecAddr; // address of this node in the BTree file
       vector<ObjectInfo> m_Keys;
       vector<BTPage *>m_SubPages;
       
       size_t  m_KeyCount;
public:
       void  Create();
       void  Reset ();
       void  Destroy () {   Reset(); delete this;}
       void  clear ();

       bool  Redistribute1   (size_t &pos);
       bool  Redistribute2   (size_t pos);
       void  RedistributeR2L (size_t pos);
       void  RedistributeL2R (size_t pos);

       bool    TreatUnderflow  (size_t &pos)
       {       return Redistribute1(pos) || Redistribute2(pos);}

       bt_ErrorCode    Merge  (size_t pos);
       bt_ErrorCode    MergeRoot ();
       void  SplitChild (size_t pos);

       ObjectInfo &GetFirstObjectInfo();

       bool Overflow()  { return m_KeyCount > m_MaxKeys; }
       bool Underflow() { return m_KeyCount < MinNumberOfKeys(); }
       bool IsFull()    { return m_KeyCount >= m_MaxKeys; }


       size_t  MinNumberOfKeys()  { return 2*m_MaxKeys/3.0; }
       size_t  GetFreeCells()  { return m_MaxKeys - m_KeyCount; }
       size_t& NumberOfKeys()  { return m_KeyCount; }
       size_t  GetNumberOfKeys()  { return m_KeyCount; }
       bool IsRoot()  { return m_MaxKeysForChilds != m_MaxKeys; }
       void SetMaxKeysForChilds(size_t orderforchilds)
       {        m_MaxKeysForChilds = orderforchilds;       }
       size_t GetFreeCellsOnLeft(size_t pos)
       {        if( pos > 0 )                                   // there is some page on left ?
                        return m_SubPages[pos-1]->GetFreeCells();
                return 0;
       }
       size_t GetFreeCellsOnRight(size_t pos)
       {    if( pos < GetNumberOfKeys() )   // there is some page on right ?
                return m_SubPages[pos+1]->GetFreeCells();
            return 0;
       }

public:
       bool SplitRoot();
       void SplitPageInto3(vector<ObjectInfo>   & tmpKeys,
                                               vector<BTPage *>  & SubPages,
                                               BTPage           *& pChild1,
                                               BTPage           *& pChild2,
                                               BTPage           *& pChild3,
                                               ObjectInfo        & oi1,
                                               ObjectInfo        & oi2);
       void MovePage(BTPage *  pChildPage,vector<ObjectInfo> & tmpKeys,vector<BTPage *> & tmpSubPages);
};

template <typename value_type, typename LinkedValueType>
CBTreePage<value_type, LinkedValueType>::CBTreePage(size_t maxKeys, bool unique)
                               : m_MaxKeys(maxKeys), m_Unique(unique), m_KeyCount(0)
{
       Create();
       SetMaxKeysForChilds(m_MaxKeys);
}

template <typename value_type, typename LinkedValueType>
CBTreePage<value_type, LinkedValueType>::~CBTreePage()
{
       Reset();
}

template <typename value_type, typename LinkedValueType>
template <typename CompareFn>
bt_ErrorCode CBTreePage<value_type, LinkedValueType>::Insert(const value_type& key, const LinkedValueType value, CompareFn compareFn)
{
       size_t pos = binary_search(m_Keys, 0, m_KeyCount, key, compareFn);
       bt_ErrorCode error = bt_ok;

       if( pos < m_KeyCount && (value_type)m_Keys[pos] == key && m_Unique)
               return bt_duplicate; // this key is duplicate

       if( !m_SubPages[pos] ) // this is a leave
       {
               ::insert_at(m_Keys, ObjectInfo(key, value), pos);
               m_KeyCount++;
               if( Overflow() )
                       return bt_overflow;
               return bt_ok;
       }
       else
       {
               // recursive insertion
               error = m_SubPages[pos]->Insert(key, value, compareFn);
               if( error == bt_overflow )
               {
                       if( !Redistribute1(pos) )
                               SplitChild(pos);
                       if( Overflow() )          // Propagate overflow
                               return bt_overflow;
                       return bt_ok;
               }
       }
       if( Overflow() ) // node overflow
               return bt_overflow;
       return bt_ok;
}

template <typename value_type, typename LinkedValueType>
bool CBTreePage<value_type, LinkedValueType>::Redistribute1(size_t &pos)
{
       if( m_SubPages[pos]->Underflow() )
       {
               size_t NumberOfKeyOnLeft = 0,
                       NumberOfKeyOnRight = 0;
               // is this the first element or there are more elements on right brother
               if( pos > 0 )
                       NumberOfKeyOnLeft = m_SubPages[pos-1]->NumberOfKeys();
               if( pos < NumberOfKeys() )
                       NumberOfKeyOnRight = m_SubPages[pos+1]->NumberOfKeys();

               if( NumberOfKeyOnLeft > NumberOfKeyOnRight )
                       if( m_SubPages[pos-1]->NumberOfKeys() > m_SubPages[pos-1]->MinNumberOfKeys() )
                               RedistributeL2R(pos-1); // bring elements from left brother
                       else
                               if( pos == NumberOfKeys() )
                                       return (--pos, false);
                               else
                                       return false;
               else //NumberOfKeyOnLeft < NumberOfKeyOnRight )
                       if( m_SubPages[pos+1]->NumberOfKeys() > m_SubPages[pos+1]->MinNumberOfKeys() )
                               RedistributeR2L(pos+1); // bring elements from right brother
                       else
                               if( pos == 0 )
                                       return (++pos, false);
                               else
                                       return false;
       }
       else // it is due to overflow
       {
               size_t FreeCellsOnLeft = GetFreeCellsOnLeft(pos),   // Free Cells On Left
               fcor = GetFreeCellsOnRight(pos);  // Free Cells On Right

               if( !FreeCellsOnLeft && !fcor && m_SubPages[pos]->IsFull() )
                       return false;
               if( FreeCellsOnLeft > fcor ) // There is more space on left
                       RedistributeR2L(pos);
               else
                       RedistributeL2R(pos);

       }
       return true;
}

// Redistribute2 function
// it considers two brothers m_SubPages[pos-1] && m_SubPages[pos+1]
// if it fails the only way is merge !
template <typename value_type, typename LinkedValueType>
bool CBTreePage<value_type, LinkedValueType>::Redistribute2(size_t pos)
{
       assert( pos > 0 && pos < NumberOfKeys()  );
       assert( m_SubPages[pos-1] != 0 && m_SubPages[pos] != 0 && m_SubPages[pos+1] != 0 );
       assert( m_SubPages[pos-1]->Underflow() ||
                       m_SubPages[ pos ]->Underflow() ||
                       m_SubPages[pos+1]->Underflow() );

       if( m_SubPages[pos-1]->Underflow() )
       {       // Rotate R2L
               RedistributeR2L(pos+1);
               RedistributeR2L(pos);
               if( m_SubPages[pos-1]->Underflow() )
                       return false;
       }
       else if( m_SubPages[pos+1]->Underflow() )
       {       // Rotate L2R
               RedistributeL2R(pos-1);
               RedistributeL2R(pos);
               if( m_SubPages[pos+1]->Underflow() )
                       return false;
       }
       else // The problem is exactly at pos !
       {
               // Rotate L2R
               RedistributeL2R(pos-1);
               RedistributeR2L(pos+1);
               if( m_SubPages[pos]->Underflow() )
                       return false;
       }
       return true;
}

template <typename value_type, typename LinkedValueType>
void CBTreePage<value_type, LinkedValueType>::RedistributeR2L(size_t pos)  
{
       BTPage  *pSource = m_SubPages[ pos ],
               *pTarget = m_SubPages[pos-1];

       while(pSource->GetNumberOfKeys() > pSource->MinNumberOfKeys() &&
                 pTarget->GetNumberOfKeys() < pSource->GetNumberOfKeys() )
       {
               // Move from this page to the down-left page \/
               ::insert_at(pTarget->m_Keys, m_Keys[pos-1], pTarget->NumberOfKeys()++);
               // Move the pointer leftest pointer to the rightest position
               ::insert_at(pTarget->m_SubPages, pSource->m_SubPages[0], pTarget->NumberOfKeys());

               // Move the leftest element to the root
               m_Keys[pos-1] = pSource->m_Keys[0];

               // Remove the leftest element from rigth page
               ::remove(pSource->m_Keys    , 0);
               ::remove(pSource->m_SubPages, 0);
               pSource->NumberOfKeys()--;
       }
}

template <typename value_type, typename LinkedValueType>
void CBTreePage<value_type, LinkedValueType>::RedistributeL2R(size_t pos)
{
       BTPage  *pSource = m_SubPages[pos],
                       *pTarget = m_SubPages[pos+1];
       while(pSource->GetNumberOfKeys() > pSource->MinNumberOfKeys() &&
                 pTarget->GetNumberOfKeys() < pSource->GetNumberOfKeys() )
       {
               // Move from this page to the down-RIGHT page \/
               ::insert_at(pTarget->m_Keys, m_Keys[pos], 0);
               // Move the pointer rightest pointer to the leftest position
               ::insert_at(pTarget->m_SubPages, pSource->m_SubPages[pSource->NumberOfKeys()], 0);
               pTarget->NumberOfKeys()++;

               // Move the rightest element to the root
               m_Keys[pos] = pSource->m_Keys[pSource->NumberOfKeys()-1];

               // Remove the leftest element from rigth page
               // it is not necessary erase because m_KeyCount controls
               pSource->NumberOfKeys()--;
       }
}

template <typename value_type, typename LinkedValueType>
void CBTreePage<value_type, LinkedValueType>::SplitChild(size_t pos)
{
       // FIRST: deciding the second page to split
       BTPage  *pChild1 = 0, *pChild2 = 0;
       if( pos > 0 )                                   // is left page full ?
               if( m_SubPages[pos-1]->IsFull() )
               {
                       pChild1 = m_SubPages[pos-1];
                       pChild2 = m_SubPages[pos--];
               }
       if( pos < GetNumberOfKeys() )   // is right page full ?
               if( m_SubPages[pos+1]->IsFull() )
               {
                       pChild1 = m_SubPages[pos];
                       pChild2 = m_SubPages[pos+1];
               }
       size_t nKeys = pChild1->GetNumberOfKeys() + pChild2->GetNumberOfKeys() + 1;

       // SECOND: copy both pages to a temporal one
       // Create two tmp vector
       vector<ObjectInfo> tmpKeys;
       //tmpKeys.resize(nKeys);
       vector<BTPage *>   tmpSubPages;
       //tmpKeys.resize(nKeys+1);

       // copy from left child
       MovePage(pChild1, tmpKeys, tmpSubPages);
       // copy a key from parent
       tmpKeys    .push_back(m_Keys[pos]);

       // copy from right child
       MovePage(pChild2, tmpKeys, tmpSubPages);

       BTPage *pChild3 = 0;
       ObjectInfo oi1, oi2;
       SplitPageInto3(tmpKeys, tmpSubPages, pChild1, pChild2, pChild3, oi1, oi2);

       // copy the first element to the root
       m_Keys    [pos] = oi1;
       m_SubPages[pos] = pChild1;

       // copy the second element to the root
       ::insert_at(m_Keys, oi2, pos+1);
       ::insert_at(m_SubPages, pChild2, pos+1);
       NumberOfKeys()++;

       m_SubPages[pos+2] = pChild3;
}

template <typename value_type, typename LinkedValueType>
void CBTreePage<value_type, LinkedValueType>::SplitPageInto3(vector<ObjectInfo>& tmpKeys,
                                                vector<BTPage *>  & tmpSubPages,
                                                BTPage*                   &     pChild1,
                                                BTPage*                   &     pChild2,
                                                BTPage*                   &     pChild3,
                                                ObjectInfo                & oi1,
                                                ObjectInfo                & oi2)
{
       assert(tmpKeys.size() >= 8);
       assert(tmpSubPages.size() >= 9);
       if( !pChild1 )
               pChild1 = new BTPage(m_MaxKeysForChilds, m_Unique);

       // Split tmpKeys page into 3 pages
       // copy 1/3 elements to the first child
       pChild1->clear();
       size_t nKeys = (tmpKeys.size()-2)/3;
       size_t i = 0;
       for(; i < nKeys; i++ )
       {
               pChild1->m_Keys    [i] = tmpKeys    [i];
               pChild1->m_SubPages[i] = tmpSubPages[i];
               pChild1->NumberOfKeys()++;
       }
       pChild1->m_SubPages[i] = tmpSubPages[i];

       // first element to go up !
       oi1 = tmpKeys[i++];

       if( !pChild2 )
               pChild2 = new BTPage(m_MaxKeysForChilds, m_Unique);
       pChild2->clear();
       // copy 1/3 to the second child
       nKeys += (tmpKeys.size()-2)/3 + 1;
       size_t j = 0;
       for(; i < nKeys; i++, j++ )
       {
               pChild2->m_Keys    [j] = tmpKeys    [i];
               pChild2->m_SubPages[j] = tmpSubPages[i];
               pChild2->NumberOfKeys()++;
       }
       pChild2->m_SubPages[j] = tmpSubPages[i];

       // copy the second element to the root
       oi2 = tmpKeys[i++];

       // copy 1/3 to the third child
       if( !pChild3 )
               pChild3 = new BTPage(m_MaxKeysForChilds, m_Unique);
       pChild3->clear();
       nKeys = tmpKeys.size();
       for(j = 0; i < nKeys; i++, j++)
       {
               pChild3->m_Keys    [j] = tmpKeys    [i];
               pChild3->m_SubPages[j] = tmpSubPages[i];
               pChild3->NumberOfKeys()++;
       }
       pChild3->m_SubPages[j] = tmpSubPages[i];
}

template <typename value_type, typename LinkedValueType>
bool CBTreePage<value_type, LinkedValueType>::SplitRoot()
{
       BTPage  *pChild1 = 0, *pChild2 = 0, *pChild3 = 0;
       ObjectInfo oi1, oi2;
       SplitPageInto3( m_Keys,m_SubPages,pChild1, pChild2, pChild3, oi1, oi2);
       clear();

       // copy the first element to the root
       m_Keys    [0] = oi1;
       m_SubPages[0] = pChild1;
       NumberOfKeys()++;

       // copy the second element to the root
       m_Keys    [1] = oi2;
       m_SubPages[1] = pChild2;
       NumberOfKeys()++;

       m_SubPages[2] = pChild3;
       return true;
}

template <typename value_type, typename LinkedValueType>
template <typename CompareFn>
bool CBTreePage<value_type, LinkedValueType>::Search(const value_type &key, LinkedValueType &value, CompareFn compareFn)
{
       size_t pos = binary_search(m_Keys, 0, m_KeyCount, key, compareFn);
       if( pos >= m_KeyCount )
       {    if( m_SubPages[pos] )
                return m_SubPages[pos]->Search(key, value, compareFn);
            else
                return false;
       }
       if( key == m_Keys[pos].key )
       {
               value = m_Keys[pos].value;
               m_Keys[pos].UseCounter++;
               return true;
       }
       if( key < m_Keys[pos].key )
               if( m_SubPages[pos] )
                       return m_SubPages[pos]->Search(key, value, compareFn);
       return false;
}

template <typename value_type, typename LinkedValueType>
template <typename Callable, typename Iter, typename... Args>
void CBTreePage<value_type, LinkedValueType>::ForEachReverse(Callable fn, Iter level, Args&&... args)
{
       if( m_SubPages[m_KeyCount] )
               m_SubPages[m_KeyCount]->ForEach(fn, level+1, args...);
       for(size_t i = m_KeyCount-1 ; i >= 0  ; i--)
       {
               fn(m_Keys[i], level, args...);
               if( m_SubPages[i] )
                       m_SubPages[i]->ForEach(fn, level+1, args...);
       }
}

template <typename value_type, typename LinkedValueType>
template <typename Callable, typename Iter, typename... Args>
void CBTreePage<value_type, LinkedValueType>::ForEach(Callable fn, Iter level, Args&&... args)
{
        for (size_t i = 0; i < m_KeyCount; i++)
        {
                if (m_SubPages[i])
                        m_SubPages[i]->ForEach(fn, level + 1, args...);
                fn(m_Keys[i], level, args...);
        }
        if (m_SubPages[m_KeyCount])
                m_SubPages[m_KeyCount]->ForEach(fn, level + 1, args...);
}

template <typename value_type, typename LinkedValueType>
template <typename Callable, typename Iter, typename... Args>
typename CBTreePage<value_type, LinkedValueType>::ObjectInfo *
CBTreePage<value_type, LinkedValueType>::FirstThat(Callable fn, Iter level, Args&&... args)
{
        ObjectInfo *pTmp;
        for (size_t i = 0; i < m_KeyCount; i++)
        {
                if (m_SubPages[i])
                        if ((pTmp = m_SubPages[i]->FirstThat(fn, level + 1, args...)))
                                return pTmp;
                if (fn(m_Keys[i], level, args...))
                        return &m_Keys[i];
        }
        if (m_SubPages[m_KeyCount])
                if ((pTmp = m_SubPages[m_KeyCount]->FirstThat(fn, level + 1, args...)))
                        return pTmp;
        return 0;
}

template <typename value_type, typename LinkedValueType>
template <typename CompareFn>
bt_ErrorCode CBTreePage<value_type, LinkedValueType>::Remove(const value_type &key, const LinkedValueType value, CompareFn compareFn)
{
       bt_ErrorCode error = bt_ok;
       size_t pos = binary_search(m_Keys, 0, m_KeyCount, key, compareFn);
       if( pos < NumberOfKeys() && key == m_Keys[pos].key /*&& m_Keys[pos].value == value*/) // We found it !
       {
               // This is a leave: First
               if( !m_SubPages[pos+1] )  // This is a leave ? FIRST CASE !
               {
                       ::remove(m_Keys, pos);
                       NumberOfKeys()--;
                       if( Underflow() )
                               return bt_underflow;
                       return bt_ok;
               }

               // We FOUND IT BUT it is NOT a leave ? SECOND CASE !
               {
                       // Get the first element from right branch
                       ObjectInfo &rFirstFromRight = m_SubPages[pos+1]->GetFirstObjectInfo();
                       // change with a leave
                       swap(m_Keys[pos], rFirstFromRight);
                       // Remove it from this leave

                       //Print(cout);
                       error = m_SubPages[++pos]->Remove(key, value);
               }
       }
       else if( pos == NumberOfKeys() ) // it is not here, go by the last branch
               error = m_SubPages[pos]->Remove(key, value);
       else if( key <= m_Keys[pos].key ) // = is because identical keys are inserted on left (see Insert)
       {        if( m_SubPages[pos] )
                       error = m_SubPages[pos]->Remove(key, value);
               else
                       return bt_nofound;
       }
       if( error == bt_underflow )
       {
               // THIRD CASE: After removing the element we have an underflow
               // Print(cout);
               if( TreatUnderflow(pos) )
                       return bt_ok;
               // FOURTH CASE: it was not possible to redistribute -> Merge
               if( IsRoot() && NumberOfKeys() == 2 )
                       return MergeRoot();
               return Merge(pos);
       }
       if( error == bt_nofound )
               return bt_nofound;
       return bt_ok;
}


template <typename value_type, typename LinkedValueType>
bt_ErrorCode CBTreePage<value_type, LinkedValueType>::Merge(size_t pos)
{
       assert( m_SubPages[pos-1]->NumberOfKeys() +
                       m_SubPages[ pos ]->NumberOfKeys() +
                       m_SubPages[pos+1]->NumberOfKeys() ==
                       3*m_SubPages[ pos ]->MinNumberOfKeys() - 1);

       // FIRST: Put all the elements into a vector
       vector<ObjectInfo> tmpKeys;
       //tmpKeys.resize(nKeys);
       vector<BTPage *>   tmpSubPages;

       BTPage  *pChild1 = m_SubPages[pos-1],
                       *pChild2 = m_SubPages[ pos ],
                       *pChild3 = m_SubPages[pos+1];
       MovePage(pChild1, tmpKeys, tmpSubPages);
       tmpKeys    .push_back(m_Keys[pos-1]);
       MovePage(pChild2, tmpKeys, tmpSubPages);
       tmpKeys    .push_back(m_Keys[ pos ]);
       MovePage(pChild3, tmpKeys, tmpSubPages);
       pChild3->Destroy();;

       // Move 1/2 elements to pChild1
       size_t nKeys = pChild1->GetFreeCells();
       size_t i = 0;
       for(; i < nKeys ; i++ )
       {
               pChild1->m_Keys    [i] = tmpKeys    [i];
               pChild1->m_SubPages[i] = tmpSubPages[i];
               pChild1->NumberOfKeys()++;
       }
       pChild1->m_SubPages[i] = tmpSubPages[i];

       m_Keys    [pos-1] = tmpKeys[i];
       m_SubPages[pos-1] = pChild1;

       ::remove(m_Keys    , pos);
       ::remove(m_SubPages, pos);
       NumberOfKeys()--;

       nKeys = pChild2->GetFreeCells();
       size_t j = ++i;
       for(i = 0 ; i < nKeys ; i++, j++ )
       {
               pChild2->m_Keys    [i] = tmpKeys    [j];
               pChild2->m_SubPages[i] = tmpSubPages[j];
               pChild2->NumberOfKeys()++;
       }
       pChild2->m_SubPages[i] = tmpSubPages[j];
       m_SubPages[ pos ]          = pChild2;

       if( Underflow() )
               return bt_underflow;
       return bt_ok;
}

template <typename value_type, typename LinkedValueType>
bt_ErrorCode CBTreePage<value_type, LinkedValueType>::MergeRoot()
{
       size_t pos = 1;
       assert( m_SubPages[pos-1]->NumberOfKeys() +
                       m_SubPages[ pos ]->NumberOfKeys() +
                       m_SubPages[pos+1]->NumberOfKeys() ==
                       3*m_SubPages[ pos ]->MinNumberOfKeys() - 1);

       BTPage  *pChild1 = m_SubPages[pos-1], *pChild2 = m_SubPages[ pos ], *pChild3 = m_SubPages[pos+1];
       size_t nKeys = pChild1->NumberOfKeys() + pChild2->NumberOfKeys() + pChild3->NumberOfKeys() + 2;

       // FIRST: Put all the elements into a vector
       vector<ObjectInfo> tmpKeys;
       //tmpKeys.resize(nKeys);
       vector<BTPage *>   tmpSubPages;

       MovePage(pChild1, tmpKeys, tmpSubPages);
       tmpKeys    .push_back(m_Keys[pos-1]);
       MovePage(pChild2, tmpKeys, tmpSubPages);
       tmpKeys    .push_back(m_Keys[ pos ]);
       MovePage(pChild3, tmpKeys, tmpSubPages);

       clear();
       size_t i = 0;
       for( ; i < nKeys ; i++ )
       {
               m_Keys    [i] = tmpKeys    [i];
               m_SubPages[i] = tmpSubPages[i];
               NumberOfKeys()++;
       }
       m_SubPages[i] = tmpSubPages[i];

       //Print(cout);
       pChild1->Destroy();
       pChild2->Destroy();
       pChild3->Destroy();

       return bt_rootmerged;
}

template <typename value_type, typename LinkedValueType>
typename CBTreePage<value_type, LinkedValueType>::ObjectInfo &
CBTreePage<value_type, LinkedValueType>::GetFirstObjectInfo()
{
        if( m_SubPages[0] )
                return m_SubPages[0]->GetFirstObjectInfo();
        return m_Keys[0];
}

template <typename value_type, typename LinkedValueType>
void Print(tagObjectInfo<value_type, LinkedValueType> &info, size_t level, void *pExtra)
{
       ostream &os = *(ostream *)pExtra;
       for(size_t i = 0; i < level ; i++)
               os << "\t";
       os << info.key << "->" << info.value << "\n";
}

template <typename value_type, typename LinkedValueType>
void CBTreePage<value_type, LinkedValueType>::Print(ostream & os)
{
        ForEach(&::Print<value_type, LinkedValueType>, 0, &os);
}

template <typename value_type, typename LinkedValueType>
void CBTreePage<value_type, LinkedValueType>::Read (istream &is)
{
        size_t size; is >> size;
        string separator;
        value_type key;
        LinkedValueType value;
        while (size--) {
                is >> key;
                is >> separator;
                is >> value;
                Insert(key, value);
        }
}

template <typename value_type, typename LinkedValueType>
void CBTreePage<value_type, LinkedValueType>::Create()
{
        Reset();
        m_Keys.resize(m_MaxKeys+1);
        m_SubPages.resize(m_MaxKeys+2, NULL);
        m_KeyCount = 0;
        m_MinKeys  = 2 * m_MaxKeys/3;
}

template <typename value_type, typename LinkedValueType>
void CBTreePage<value_type, LinkedValueType>::Reset(){
        for( size_t i = 0 ; i < m_KeyCount ; i++ )
        delete m_SubPages[i];
        clear();
}

template <typename value_type, typename LinkedValueType>
void CBTreePage<value_type, LinkedValueType>::clear()
{
       //m_Keys.clear();
       //m_SubPages.clear();
       m_KeyCount = 0;
}

template <typename value_type, typename LinkedValueType>
CBTreePage<value_type, LinkedValueType> * CreateBTreeNode (size_t maxKeys, bool unique)
{
       return new CBTreePage<value_type, LinkedValueType> (maxKeys, unique);
}

template <typename value_type, typename LinkedValueType>
void CBTreePage<value_type, LinkedValueType>::MovePage(BTPage *pChildPage, vector<ObjectInfo> &tmpKeys, vector<BTPage *> &tmpSubPages)
{
       size_t nKeys = pChildPage->GetNumberOfKeys();
       size_t i = 0;
       for(i = 0; i < nKeys; i++ )
       {
                tmpKeys    .push_back(pChildPage->m_Keys[i]);
                tmpSubPages.push_back(pChildPage->m_SubPages[i]);
       }
       tmpSubPages.push_back(pChildPage->m_SubPages[i]);
       pChildPage->clear();
}

template <typename T, typename V>
ostream &operator<<(ostream &os, CBTreePage<T, V> &bTreePage)
{
        bTreePage.Print(os);
        return os;
}

template <typename T, typename V>
istream &operator>>(istream &is, CBTreePage<T, V> &bTreePage)
{
        bTreePage.Read(is);
        return is;
}

#endif
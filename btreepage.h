#ifndef __CBTreePage_H__
#define __CBTreePage_H__

#include <vector>
#include <assert.h>
#include <functional>
#include "xtrait.h"

// TODO: #1 Crear una function para agregarla al demo.cpp ( no trivial )
// TODO: #2 Agregarle un Trait (prueba git) ( no trivial )
// TODO: #3 crear un iterator ( no trivial )
//       Sugerencia: Tarea1 cada pagina debe tener un puntero al padre primero ( no trivial )
// TODO: #4 integrarlo al recorrer ( no trivial )


template<typename Container>
class iterator_Btree{
    public:
        typedef typename Container::Node Node;
        typedef typename Node::value_type     value_type;
        typedef iterator_Btree<Container> myself;
    protected:
        Container *m_pContainer;
        Node      *m_pNode;
        size_t    m_dir;  
    public:
        iterator_Btree(Container *pContainer, Node *pNode,size_t dir)
        : m_pContainer(pContainer), m_pNode(pNode),m_dir(dir){}
        iterator_Btree(myself &other)
        : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode),m_dir(other.m_dir){}
        iterator_Btree(myself &&other) {   
            m_pContainer = move(other.m_pContainer);
            m_pNode      = move(other.m_pNode);
            m_dir        =move(other.m_dir);
          }
        bool operator==(myself iter)   { return m_pNode == iter.m_pNode; }
        bool operator!=(myself iter)   { return !(*this == iter);        }
        value_type &operator*() { return m_pNode->getDataRef();}
        iterator_Btree & operator++() {
          if(m_dir==1)
            m_pNode=m_pNode->m_pNext;
          else
            m_pNode=m_pNode->m_pPrev;
          return *this;
        }
};

template <typename Trait>
class BTree;

using namespace std;
enum bt_ErrorCode {bt_ok, bt_overflow, bt_underflow, bt_duplicate, bt_nofound, bt_rootmerged};

template <typename Container, typename value_type>
size_t binary_search(Container& container, size_t first, size_t last, value_type &object){
       if( first >= last ){ 
	return first;
       }    
       while( first < last ){
	       size_t mid = (first+last)/2;
	       if( object == container[mid]->getData() )
		       return mid;
	       if( object > container[mid ]->getData() )
		       first = mid+1;
	       else
		       last  = mid;
       }
       if(container[first]){
		if( object <= container[first]->getData()){
		return first;
		}   
       }        
       return last;
}

// Error al poner size_t
// Posible motivo: El i está disminuyendo
template <typename Container>
void Print_Route(Container& ct){
   size_t n_count=0;
   size_t size=ct.size();
   for(auto i=0;i<=size-1;i++){
      if(ct[i]){
         n_count++;
      }
   }
   for (auto i=0;i<=n_count-1;i++){
      if(ct[i]){
         cout<<"Iter "<<i<< " "<<(ct[i]->m_pPrev?ct[i]->m_pPrev->getData():-99)<<"<--Prev--"<<ct[i]->getData()<<"--Next-->"<<(ct[i]->m_pNext?ct[i]->m_pNext->getData():-99)<<endl;
      }
      else{
         cout<<"Iter: "<<i<<" --"<<ct[i]<<" No Data"<<endl;
      }
	      
   }
       
}
template <typename Container,typename Obj>
auto Find_Begin(Container ct, Obj ct_p) { 
    Obj pTmp = ct_p;
    while (pTmp->m_pPrev) {
        pTmp = pTmp->m_pPrev;
    }
    return pTmp;
}

template <typename Container, typename Obj>
auto Find_End(Container ct,Obj ct_p){
   Obj pTmp=ct_p;
   while(pTmp->m_pNext){
      pTmp=pTmp->m_pNext;
   }
   //cout<<"\nTail: "<<pTmp->getData()<<endl;
   return pTmp;
}
template <typename Container>
void Assign_Route(Container& ct,size_t pos){
   auto pTmp = ct[pos];
   size_t t_cont=0;
   for(auto i=0;i<=ct.size()-1;i++){
      if(ct[i])
         t_cont++;
   }
   if(t_cont>1){
      if(t_cont-1==pos){
         ct[pos]->m_pNext=ct[pos-1]->m_pNext;
         if(ct[pos-1]->m_pNext)
            ct[pos-1]->m_pNext->m_pPrev=ct[pos];
         ct[pos]->m_pPrev=ct[pos-1];
         ct[pos-1]->m_pNext=ct[pos];   
      }
      else{
         if(pos>0){
            ct[pos]->m_pNext=ct[pos+1];
            ct[pos+1]->m_pPrev=ct[pos];
            ct[pos]->m_pPrev=ct[pos-1];
            ct[pos-1]->m_pNext=ct[pos];
         }
         else{
            auto p_Parent=ct[pos+1]->m_pPrev;
            if(p_Parent){
               ct[pos]->m_pPrev=p_Parent;
               p_Parent->m_pNext=ct[pos];
            }
            ct[pos]->m_pNext=ct[pos+1];
            ct[pos+1]->m_pPrev=ct[pos];
         }
      }
   }   
}
template <typename Container, typename T_Obj>
void insert_at(Container& ct, T_Obj* pNode, int pos){
   size_t size = ct.size();
   for(int i = size-2 ; i >= pos ; i--){
	   ct[i+1] = ct[i];       
   } 
   ct[pos] =  pNode;
   ::Assign_Route(ct,pos);
}
template <typename Container ,typename T_Obj>
void insert_at_page(Container& ct, T_Obj* object, int pos)
{
    size_t size = ct.size();
    for(int i = size-2; i >= pos; i--){
	    ct[i+1] = ct[i];
    }      
    ct[pos] = object;  
}


template <typename Container>
void remove(Container& container, size_t pos){
   size_t size = container.size();
   size_t ct_count=0;
   for(auto i=0;i<=size-1;i++){
      if(container[i])
         ct_count++;
   }
   //cout<<"\nRemove"<<endl;
   //cout<<"size: "<<size<<" ct_count: "<<ct_count;
   for(auto i = pos+1 ; i < size ; i++){
      container[i-1] = container[i];
   }
   container[size-1]=nullptr;


}
template <typename Container>
void remove_t(Container& container, size_t pos){
   size_t size = container.size();
   for(auto i = pos+1 ; i < size ; i++){
      //container[i-1] = container[i];
      container[i-1]=nullptr;
   }
   container[size-1]=nullptr;
}
template <typename Trait>
class CBTreePage 
{
       friend class BTree<Trait>;
       typedef typename Trait::value_type  value_type;
       typedef typename Trait::LinkedValueType  LinkedValueType;
       typedef CBTreePage<Trait>    BTPage; 
 public:      
       typedef typename Trait::Node Node; 
       typedef Node *(*lpfnFirstThat2)(Node &info, size_t level, void *pExtra1);
       typedef Node *(*lpfnFirstThat3)(Node &info, size_t level, void *pExtra1, void *pExtra2);
       template<typename... Extras>
       using lpfnForEach = void (*)(Node &info, size_t level, Extras...);
        typedef iterator_Btree<BTPage>  iterator;
 public:
       CBTreePage(size_t maxKeys, bool unique = true);
       virtual ~CBTreePage();
       bt_ErrorCode    Insert (const value_type &key, const LinkedValueType ObjID);
       bt_ErrorCode    Remove (const value_type &key, const LinkedValueType ObjID);
       bool            Search (const value_type &key, LinkedValueType &ObjID);
       void            Print  (ostream &os);
       void            Print_Route(BTPage &Obj);
       void            Print_Route ();
       template <typename Container,typename Obj,typename Root>
       void Pointer_Settings(Container ct,Obj cp_t,Root &rt);
       template <typename F,typename... Extras>
       void            Function_G(F function,Extras... extras);
       template <typename F,typename... Extras>
       void            Function_G_Reverse(F function,Extras... extras);
       template <typename... Extras>
       void            ForEachG(lpfnForEach<Extras...> lpfn, size_t level, Extras... extras);
       template <typename... Extras>
       void            ForEachG_Reverse(lpfnForEach<Extras...> lpfn, size_t level, Extras... extras);
      iterator begin() { iterator iter(this,this->m_Heap,1);     return iter;    }
      iterator end()   { iterator iter(this,nullptr,1);     return iter;    }
      iterator rbegin() { iterator iter(this,this->m_Tail,-1);     return iter;    }
      iterator rend()   { iterator iter(this,nullptr,-1);     return iter;    }
       // TODO: #8 You may reduce these two function by using variadic templates
       Node*     FirstThat(lpfnFirstThat2 lpfn, size_t level, void *pExtra1);
       Node*     FirstThat(lpfnFirstThat3 lpfn, size_t level, void *pExtra1, void *pExtra2);
       Node *m_Heap=nullptr;
       Node *m_Tail=nullptr;

protected:
       size_t  m_MinKeys; // minimum number of keys in a node
       size_t  m_MaxKeys, // maximum number of keys in a node
		 m_MaxKeysForChilds; // just to distinguish the root
       bool m_Unique;
       bool m_isRoot;
       vector<Node*> m_Keys;
       vector<BTPage *>m_SubPages;
       size_t  m_KeyCount;
       CBTreePage* parent=nullptr;
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
       void  SplitChild (size_t &pos);

       Node* GetFirstObjectInfo();

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

private:
       bool SplitRoot();
       void SplitPageInto3(vector<Node*>   & tmpKeys,
					       vector<BTPage *>  & SubPages,
					       BTPage           *& pChild1,
					       BTPage           *& pChild2,
					       BTPage           *& pChild3,
					       Node*        & oi1,
					       Node*       & oi2);
       void MovePage(BTPage *  pChildPage,vector<Node*> & tmpKeys,vector<BTPage *> & tmpSubPages);
};

template <typename Trait>
CBTreePage<Trait>:: CBTreePage(size_t maxKeys, bool unique)
			       : m_MaxKeys(maxKeys), m_Unique(unique), m_KeyCount(0)
{
       Create();
       SetMaxKeysForChilds(m_MaxKeys);
}

template <typename Trait>
CBTreePage<Trait>::~CBTreePage()
{
       Reset();
}

template <typename Trait>
bt_ErrorCode CBTreePage<Trait>::Insert(const value_type& key, const LinkedValueType ObjID){
   size_t pos = binary_search(m_Keys, 0, m_KeyCount, key);
   bt_ErrorCode error = bt_ok;
   if( pos < m_KeyCount && m_Keys[pos]->getData() == key && m_Unique)
      return bt_duplicate; // this key is duplicate

   if( !m_SubPages[pos] ){
      ::insert_at(m_Keys, new Node(key, ObjID), pos);
      m_KeyCount++;
      //Here Should set the m_Heap,m_Tail
      // cout<<"\nPointer Transfer "<<m_Keys[pos]->getData()<<endl;
      auto pParent=this;
      while(pParent->parent){
         pParent=pParent->parent;
      }
      // for(auto i=0;i<m_Keys.size();i++){
      //    if(m_Keys[i])
      //       cout<<"\nmKeys_data"<<m_Keys[i]->getData();
      // }
      Pointer_Settings(m_Keys,m_Keys[pos],pParent);
      if( Overflow() )
         return bt_overflow;
      return bt_ok;
   }
   else{
      // recursive insertion
      error = m_SubPages[pos]->Insert(key, ObjID);
      if( error == bt_overflow ){
                     //cout<<"\nIngresa Redistribute1"<<endl;
                     if(!Redistribute1(pos)){
                        SplitChild(pos);
                     }
                     if(Overflow())          // Propagate overflow
                        return bt_overflow;
                     return bt_ok;
      }
   }

   if( Overflow() ) // node overflow
      return bt_overflow;
   return bt_ok;
}

template <typename Trait>
bool CBTreePage<Trait>::Redistribute1(size_t &pos){
       //cout<<"\nInto Redistribute1"<<endl;
       if( m_SubPages[pos]->Underflow() ){
         //cout<<"\nInto Underflow"<<endl;
               size_t NumberOfKeyOnLeft = 0,NumberOfKeyOnRight = 0;
               // is this the first element or there are more elements on right brother
               if( pos > 0 )
                  NumberOfKeyOnLeft = m_SubPages[pos-1]->NumberOfKeys();
               if( pos < NumberOfKeys() )
                  NumberOfKeyOnRight = m_SubPages[pos+1]->NumberOfKeys();
               //cout<<"\n"<<NumberOfKeyOnLeft<<"--"<<NumberOfKeyOnRight<<endl;
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
          //cout<<"\nInto Overflow"<<endl;
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
template <typename Trait>
bool CBTreePage<Trait>::Redistribute2(size_t pos)
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

template <typename Trait>
void CBTreePage<Trait>::RedistributeR2L(size_t pos)  
{      //cout<<"\nInto R2L"<<endl;
       BTPage  *pSource = m_SubPages[ pos ],*pTarget = m_SubPages[pos-1];
       while(pSource->GetNumberOfKeys() > pSource->MinNumberOfKeys() &&
		 pTarget->GetNumberOfKeys() < pSource->GetNumberOfKeys() )
       {
	       // Move from this page to the down-left page \/
	       ::insert_at_page(pTarget->m_Keys, m_Keys[pos-1], pTarget->NumberOfKeys()++);
	       // Move the pointer leftest pointer to the rightest position
	       ::insert_at_page(pTarget->m_SubPages, pSource->m_SubPages[0], pTarget->NumberOfKeys());
	       // Move the leftest element to the root
	       m_Keys[pos-1] = pSource->m_Keys[0];

	       // Remove the leftest element from rigth page
             // cout<<"\nFirst Remove"<<endl;
	       ::remove(pSource->m_Keys    , 0);
             // cout<<"\nSecond Remove"<<endl;
	       ::remove(pSource->m_SubPages, 0);
	       pSource->NumberOfKeys()--;
       }
}

template <typename Trait>
void CBTreePage<Trait>::RedistributeL2R(size_t pos){
      //cout<<"\nInto L2R"<<endl;
       BTPage  *pSource = m_SubPages[pos],*pTarget = m_SubPages[pos+1];
       while(pSource->GetNumberOfKeys() > pSource->MinNumberOfKeys() && pTarget->GetNumberOfKeys() < pSource->GetNumberOfKeys() ){
	       // Move from this page to the down-RIGHT page \/
	       ::insert_at_page(pTarget->m_Keys, m_Keys[pos], 0);
	       // Move the pointer rightest pointer to the leftest position
	       ::insert_at_page(pTarget->m_SubPages, pSource->m_SubPages[pSource->NumberOfKeys()], 0);
	       pTarget->NumberOfKeys()++;
	       // Move the rightest element to the root
	       m_Keys[pos] = pSource->m_Keys[pSource->NumberOfKeys()-1];
	       // Remove the leftest element from rigth page
	       // it is not necessary erase because m_KeyCount controls
         ::remove_t(pSource->m_Keys,pSource->NumberOfKeys()-1);
         size_t t_ct=0;
         for(auto i=0;i<=pSource->m_SubPages.size();i++){
               if(pSource->m_SubPages[i])
                        t_ct++;
         }
         if(t_ct>0)
            ::remove_t(pSource->m_SubPages,t_ct-1);
	      pSource->NumberOfKeys()--;
       }
}

template <typename Trait>
void CBTreePage<Trait>::SplitChild(size_t &pos)
{      //cout<<"\nInto SplitChild"<<endl;
       // FIRST: deciding the second page to split
       BTPage  *pChild1 = 0, *pChild2 = 0;
       if( pos > 0 )                                   // is left page full ?
	       if( m_SubPages[pos-1]->IsFull()){
		       pChild1 = m_SubPages[pos-1];
		       pChild2 = m_SubPages[pos--];
	        }
       if( pos < GetNumberOfKeys() )   // is right page full ?
	       if( m_SubPages[pos+1]->IsFull()){
		       pChild1 = m_SubPages[pos];
		       pChild2 = m_SubPages[pos+1];
	       }
       size_t nKeys = pChild1->GetNumberOfKeys() + pChild2->GetNumberOfKeys() + 1;
      // cout<<"\n"<<nKeys<<pChild1->GetNumberOfKeys()<<"-"<<pChild2->GetNumberOfKeys();
       // SECOND: copy both pages to a temporal one
       // Create two tmp vector
       vector<Node*> tmpKeys;
       //tmpKeys.resize(nKeys);
       vector<BTPage *>   tmpSubPages;
       //tmpKeys.resize(nKeys+1);

       // copy from left child
       MovePage(pChild1, tmpKeys, tmpSubPages);
       // copy a key from parent
       tmpKeys.push_back(m_Keys[pos]);
       // copy from right child
       MovePage(pChild2, tmpKeys, tmpSubPages);

       BTPage *pChild3 = 0;
       Node* oi1;
       Node* oi2;
       SplitPageInto3(tmpKeys, tmpSubPages, pChild1, pChild2, pChild3, oi1, oi2);

       // copy the first element to the root
       m_Keys    [pos] = oi1;
       m_SubPages[pos] = pChild1;

       // copy the second element to the root
       ::insert_at_page(m_Keys, oi2, pos+1);
       ::insert_at_page(m_SubPages, pChild2, pos+1);
       NumberOfKeys()++;
       m_SubPages[pos+2] = pChild3;
       //Remove the last element
       
       for(auto i=0;i<=2;i++){
         ::remove_t(m_SubPages[pos+i]->m_Keys,2);
         m_SubPages[pos+i]->parent = this;
       }
}

template <typename Trait>
void CBTreePage<Trait>::SplitPageInto3(vector<Node*>& tmpKeys,
						vector<BTPage *>  & tmpSubPages,
						BTPage*                   &     pChild1,
						BTPage*                   &     pChild2,
						BTPage*                   &     pChild3,
						Node*               & oi1,
						Node*                & oi2)
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

template <typename Trait>
bool CBTreePage<Trait>::SplitRoot()
{
       BTPage  *pChild1 = 0, *pChild2 = 0, *pChild3 = 0;
      //  pChild1->parent = this;
      //  pChild2->parent = this;
      //  pChild3->parent = this;
       Node* oi1;
       Node* oi2;
       SplitPageInto3(m_Keys,m_SubPages,pChild1, pChild2, pChild3, oi1, oi2);
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
       m_SubPages[0]->parent = this;
       m_SubPages[1]->parent = this;
       m_SubPages[2]->parent = this;      
       return true;
}

template <typename Trait>
bool CBTreePage<Trait>::Search(const value_type &key, LinkedValueType &ObjID)
{
       size_t pos = binary_search(m_Keys, 0, m_KeyCount, key);
       if( pos >= m_KeyCount )
       {    if( m_SubPages[pos] )
		return m_SubPages[pos]->Search(key, ObjID);
	    else
		return false;
       }
       if( key == m_Keys[pos]->getData() )
       {
	       ObjID = m_Keys[pos]->getValue();
	       m_Keys[pos]->getUseCounterRef()++;
	       return true;
       }
       if( key < m_Keys[pos]->getData())
	       if( m_SubPages[pos] )
		       return m_SubPages[pos]->Search(key, ObjID);
       return false;
}

/*template <typename value_type, typename LinkedValueType>
void CBTreePage<value_type, LinkedValueType>::ForEachReverse(lpfnForEach2 lpfn, size_t level, void *pExtra1)
{
       if( m_SubPages[m_KeyCount] )
	       m_SubPages[m_KeyCount]->ForEach(lpfn, level+1, pExtra1);
       for(size_t i = m_KeyCount-1 ; i >= 0  ; i--)
       {
	       lpfn(m_Keys[i], level, pExtra1);
	       if( m_SubPages[i] )
		       m_SubPages[i]->ForEach(lpfn, level+1, pExtra1);
       }
}*/


// TODO replace by generic foreach
// template <typename Trait>
// void CBTreePage<Trait>::ForEach(lpfnForEach2 lpfn, size_t level, void *pExtra1)
// {
//        for(size_t i = 0 ; i < m_KeyCount ; i++)
//        {
//                if( m_SubPages[i] )
//                        m_SubPages[i]->ForEach(lpfn, level+1, pExtra1);
//                lpfn(m_Keys[i], level, pExtra1);
//        }
//        if( m_SubPages[m_KeyCount] )
//                m_SubPages[m_KeyCount]->ForEach(lpfn, level+1, pExtra1);
// }
// Generic ForEach
template <typename Trait>
template <typename... Extras>
void CBTreePage<Trait>::ForEachG(lpfnForEach<Extras...> lpfn, size_t level, Extras... extras){
    for(size_t i = 0; i < m_KeyCount; i++){
	if(m_SubPages[i]){
	    m_SubPages[i]->ForEachG(lpfn, level+1, extras...);
	}
	lpfn(*m_Keys[i], level, extras...);
    }
    if(m_SubPages[m_KeyCount]){
	m_SubPages[m_KeyCount]->ForEachG(lpfn, level+1, extras...);
    }
}
template <typename Trait>
template <typename... Extras>
void CBTreePage<Trait>::ForEachG_Reverse(lpfnForEach<Extras...> lpfn, size_t level, Extras... extras){
    if(m_SubPages[m_KeyCount]){
	m_SubPages[m_KeyCount]->ForEachG(lpfn, level+1, extras...);
    }
    for(int i = static_cast<int>(m_KeyCount) - 1; i >= 0; i--){
	lpfn(*m_Keys[i], level, extras...);
	if(m_SubPages[i]){
	    m_SubPages[i]->ForEachG(lpfn, level+1, extras...);
	}
       
    }
    
}

/*template <typename value_type, typename LinkedValueType>
void CBTreePage<value_type, LinkedValueType>::ForEachReverse(lpfnForEach3 lpfn,
												       size_t level, void *pExtra1, void *pExtra2)
{
       if( m_SubPages[m_KeyCount] )
	       m_SubPages[m_KeyCount]->ForEach(lpfn, level+1, pExtra1, pExtra2);
       for(size_t i = m_KeyCount-1 ; i >= 0  ; i--)
       {
	       lpfn(m_Keys[i], level, pExtra1, pExtra2);
	       if( m_SubPages[i] )
		       m_SubPages[i]->ForEach(lpfn, level+1, pExtra1, pExtra2);
       }
}*/

// TODO replace by generic foreach
// template <typename Trait>
// void CBTreePage<Trait>::ForEach(lpfnForEach3 lpfn, size_t level, void *pExtra1, void *pExtra2)
// {
//        for(size_t i = 0 ; i < m_KeyCount ; i++)
//        {
//                if( m_SubPages[i] )
//                        m_SubPages[i]->ForEach(lpfn, level+1, pExtra1, pExtra2);
//                lpfn(m_Keys[i], level, pExtra1, pExtra2);
//        }
//        if( m_SubPages[m_KeyCount] )
//                m_SubPages[m_KeyCount]->ForEach(lpfn, level+1, pExtra1, pExtra2);
// }

// TODO replace by generic firstthat
template <typename Trait>
typename CBTreePage<Trait>::Node *
CBTreePage<Trait>::FirstThat(lpfnFirstThat2 lpfn, size_t level, void *pExtra1)
{
       Node *pTmp;
       for(size_t i = 0 ; i < m_KeyCount ; i++)
       {
	       if( m_SubPages[i] )
		       if( (pTmp = m_SubPages[i]->FirstThat(lpfn, level+1, pExtra1)) )
			       return pTmp;
	       if( lpfn(m_Keys[i], level, pExtra1) )
		       return &m_Keys[i];
       }
       if( m_SubPages[m_KeyCount] )
	       if( (pTmp = m_SubPages[m_KeyCount]->FirstThat(lpfn, level+1, pExtra1)) )
		       return pTmp;
       return 0;
}

// TODO replace by generic firstthat
template <typename Trait>
typename CBTreePage<Trait>::Node *
CBTreePage<Trait>::FirstThat(lpfnFirstThat3 lpfn,size_t level, void *pExtra1, void *pExtra2)
{
       Node *pTmp;
       for(size_t i = 0 ; i < m_KeyCount ; i++)
       {
	       if( m_SubPages[i] )
		       if( (pTmp = m_SubPages[i]->FirstThat(lpfn, level+1, pExtra1, pExtra2) ) )
			       return pTmp;
	       if( lpfn(m_Keys[i], level, pExtra1, pExtra2) )
		       return &m_Keys[i];
       }
       if( m_SubPages[m_KeyCount] )
	       if( (pTmp = m_SubPages[m_KeyCount]->FirstThat(lpfn, level+1, pExtra1, pExtra2) ) )
		       return pTmp;
       return 0;
}

template <typename Trait>
bt_ErrorCode CBTreePage<Trait>::Remove(const value_type &key, const LinkedValueType ObjID)
{
       bt_ErrorCode error = bt_ok;
       size_t pos = binary_search(m_Keys, 0, m_KeyCount, key);
       if( pos < NumberOfKeys() && key == m_Keys[pos]->getData() /*&& m_Keys[pos].m_ObjID == ObjID*/) // We found it !
       {
          //Reasigne directions
          Node *pNext=m_Keys[pos]->m_pNext;
          Node *pPrev=m_Keys[pos]->m_pPrev;
          if(pNext){
            if(pPrev){
               pPrev->m_pNext=pNext;
               pNext->m_pPrev=pPrev;
            }
            else
               pNext->m_pPrev=nullptr;
          }
          else{
            if(pPrev)
               pPrev->m_pNext=nullptr;
          }
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
		       Node* rFirstFromRight = m_SubPages[pos+1]->GetFirstObjectInfo();
		       // change with a leave
		       swap(m_Keys[pos], rFirstFromRight);
		       // Remove it from this leave

		       //Print(cout);
		       error = m_SubPages[++pos]->Remove(key, ObjID);
	       }
       }
       else if( pos == NumberOfKeys() ) // it is not here, go by the last branch
	       error = m_SubPages[pos]->Remove(key, ObjID);
       else if( key <= m_Keys[pos]->getData() ) // = is because identical keys are inserted on left (see Insert)
       {        if( m_SubPages[pos] )
		       error = m_SubPages[pos]->Remove(key, ObjID);
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


template <typename Trait>
bt_ErrorCode CBTreePage<Trait>::Merge(size_t pos)
{
       assert( m_SubPages[pos-1]->NumberOfKeys() +
		       m_SubPages[ pos ]->NumberOfKeys() +
		       m_SubPages[pos+1]->NumberOfKeys() ==
		       3*m_SubPages[ pos ]->MinNumberOfKeys() - 1);

       // FIRST: Put all the elements into a vector
       vector<Node*> tmpKeys;
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

template <typename Trait>
bt_ErrorCode CBTreePage<Trait>::MergeRoot()
{
       size_t pos = 1;
       assert( m_SubPages[pos-1]->NumberOfKeys() +
		       m_SubPages[ pos ]->NumberOfKeys() +
		       m_SubPages[pos+1]->NumberOfKeys() ==
		       3*m_SubPages[ pos ]->MinNumberOfKeys() - 1);

       BTPage  *pChild1 = m_SubPages[pos-1], *pChild2 = m_SubPages[ pos ], *pChild3 = m_SubPages[pos+1];
       size_t nKeys = pChild1->NumberOfKeys() + pChild2->NumberOfKeys() + pChild3->NumberOfKeys() + 2;

       // FIRST: Put all the elements into a vector
       vector<Node*> tmpKeys;
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

template <typename Trait>
typename CBTreePage<Trait>::Node* 
CBTreePage<Trait>::GetFirstObjectInfo()
{
	if( m_SubPages[0] )
		return m_SubPages[0]->GetFirstObjectInfo();
	return m_Keys[0];
}

template <typename value_type, typename LinkedValueType>
void f_Print(KeyNode<value_type, LinkedValueType> &info, size_t level, void *pExtra)
{
       ostream &os = *(ostream *)pExtra;
       for(size_t i = 0; i < level ; i++)
	       os << "\t";
       os << info.getDataRef()<< "-:>" << info.getValueRef() << "\n";
}

// template <typename Trait>
// void CBTreePage<Trait>::Print(ostream & os)
// {
//        //lpfnForEach2 lpfn = &::Print<value_type, LinkedValueType>;
//        lpfnForEach2 lpfn = &::f_Print;
//        ForEach(lpfn, 0, &os);
// }
template <typename Trait>
void CBTreePage<Trait>::Print(ostream &os){
       lpfnForEach<void*> lpfn = &f_Print<value_type, LinkedValueType>;
       ForEachG<void*>(lpfn,0,&os);
}
template <typename Trait>
void CBTreePage<Trait>::Print_Route(BTPage &Obj){
	for(auto i=0;i<=Obj.GetNumberOfKeys();i++){
		if(!Obj.m_SubPages[i]){
			::Print_Route(Obj.m_Keys);
			break;
		}
		else{
			Print_Route(*(Obj.m_SubPages[i]));
		}
	}       
}
template <typename Trait>
void CBTreePage<Trait>::Print_Route(){
	Print_Route(*this);
}

template <typename Trait>
template <typename F, typename...Extras>
void CBTreePage<Trait>::Function_G(F function,Extras... extras){
    lpfnForEach<Extras...> lpfn = function;
    ForEachG<Extras...>(lpfn, 0, extras...);
}
template <typename Trait>
template <typename F, typename...Extras>
void CBTreePage<Trait>::Function_G_Reverse(F function,Extras... extras){
    lpfnForEach<Extras...> lpfn = function;
    ForEachG_Reverse<Extras...>(lpfn, 0, extras...);
}
template <typename Trait>
template <typename Container,typename Obj,typename Root>
void CBTreePage<Trait>::Pointer_Settings(Container ct,Obj cp_t,Root &rt){
      if(rt->m_Heap!= nullptr){
         if(cp_t->getData()>rt->m_Tail->getData())
               rt->m_Tail=Find_End(ct,cp_t);
         if(cp_t->getData()< rt->m_Heap->getData())
               rt->m_Heap=Find_Begin(ct,cp_t);
       }
      else{
         if(rt->m_Heap==rt->m_Tail && rt->m_Heap==nullptr){
            rt->m_Heap=cp_t;
            rt->m_Tail=cp_t;
         }  
      }
}
template <typename Trait>
void CBTreePage<Trait>::Create()
{
       Reset();
       m_Keys.resize(m_MaxKeys+1);
       m_SubPages.resize(m_MaxKeys+2, NULL);
       m_KeyCount = 0;
       m_MinKeys  = 2 * m_MaxKeys/3;
}

template <typename Trait>
void CBTreePage<Trait>::Reset(){
	for( size_t i = 0 ; i < m_KeyCount ; i++ )
	delete m_SubPages[i];
	clear();
}

template <typename Trait>
void CBTreePage<Trait>::clear()
{
       //m_Keys.clear();
       //m_SubPages.clear();
       m_KeyCount = 0;
}

template <typename Trait>
CBTreePage<Trait> * CreateBTreeNode (size_t maxKeys, bool unique)
{
       return new CBTreePage<Trait> (maxKeys, unique);
}

template <typename Trait>
void CBTreePage<Trait>::MovePage(BTPage *pChildPage, vector<Node*> &tmpKeys,vector<BTPage *> &tmpSubPages)
{
	// TODO: #37 change int by size_t
       size_t nKeys = pChildPage->GetNumberOfKeys();
       size_t i = 0;
       for(i = 0; i < nKeys; i++ ){
		   tmpKeys.push_back(pChildPage->m_Keys[i]);
		   tmpSubPages.push_back(pChildPage->m_SubPages[i]);
       }
       tmpSubPages.push_back(pChildPage->m_SubPages[i]);
       pChildPage->clear();
}

// TODO Add opertor<<
template <typename T>
ostream &operator<<(ostream &os, CBTreePage<T> &obj){
    obj.Print(os);
    return os;
}

// TODO Add opertor>>

#endif
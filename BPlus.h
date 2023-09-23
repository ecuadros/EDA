#ifndef __BPLUS_H__
#define __BPLUS_H__

#include <iostream>
#include "btreepage.h"
#include <sstream> 
#include <string>
#include "xtrait.h"
#include <mutex>
namespace BPlusNamespace {
#define DEFAULT_BPLUS_ORDER 3
const size_t MaxHeight = 5; 

template <typename Trait>
class BPlus{
       typedef typename Trait::value_type    value_type;
       typedef typename Trait::LinkedValueType    LinkedValueType;
       typedef CBTreePage <Trait> BTNode;
private:
    mutable std::mutex m_Mutex;       
public:
       typedef typename BTNode::Node  Node;
public:
       BPlus(size_t order = DEFAULT_BPLUS_ORDER, bool unique = true)
              : m_Order(order),
                m_Root(2 * order  + 1, unique),
                m_Unique(unique),
                m_NumKeys(0)
       {
              m_Root.SetMaxKeysForChilds(order);
              m_Height = 1;
       }
       ~BPlus() {}
       bool Insert (const value_type key, const LinkedValueType ObjID);
       bool Remove (const value_type key, const LinkedValueType ObjID);
       LinkedValueType  Search (const value_type key)
       {      std::lock_guard<std::mutex> lock(m_Mutex);
              LinkedValueType ObjID = -1;
              m_Root.Search(key, ObjID);
              return ObjID;
       }
       size_t            size()  { return m_NumKeys; }
       size_t            height() { return m_Height;      }
       size_t            GetOrder() { return m_Order;     }
       BTNode            GetRoot(){ return m_Root;}

       void Write(){m_Root.Write();}
       void Read(const std::string& filename);
       void Print (ostream &os){m_Root.Print(os);}
       void Print_Route (){m_Root.Print_Route();}
       template <typename Func, typename...Extras>
       void Function_G (Func f,Extras... extras){m_Root.Function_G(f,extras...); }
       template <typename Func, typename...Extras>
       void Function_G_Reverse (Func f,Extras... extras){ m_Root.Function_G_Reverse(f,extras...);}
       auto begin(){return m_Root.begin();}
       auto end(){return m_Root.end();}
       auto rbegin(){return m_Root.rbegin();}
       auto rend(){return m_Root.rend();}

protected:
       BTNode          m_Root;
       size_t          m_Height;  // height of tree
       size_t          m_Order;   // order of tree
       size_t          m_NumKeys; // number of keys
       bool            m_Unique;  // Accept the elements only once ?
};     

template <typename Trait>
bool BPlus<Trait>::Insert(const value_type key,const  LinkedValueType ObjID){
    std::lock_guard<std::mutex> lock(m_Mutex);
    bt_ErrorCode error = m_Root.Insert(key, ObjID);
    if( error == bt_duplicate )
            return false;
    m_NumKeys++;
    if( error == bt_overflow ){
        m_Root.SplitRoot();
        m_Height++;
    }
    return true;
}

template <typename Trait>
bool BPlus<Trait>::Remove (const value_type key, const LinkedValueType  ObjID){
       std::lock_guard<std::mutex> lock(m_Mutex);
       bt_ErrorCode error = m_Root.Remove(key, ObjID);
       if( error == bt_duplicate || error == bt_nofound )
               return false;
       m_NumKeys--;

       if( error == bt_rootmerged )
               m_Height--;
       return true;
}

template <typename Trait>
void BPlus<Trait>::Read(const std::string& filename){
    std::lock_guard<std::mutex> lock(m_Mutex);
    std::ifstream is(filename); 
    if (!is.is_open()) { 
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    string tmp_flow,num;
    size_t count;
    value_type  value;
    LinkedValueType kvalue;
    while (getline(is,tmp_flow)) { 
        std::istringstream iss(tmp_flow);
        count = 0;
        while (iss >> num && count < 2) {
            if(count==0)
                value = BTreeNamespace::convertFromString<value_type>(num);
            else
                kvalue = BTreeNamespace::convertFromString<LinkedValueType>(num);  
            count++;
        }
        m_Root.Insert(value,kvalue);
    } 

    is.close();
}

// operator<<
template <typename T>
inline ostream &operator<<(ostream &os, BPlus<T> &obj){
    obj.Print(os);
    return os;
}
//  operator>>
template <typename T>
inline T convertFromString(const std::string &str) {
    std::istringstream iss(str);
    T value;
    iss >> value;
    return value;
}
template <typename T>
inline istream & operator>>(istream &is, BPlus<T> &obj){
    string tmp_flow,num;
    size_t count;
    using vt      = typename T::value_type;
    using lvt     = typename T::LinkedValueType ;
    vt  value;
    lvt kvalue;
    while (getline(is,tmp_flow)) { 
        istringstream iss(tmp_flow);
        count = 0;
        while (iss >> num && count < 2) {
            if(count==0){
                vt num_f = convertFromString<vt>(num);
                value=num_f;
            } 
            else{
                lvt num_f = convertFromString<lvt>(num);
                kvalue=num_f; 
            }      
            count++;
        }
        obj.Insert(value,kvalue);
    }
    return is;    
}

}
#endif
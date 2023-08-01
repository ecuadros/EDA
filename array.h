#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
#include <functional> 
#include <algorithm> // sort algorithm
#include "types.h"
//#include "iterator.h"
#include "keynode.h"
#include "xtrait.h"
using namespace std;

// This class does not have herency...
template<typename Container>
class array_f_i{
    public:
        typedef typename Container::Node Node;
        typedef typename Node::Type      Type;
        typedef array_f_i<Container> myself;
    protected:
        Container *m_pContainer;
        Node      *m_pNode;
        TX      m_dirIter;    
    public:
        array_f_i(Container *pContainer, Node *pNode,TX dirIter)
        : m_pContainer(pContainer), m_pNode(pNode),m_dirIter(dirIter){}
        array_f_i(myself &other,TX dirIter)
        : m_pContainer(other.m_pContainer), m_pNode(other.m_pNode),m_dirIter(dirIter){}
        array_f_i(myself &&other,TX dirIter) {   
            m_pContainer = move(other.m_pContainer);
            m_pNode      = move(other.m_pNode);
            m_dirIter= dirIter;
          }
        bool operator==(myself iter)   { return m_pNode == iter.m_pNode; }
        bool operator!=(myself iter)   { return !(*this == iter);        }
        Type &operator*()              { return m_pNode->getDataRef();}
        array_f_i & operator++() {
            if (m_dirIter == 1){
                 m_pNode++;
                return *this;
            }
            else{
                 m_pNode--;
                return *this;
            }
        }
};

// template <typename Container>
// class array_forward_iterator 
//      : public general_iterator<Container,  class array_forward_iterator<Container> > // 
// {public: 
//     // Se subio al padre  
//     typedef class general_iterator<Container, array_forward_iterator<Container> > Parent; 
//     typedef typename Container::Node           Node; // 
//     typedef array_forward_iterator<Container>  myself;

//   public:
//     array_forward_iterator(Container *pContainer, Node *pNode) 
//             : Parent (pContainer,pNode) {}
//     array_forward_iterator(myself &other)  : Parent (other) {}
//     array_forward_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

// public:
//     array_forward_iterator operator++() { Parent::m_pNode++;  
//                                           return *this;
//                                         }
// };

// template <typename Container>
// class array_backward_iterator 
//      : public general_iterator<Container,  class array_backward_iterator<Container> > // 
// {public: 
//     // Se subio subir al padre  
//     typedef class general_iterator<Container, array_backward_iterator<Container> > Parent; 
//     typedef typename Container::Node           Node; // 
//     typedef array_backward_iterator<Container>  myself;

//   public:
//     array_backward_iterator(Container *pContainer, Node *pNode) 
//             : Parent (pContainer,pNode) {}
//     array_backward_iterator(myself &other)  : Parent (other) {}
//     array_backward_iterator(myself &&other) : Parent(other) {} // Move constructor C++11 en adelante

// public:
//     array_backward_iterator operator++() { Parent::m_pNode--;
//                                           return *this;
//                                         }
// };

using TraitArrayFloatString   = XTrait<float, string>;
using TraitArrayStrinfString  = XTrait<string,string>;
using TraitArrayIntInt        = XTrait<TX,int,std::greater<KeyNode<TX,int>&>>;
using TraitFloatLong          = XTrait<float,long,std::greater<KeyNode<float, long>&>>;

// Created by: @ecuadros
template <typename Traits>
class CArray{
public:
    using value_type      = typename Traits::value_type;
    using KeyType         = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using Node            = typename Traits::Node;
    using CompareFn_X       = typename Traits::CompareFn;
    using CompareFn_Less= typename Traits::CompareFn_Less;
    using CompareFn_Great= typename Traits::CompareFn_Great;
    using myself          = CArray<Traits>;
    using iterator        = array_f_i<myself>;
    //using riterator  = array_backward_iterator<myself>; //riterator means reverse_iterator
private:    
    Node     *m_pVect = nullptr;
    size_t    m_vcount = 0, m_vmax = 0;
    string    m_name = "Empty";
public:
    CArray(): m_name("Empty"){}
    CArray(string name) : m_name(name) {}
    ~CArray(){
        cout << "Destroying " << m_name << "..." << endl;
        destroy();
    }
    void insert(const value_type &key, LinkedValueType value){
        if(m_vcount == m_vmax) // Array is already full?
            resize();
        m_pVect[m_vcount++] = Node(key, value);
        // cout << "Key=" << key << " Value=" << value << "\tinserted, m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
    }
   // This function return a Node by Value
    Node CompareValue(Node& a, Node& b) {
        CompareFn_X tmp;
        if(tmp(a, b))
            return a;
        return b;
    }
    // This function return a boolean
    bool CompareValue_(Node& a, Node& b) {
        CompareFn_X tmp;
        if(tmp(a, b))
            return true;
        return false;
    }
    // This function return a boolean
    bool CompareValue_Less(Node& a, Node& b) {
        CompareFn_Less tmp;
        if(tmp(a, b))
            return true;
        return false;
    }
    // This function return a boolean
    bool CompareValue_Great(Node& a, Node& b) {
        CompareFn_Great tmp;
        if(tmp(a, b))
            return true;
        return false;
    }
    // This function remove the last element and returns it
    Node back(){
        m_vcount=m_vcount-1;
        return m_pVect[m_vcount];
    }
    // This functionremove the last element only
    void pop_back(){
        if(m_vcount!=0){
           m_vcount=m_vcount-1;
        }
    }
    void resize       ();
    void destroy(){
        delete [] m_pVect;
        m_pVect = nullptr;
        m_vcount = 0;
        m_vmax = 0;
    }
    
    void print        (ostream &os){
        // os << "Printing: " << m_name << endl;
        os << m_vcount << " " << m_vmax << endl;
        // sort(m_pVect, m_pVect+m_vcount, CompareFn() );
        for(size_t i = 0; i < m_vcount ; ++i )
            os << m_pVect[i].getData() << "\t: " << m_pVect[i].getValue() << endl;
        //os << "m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
    }
    void read(istream &is){
        //freeing up space if it was already assigned
        destroy();
        // read here By Edson Caceres
        size_t vcount;
        is>>vcount>>m_vmax;
        Node *pTemp = new Node[m_vmax];
        //inserting values from .txt file
        value_type value;
        while(is >> value && size() != vcount){ //keeping in mind the m_vmax
            this->insert(value);
        }
    }

    size_t size()
    {  return m_vcount;    }
    value_type &operator[](size_t pos)
    {   return m_pVect[pos].getDataRef();    }

    Node &GetNode(size_t pos){
        return m_pVect[pos];
    }

    iterator begin() { iterator iter(this, m_pVect,1);    return iter;    }
    iterator end()   { iterator iter(this, m_pVect+m_vcount,1);    return iter;    }
    iterator rbegin() { iterator iter(this, m_pVect+m_vcount-1,-1);     return iter;    }
    iterator rend()   { iterator iter(this, m_pVect-1,-1);   return iter;    }
};

template <typename Traits>
void CArray<Traits>::resize(){
    Node *pTemp = new Node[m_vmax+10];
    for(size_t i = 0 ; i < m_vcount ; ++i)
        pTemp[i]   = m_pVect[i];
        // *(pTemp+i) = m_pVect[i];
        // pTemp[i]   = *(m_pVect+i);
        // *(pTemp+i) = *(m_pVect+i);
        // *(i+pTemp) = m_pVect[i];
        // i[pTemp]   = m_pVect[i];
    delete [] m_pVect;
    m_pVect = pTemp;
    m_vmax +=10;
    // cout << "Vector resized m_vcount=" << m_vcount << " m_vmax=" << m_vmax << endl;
}

template <typename T>
ostream &operator<<(ostream &os, CArray<T> &obj){
    obj.print(os);
    return os;
}

//Operator was built
template <typename T>
T convertFromString(const string& str) {
    T rs;
    stringstream ss(str);
    ss >> rs;
    return rs;
}
template <typename T>
istream & operator>>(istream &is, CArray<T> &obj){
    string tmp_flow,num;
    size_t count;
    using vt      = typename T::value_type;
    using lvt     = typename T::LinkedValueType;
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
        obj.insert(value,kvalue);
    }
    return is;    
}



#endif // __ARRAY_H__

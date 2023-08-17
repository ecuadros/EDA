#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <iostream>
#include <algorithm> 
#include "types.h"
#include "iterator.h"
#include "keynode.h"
#include "xtrait.h"
using namespace std;

template <typename Container>
class array_forward_iterator 
     : public general_iterator<Container,  class array_forward_iterator<Container> > 
{public:
     
    typedef class general_iterator<Container, array_forward_iterator<Container> > Parent; 
    typedef typename Container::Node           Node; 
    typedef array_forward_iterator<Container>  myself;

  public:
    array_forward_iterator(Container *pContainer, Node *pNode) 
            : Parent (pContainer,pNode) {}
    array_forward_iterator(myself &other)  : Parent (other) {}
    array_forward_iterator(myself &&other) : Parent(other) {} 

public:
    array_forward_iterator operator++() { Parent::m_pNode++;  
                                          return *this;
                                        }
};

template <typename Container>
class array_backward_iterator 
     : public general_iterator<Container,  class array_backward_iterator<Container> > // 
{public: 
   
    typedef class general_iterator<Container, array_backward_iterator<Container> > Parent; 
    typedef typename Container::Node           Node; // 
    typedef array_backward_iterator<Container>  myself;

  public:
    array_backward_iterator(Container *pContainer, Node *pNode) 
            : Parent (pContainer,pNode) {}
    array_backward_iterator(myself &other)  : Parent (other) {}
    array_backward_iterator(myself &&other) : Parent(other) {} 

public:
    array_backward_iterator operator++() { Parent::m_pNode--;
                                          return *this;
                                        }
};

using TraitArrayFloatString = XTrait<float, string>;
using TraitArrayIntInt      = XTrait<TX  , int   , std::greater<KeyNode<TX  , int > &>>;
using TraitFloatLong        = XTrait<float, long  , std::greater<KeyNode<float, long> &>>;

template <typename Traits>
class CArray{
public:
    using value_type      = typename Traits::value_type;
    using KeyType         = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using Node      = typename Traits::Node;
    using CompareFn = typename Traits::CompareFn;
    using myself    = CArray<Traits>;
    using iterator  = array_forward_iterator<myself>;
    using riterator  = array_backward_iterator<myself>; 
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
        if(m_vcount == m_vmax) 
            resize();
        m_pVect[m_vcount++] = Node(key, value);
        
}

template <typename T>
ostream &operator<<(ostream &os, CArray<T> &obj){
    obj.print(os);
    return os;
}

template <typename T>
istream & operator>>(istream &is, CArray<T> &obj){

    return is;
}


#endif 

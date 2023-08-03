#ifndef __HEAP_H__
#define __HEAP_H__

#include <iostream>
#include <algorithm> // swap algorithm
#include <cassert>
#include "types.h"
#include "keynode.h"
#include "xtrait.h"
#include "array.h"

using namespace std;

using XTraitFloatStringDesc = XTrait<float, string, std::less<KeyNode<int, int > &>>;
using XTraitIntIntMax       = XTrait<int  , int   , std::greater<KeyNode<int, int > &>>;
using XTraitIntIntMin      = XTrait<int , int,std::less<KeyNode<int, int > &>>;
// Created by: @ecuadros
template <typename Traits>
class CHeap{
public:
    using value_type      = typename Traits::value_type;
    using KeyType         = typename Traits::value_type;
    using LinkedValueType = typename Traits::LinkedValueType;
    using Node            = typename Traits::Node;
    using CompareFn       = typename Traits::CompareFn;
    using myself          = CHeap<Traits>;
private:
    CArray<Traits>    m_heap;
    string    m_name = "Empty";
    CompareFn  Fn;
public:
    CHeap(string name)  : m_name(name){ destroy();  }
    CHeap()                           { destroy();  }
    virtual ~CHeap(){
        cerr << "Destroying " << m_name << "..." << endl;
        //reset();
        destroy();
    }
    void destroy(){
        m_heap.destroy();
        m_heap.insert(0, 0);
    }

    // CHeap operator=(CHeap &other){
    //     destroy();
        
    // }
    
    void insert(const value_type &key, LinkedValueType value){
        m_heap.insert(key, value);
        heapifyAsc();
        
    }

    // TODO: complete heapifyAsc function (useful for insertion)
    void heapifyAsc(){
        
        size_t son=m_heap.size()-1;
        while (son>1){
            size_t parent=son/2;
                if(Fn(m_heap[son],m_heap[parent]))
            {   swap(m_heap[parent],m_heap[son]);
                son=parent;
            
            }
            else {
            break; 
        }
        }                    
}
// TODO: complete heapifyDesc function (useful when we remove an element)
void auxDesc(size_t parent){
    size_t son_one=2*parent;
    size_t son_two=2*parent+1;
    if (son_two<m_heap.size()){
        if(Fn(m_heap[son_one],m_heap[son_two])){
        if(!Fn(m_heap[parent],m_heap[son_one])) {
            swap(m_heap[parent],m_heap[son_one]);
        }
    }
    else{
        if(!Fn(m_heap[parent],m_heap[son_two])) {
            swap(m_heap[parent],m_heap[son_two]);
        }
    }

    }
    else{
        if(!Fn(m_heap[parent],m_heap[son_one])) {
            swap(m_heap[parent],m_heap[son_one]);
        }
    }
    
    
}
     void heapifyDesc(){
            size_t n=1;
            while (n!=3){
                auxDesc(n);
                n++;
            }

     }

    Node pop(){
        assert(m_heap.size() > 1);
        swap(m_heap[1], m_heap[m_heap.size()-1]);
        m_heap.pop_back();
       heapifyDesc();
    }

    void print        (ostream &os){
        os << m_heap;
    }
    void read(istream &is){
        is >> m_heap;
    }

    size_t size()
    {  return m_heap.size();    }

    // TODO : agregar el operator value_type &
   // value_type &operator[](size_t pos)
    //{   return m_heap[pos].getDataRef();    }


};

#endif

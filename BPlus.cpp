#ifndef __BPLUS_H__
#define __BPLUS_H__

#include <iostream>
#include <algorithm> // swap algorithm
#include <cassert>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "keynode.h"
#include "xtrait.h"
#include "array.h"

using namespace std;

// using XTraitFloatStringDesc = XTraitTrait<float, string, std::less<KeyNode<int, int > &>>;>;
// using XTraitIntIntAsc       = XTraitTrait<int  , int   , std::greater<KeyNode<int, int > &>>;

// Created by: @ecuadros
template <typename Traits>
class CBPlus{
public:
    // using value_type      = typename Traits::value_type;
    // using KeyType         = typename Traits::value_type;
    // using LinkedValueType = typename Traits::LinkedValueType;
    // using Node            = typename Traits::Node;
    // using CompareFn       = typename Traits::CompareFn;
    // using myself          = CBPlus<Traits>;
private:
    
    string    m_name = "Empty";
public:
    CBPlus(string name)  : m_name(name){ destroy();  }
    CBPlus()                           { destroy();  }
    virtual ~CBPlus(){
        cerr << "Destroying " << m_name << "..." << endl;
        reset();
    }
    void destroy(){
        
    }

    // CBPlus operator=(CBPlus &other){
    //     destroy();
        
    // }
    
    void insert(const value_type &key, LinkedValueType value){
    }

    void print        (ostream &os){
        
    }
    void read(istream &is){
        
    }

    size_t size()
    {  return 0;    }

    // TODO : agregar el operator value_type &
    // value_type &operator[](size_t pos)
    // {   return m_heap[pos].getDataRef();    }
};

#endif

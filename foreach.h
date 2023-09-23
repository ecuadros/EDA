#ifndef __RECORRRER_H__  
#define __RECORRRER_H__ 
//#include "object_function.h"
#include <functional>
using namespace std;

template <typename T>
void f2(T &x)
{  cout << x << "  "; }

template <typename T>
void f1(T &x)
{  x+= 5; }

template <typename Iterator, typename F>
void foreach(Iterator ItBegin, Iterator ItEnd, F ope)
{
  for(auto& iter = ItBegin; iter != ItEnd ; ++iter){
        // cout<< *iter;
      ope(*iter);
  }
}

template <typename Iterator, typename F>
typename Iterator::Type* firstThat(Iterator ItBegin, Iterator ItEnd, F ope)
{
  for(auto& iter = ItBegin; iter != ItEnd ; ++iter){
    if(ope(*iter)) return &*iter;
  }
  return nullptr;
}

// Variadic templates
template <typename Iterator, typename Callable, typename... Args>
typename Iterator::Type* firstThat(Iterator ItBegin, Iterator ItEnd, Callable op, Args&&... args)
{
    // cout<<"asdf";
    for (auto& iter = ItBegin; iter != ItEnd; ++iter)
    {
        if(op(*iter, args...)) return &*iter;
    }
    return nullptr;
}


// Variadic templates
template <typename Iterator, typename Callable, typename... Args>
void foreach(Iterator ItBegin, Iterator ItEnd, Callable op, Args&&... args)
{
    for (auto& iter = ItBegin; iter != ItEnd; ++iter)
    {
        op(*iter, args...);
    }
}

// #1
template <typename Container, typename F>
void foreach(Container &container, F ope){
    foreach(container.begin(), container.end() , ope);
}

// DONE: Foreach inverso
template <typename Container, typename F>
void foreach_reverse(Container &container, F ope){
    foreach(container.rbegin(), container.rend() , ope);
}

// template <typename Iterator, typename Callable, typename... Args>
// void foreach(Iterator ItBegin, Iterator ItEnd, Callable op, Args&&... args)
// {   foreach(container.begin(), container.end(), ope);
// }

// DONE implementar el foreach inverso
// # 2
// template <typename Container, typename F>
// void foreach_inverso(Container &container, F ope)
// { foreach(container.rbegin(), container.rend(), ope);
// }

// template <typename Container>
// void foreach(Container &container)
// {
//     using T = typename Container::value_type;
    // using T = typename Container::value_type;
    // foreach(container, print<T>);  cout << endl; // recorre imprimiendo
    // foreach(container, inc<T>);  // recorre incrementando
    // foreach(container, print<T>);  cout << endl; // recorre imprimiendo

//     // funciones lambda
//     foreach(container, [](T &n){ n-= 5;}); cout << endl; // -5 a cada uno
//     foreach(container, print<T>);  cout << endl; // recorre imprimiendo
    
//     OperacionEspecial<T> ope; 
//     foreach(container, ope);  
//     foreach(container, print<T>);  cout << endl;
// }

#endif

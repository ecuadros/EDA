#ifndef __RECORRRER_H__  
#define __RECORRRER_H__ 
//#include "object_function.h"
#include <functional>

//using namespace std;

template <typename T>
void f2(T &x)
{  cout << x << "  "; }

template <typename T>
void f1(T &x)
{  x+= 5; }

template <typename Iterator, typename F>
void foreach(Iterator ItBegin, Iterator ItEnd, F ope)
{
  auto iter = ItBegin;
  for(; iter != ItEnd ; ++iter)
      ope(*iter);
}

template <typename Container, typename F>
void foreach(Container &container, F ope){
    foreach(container. begin(), container.end() , ope);
}

template <typename Container, typename F>
void foreach_reverse(Container &container, F ope){
    foreach(container. rbegin(), container.rend() , ope);
}

// =============== Variadic Function==============

// # 1
template <typename Iterator, typename F, typename... Args>
void ForeachOne(Iterator ItBegin, Iterator ItEnd, F ope, Args&&... args)
{
    auto iter = ItBegin;
    for(; iter != ItEnd ; ++iter)
       invoke(ope, *iter, forward<Args>(args)...);
}

// # 2
template <typename Container, typename F, typename... Args>

void ForeachTwo( Container container,  F ope, Args&&... args)
	{   
        ForeachOne(container.begin(), container.end(),  ope, args...);
 	}

#endif


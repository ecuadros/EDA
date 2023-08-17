#ifndef __RECORRRER_H__
#define __RECORRRER_H__

#include <iostream>

template <typename T>
void f2(T& x)
{
    std::cout << x << "  ";
}

template <typename T>
void f1(T& x)
{
    x += 5;
}

template <typename Iterator, typename F>
void foreach(Iterator ItBegin, Iterator ItEnd, F ope)
{
    auto iter = ItBegin;
    for (; iter != ItEnd; ++iter)
        ope(*iter);
}

template <typename Container, typename F>
void foreach(Container& container, F ope)
{
    foreach(container.begin(), container.end(), ope);
}

#endif
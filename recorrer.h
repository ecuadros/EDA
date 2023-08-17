#ifndef __RECORRRER_H__
#define __RECORRRER_H__
//#include "object_function.h"
using namespace std;

template <typename T>
void f2(T &x)
{ 
    cout << x << "  "; 
}

template <typename T>
void f1(T &x)
{ 
    x += 5; 
}

template <typename Iterator, typename F>
void recorrer(Iterator ItBegin, Iterator ItEnd, F ope)
{
    auto iter = ItBegin;
    for (; iter != ItEnd; ++iter)
        ope(*iter);
}

// #1
template <typename Container, typename F>
void recorrer(Container &container, F ope)
{
    recorrer(container.begin(), container.end(), ope);
}

// # 2
template <typename Container, typename F>
void recorrer_inverso(Container &container, F ope)
{
    recorrer(container.rbegin(), container.rend(), ope);
}

template <typename Container>
void recorrer(Container &container)
{
    using T = typename Container::value_type;

    // recorre imprimiendo
    recorrer(container, [](T &n) { cout << n << " "; });
    cout << endl;

    // recorre incrementando
    recorrer(container, [](T &n) { n += 5; });

    // recorre imprimiendo
    recorrer(container, [](T &n) { cout << n << " "; });
    cout << endl;

    // -5 a todos
    recorrer(container, [](T &n) { n -= 5; });
    // recorre imprimiendo
    recorrer(container, [](T &n) { cout << n << " "; });
    cout << endl;

    // OperacionEspecial<T> ope;
    // recorrer(container, ope);
    // recorre imprimiendo
    recorrer(container, [](T &n) { cout << n << " "; });
    cout << endl;
}

#endif
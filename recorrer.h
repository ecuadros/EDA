#ifndef __RECORRRER_H__  
#define __RECORRRER_H__ 
//#include "object_function.h"
using namespace std;

template <typename T>
void f2(T &x)
{  cout << x << "  "; }

template <typename T>
void f1(T &x)
{  x+= 5; }

template <typename Iterator, typename F>
void recorrer(Iterator ItBegin, Iterator ItEnd, F ope)
{
  auto iter = ItBegin;
  for(; iter != ItEnd ; ++iter)
      ope(*iter);
}

// #1
template <typename Container, typename F>
void recorrer(Container &container, F ope)
{  
    recorrer(container.begin(), container.end(), ope);
}


//=======================================================

// TODO implementar el recorrer inverso
// # 2
// Se declara la sobrecarga de recorrer_inverso en el que se le da
// la dirección del ultimo elemento del contenedor y va recorriendo
// de posición en posición hasta el primero. Para realizar esta lógica
// se ha sobrecargado el operador >= (en array.h, línea 38) y el 
// operador -- // (en array.h, línea 44-48). 

template <typename Iterator, typename F>
void recorrer_inverso(Iterator ItEnd, Iterator ItBegin, F ope)
{
  auto iter = ItEnd;
  for(auto iter = ItEnd; iter >= ItBegin; --iter)
      ope(*iter);
}

// Se declara recorrer_inverso dándole como parámetro el container y 
// el operador. Internamente usa la dirección del último valor del contenedor,
// la dirección del primer valor del contenedor y la operación. 
template <typename Container, typename F>
void recorrer_inverso(Container &container, F ope)
{

    recorrer_inverso(container.last(), container.begin(), ope);
}
//========================================================

template <typename Container>
void recorrer(Container &container)
{
    using T = typename Container::value_type;
    // using T = typename Container::value_type;
    // recorrer(container, print<T>);  cout << endl; // recorre imprimiendo
    // recorrer(container, inc<T>);  // recorre incrementando
    // recorrer(container, print<T>);  cout << endl; // recorre imprimiendo

//     // funciones lambda
//     recorrer(container, [](T &n){ n-= 5;}); cout << endl; // -5 a todos
//     recorrer(container, print<T>);  cout << endl; // recorre imprimiendo
    
//     OperacionEspecial<T> ope; 
//     recorrer(container, ope);  
//     recorrer(container, print<T>);  cout << endl;
}

#endif

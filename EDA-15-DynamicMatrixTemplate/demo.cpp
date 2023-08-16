#include <iostream>
#include <fstream>
#include <cmath>
#include "demo.h"
#include "array.h"
#include "matrix.h"
#include "recorrer.h"
using namespace std;

template <typename T, int N>
void increment(T &x)
{
    x += N;
}

template <typename T>
void print(T &x)
{
    cout << x << "  ";
}

// Object function
template <typename T>
class ClassX
{
    int m_inc = 0;

public:
    ClassX(int n) : m_inc(n) {}
    void operator()(T &n)
    {
        n += m_inc;
    }
};

void Fx1(int n) { n++; }
void Fx2(int &n) { n++; }
void Fx3(int *pi) { ++*pi; pi = nullptr; }
void Fx4(int *&rp) { ++*rp; rp = nullptr; }

void DemoBasicPointers()
{
    int i = 10, j = 20, &r = i;
    int *b /*Peligro*/, *p = nullptr, *q = nullptr, **pp = nullptr;
    p = &i;
    q = &j;
    pp = &p;
    float f = 3.14;
    cout << "***** Fx1 *****" << endl;
    Fx1(i);
    cout << i << endl; //  10
    Fx1(15);
    Fx1(*p);
    cout << i << endl; //  10
    Fx1(**pp);
    cout << i << endl; //  10
    Fx1(r);
    cout << i << endl; //  10

    cout << "***** Fx2 *****" << endl;
    i = 10; // r = 10;
    Fx2(i);
    cout << i << endl; // 11
    Fx2(20);
    Fx2(i + 5);
    Fx2(i + j);
    Fx2(f);
    Fx2(r);
    cout << i << endl; // 12

    cout << "***** Fx3 *****" << endl;
    **pp = 10; // *p = 10;     i = 10;
    *q = 20;   //  j = 20;
    Fx3(p);
    cout << i << endl; // 11
    Fx3(*pp);
    cout << i << endl; // 12
    Fx3(&i);
    cout << i << endl; // 13
    Fx3(q);
    cout << j << endl; // 21
    Fx3(&j);
    cout << j << endl; // 22

    cout << "***** Fx4 *****" << endl;
    p = &i;
    q = &j;
    pp = &p;
    **pp = 50; // *p = 10;     i = 10;
    *q = 60;   //  j = 20;
    Fx4(p);
    cout << i << " p: :" << p << endl; // 51
    p = &i;
    Fx4(*pp);
    cout << i << " p: :" << p << endl; // 52, p: 0x0
    Fx4(&i); // Error! es un valor
    Fx4(q);
    cout << j << " q: :" << q << endl; // 61 q: 0x0
}

void DemoSmartPointers()
{
    CArray<TraitArrayIntInt> v2("Lucero"), *pX;

    shared_ptr<CArray<TraitArrayFloatString>> pV3(new CArray<TraitArrayFloatString>("Luis")), pV4;
    pV4 = pV3;
    auto &rA = *pV3;
    for (auto i = 100; i < 112; i++)
    {
        v2.insert(i, i * i);
        pV3->insert(sqrt(i), string("**") + to_string(sqrt(i) + 5) + string("**"));
    }
    cout << "Printing pV3 float -> string (greater)" << endl;
    cout << *pV3;
}

void DemoDynamicMatrixes()
{

    CMatrix<MatrixTraitFloat> mat1(3, 4);
    mat1.fill(1);
    cout << mat1;

    CMatrix<MatrixTraitFloat> mat2(4, 5);
    mat2.fill(2.5);
    cout << mat2;

template <typename T>
class CMatrix
{
    // Resto del código de la clase CMatrix

public:
    // ...

    // Sobrecarga del operador *
    CMatrix<T> operator*(const CMatrix<T> &other) const
    {
        // Verificar si las dimensiones de las matrices son compatibles para realizar la multiplicación
        if (m_cols != other.m_rows)
        {
            throw std::runtime_error("Incompatible matrix dimensions for multiplication");
        }

        // Crear una nueva matriz para almacenar el resultado de la multiplicación
        CMatrix<T> result(m_rows, other.m_cols);

        // Realizar la multiplicación de las matrices
        for (int i = 0; i < m_rows; i++)
        {
            for (int j = 0; j < other.m_cols; j++)
            {
                T sum = 0;
                for (int k = 0; k < m_cols; k++)
                {
                    sum += m_data[i][k] * other.m_data[k][j];
                }
                result.m_data[i][j] = sum;
            }
        }

        return result;
    }

    // ...
};
#include <iostream> // cout
#include <fstream>  // ofstream, ifstream
#include <cmath>
#include "demo.h"
#include "array.h"
using namespace std;

void DemoSmartPointers(){
    CArray<int> v2("Lucero"), *pX; //, *pV3 = new CArray("Luis");
    shared_ptr< CArray<float> > pV3(new CArray<float>("Luis")), pV4;
    pV4 = pV3;
    auto &rA = *pV3;
    for(auto i = 100 ; i < 112 ; i++)
    {   v2.insert(i);
        pV3->insert(sqrt(i));
        //  (*pv3).insert(i);
        //  rA.insert(i);
    }
}

void DemoArray(){   
    cout << "Hello from DemoArray()" <<endl;
    cout << "Vector #1()" <<endl;
    CArray<int> v1("Antonio"); 
    for(auto i = 0 ; i < 15 ; i++)
        v1.insert(i);   //  insert(&v1);

    cout << "Vector #2()" <<endl;
    CArray<float> v2("Cristian Vera"), *pV3 = new CArray<float>("Guiomar ABC");
    auto &rA = *pV3;
    for(auto i = 100 ; i < 112 ; i++)
    {   v2.insert(sqrt(i));
        pV3->insert(i);
        //  (*pv3).insert(i);
        //  rA.insert(i);
    }
    cout << v1; // v1.print(cout);
    ostream &tmp = cout << v2 << "More text" << endl;
    tmp << "Hola !!!" << endl;
    cout << &tmp << "..." << &cout <<endl;
    // cout << x << f << y << endl;
    pV3->print(cout);
    // (*pV3).print();     *pV3 is already an object
    // rA.print();          rA is also an object
    // pV3[0].print();      pV3 is also an array with just 1 element [0]
    // (*(pV3+0)).print();
    // (*(0+pV3)).print();
    // 0[pV3].print();
    // delete pV3;

    // Using an array with []
    for(auto i = 0 ; i < v2.size() ; i++)
        cout << "v2[" << i << "] = " << v2[i] << endl;
    ofstream of("test.txt", ios::out);
    of << v2 << endl; 
}

void DemoBinaryTree()
{
    cout << "Hello from DemoBinaryTree()" <<endl;
}

void DemoHash()
{
    cout << "Hello from DemoHash()" <<endl;
}

//=====================================================
void DemoLecturaDeTxt()
{
    CArray <int> enteros("Numeros Enteros");
    CArray <float> flotantes("Numeros Flotantes");
    CArray <char> letras("Letras");

    //Se generan diferentes tipos de CArrays
    for(auto i = 10; i<= 100; i+=10)
        enteros.insert(i);

    for(auto i = 1; i<= 15; i++)
        flotantes.insert(0.1*i);

    for(auto i = 0; i< 26; i++)
        letras.insert('a'+i);


    //Se guarda los datos del objeto creado en un archivo txt
    ofstream of1("testEnteros.txt", ios::out);
    of1 << enteros << endl; of1.close();
    ofstream of2("testFlotantes.txt", ios::out);
    of2 << flotantes << endl; of2.close();
    ofstream of3("testLetras.txt", ios::out);
    of3 << letras << endl; of3.close();

    cout << endl;
    cout<< "***Se guarda el Array de Enteros de 10 elementos (10, 20, 30 ... 90, 100) en testEnteros.txt***" << endl;
    cout<< "***Se guarda el Array de Flotantes de 15 elementos (0.1, 0.2, 0.3 ... 1.4, 1.5) en testFlotantes.txt***" << endl;  
    cout<< "***Se guarda el Array de Char de 25 elementos (a, b, c ... y, z) en testLetras.txt***" << endl << endl;


    CArray <int> enterosLeidos("Enteros Leidos");
    CArray <float> flotantesLeidos("Flotantes Leidos");
    CArray <char> letrasLeidas("Letras Leidas");
    ifstream file1, file2, file3;

    cout << "Se lee e imprime el Array de Enteros" << endl;
    file1.open("testEnteros.txt");
    file1 >> enterosLeidos;
    cout<< enterosLeidos << endl;

    cout << "Se lee e imprime el Array de Flotantes" << endl;
    file2.open("testFlotantes.txt");
    file2 >> flotantesLeidos;
    cout<< flotantesLeidos << endl;

    cout << "Se lee e imprime el Array de Caracteres" << endl;
    file3.open("testLetras.txt");
    file3 >> letrasLeidas;
    cout<< letrasLeidas;

}
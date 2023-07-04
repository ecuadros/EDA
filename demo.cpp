#include <iostream> // cout
#include <fstream>  // ofstream, ifstream
#include <cmath>
#include "demo.h"
#include "array.h"
#include <memory>
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

// Read txt and setting up to object

void DemoInputStream() {

    CArray<float> *arr = new CArray<float>("Pierr Daniel Chino Lurita");

    ifstream is("test.txt", ios::in);

    // [BEGIN] Read the size of characters from the .txt
    
    is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);
    cout << "Reading " << length << " characters... " << endl;

    // [END] Read the size of characters from the .txt

    is >> *arr;

    arr->print(cout);

    // arr->insert(11.3);

    // arr->print(cout);
}

void DemoBinaryTree()
{
    cout << "Hello from DemoBinaryTree()" <<endl;
}

void DemoHash()
{
    cout << "Hello from DemoHash()" <<endl;
}


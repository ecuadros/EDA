#include <iostream> // cout
#include <fstream>  // ofstream, ifstream
#include "demo.h"
#include "array.h"
using namespace std;

void DemoSmartPointers(){
    CArray v2("Lucero"), *pX; //, *pV3 = new CArray("Luis");
    shared_ptr<CArray> pV3(new CArray("Luis")), pV4;
    pV4 = pV3;
    CArray &rA = *pV3;
    for(auto i = 0 ; i < 12 ; i++)
    {   v2.insert(i);
        pV3->insert(i);
        //  (*pv3).insert(i);
        //  rA.insert(i);
    }
}
void DemoArray(){   
    cout << "Hello from DemoArray()" <<endl;
    cout << "Vector #1()" <<endl;
    CArray v1("Antonio"); 
    for(auto i = 0 ; i < 15 ; i++)
        v1.insert(i);   //  insert(&v1);

    cout << "Vector #2()" <<endl;
    CArray v2("Cristian Vera"), *pV3 = new CArray("Guiomar ABC");
    CArray &rA = *pV3;
    for(auto i = 0 ; i < 12 ; i++)
    {   v2.insert(i);
        pV3->insert(i);
        //  (*pv3).insert(i);
        //  rA.insert(i);
    }
    
    cout << v1; // v1.print(cout);
    ostream &tmp = cout << v2 << "More text" << endl;
    tmp << "Hola !!!" << endl;
    cout << &tmp << "..." << &cout <<endl;
    // cout << x << f << y << endl;
    ofstream of("test.txt", ios::out);
    of << v2 << "Extra text here" << endl;
    pV3->print(cout);
    // (*pV3).print();     *pV3 is already an object
    // rA.print();          rA is also an object
    // pV3[0].print();      pV3 is also an array with just 1 element [0]
    // (*(pV3+0)).print();
    // (*(0+pV3)).print();
    // 0[pV3].print();
    // delete pV3;
    // Test of Cin
    cout<<"Print Value of V2"<<endl;
    v2.print(cout);
    cout<<"Leyendo file"<<endl;
    ifstream ifx("file.txt",ios::in);
    ifx>>v2;
    cout<<"Print Value of V2 Update"<<endl;
    v2.print(cout);

}

void DemoBinaryTree()
{
    cout << "Hello from DemoBinaryTree()" <<endl;
}

void DemoHash()
{
    cout << "Hello from DemoHash()" <<endl;
}


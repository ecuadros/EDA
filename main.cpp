#include <iostream>
#include "demo.h"
// #include "array.h"

using namespace std;

// g++ -pthread -std=c++17   -o main main.cpp demo.cpp
// g++ -pthread -std=c++20   -o main main.cpp demo.cpp
// g++ -pthread -std=c++2b   -o main main.cpp demo.cpp
// g++ -pthread -std=gnu++2b -o main main.cpp demo.cpp
int main()
{
    // DemoParameters();
    // DemoArray();
    // DemoIterators();
    // DemoReverseIterators();
    // DemoDynamicMatrixes();

    // DemoHeap();
    cout<<"-------------------------------------------------------" << endl;
    cout<<"                    Linked List                        " << endl;
    cout<<"-------------------------------------------------------" << endl;
    DemoLinkedList();
    cout<<"-------------------------------------------------------" << endl;
    cout<<"                 Double Linked List                    " << endl;
    cout<<"-------------------------------------------------------" << endl;
    DemoDoubleLinkedList();
    // DemoBinaryTree();
    // DemoHash();

    // DemoPreandPostIncrement();
    // DemoSmartPointers();
    
    return 0;
}
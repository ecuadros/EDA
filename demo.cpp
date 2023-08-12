#ifndef __DEMO_H__
#define __DEMO_H__

void DemoArray();
void DemoBinaryTree();
void DemoHash();

#endif // __DEMO_H__

#include <iostream> // cout
#include "demo.h"
#include "heap.h"
using namespace std;

void DemoArray()
{
    cout << "Hello from DemoArray()" <<endl;
}

void DemoBinaryTree()
{
    cout << "Hello from DemoBinaryTree()" <<endl;
}

void DemoHash()
{
    cout << "Hello from DemoHash()" <<endl;
}

void demoHeap()
{
    cout << "This is a demo of the heap data structure." <<endl;
    CHeap<XTraitIntIntAsc> h;
    h.insert(5, "Hello");
    h.insert(10, "World");
    h.insert(2, "!");
    cout << h[1] << endl; // Prints "5"
    cout << h.pop().getData() << endl; // Prints "2"
    cout << h.size() << endl; // Prints "2"
}

int main() {
    DemoArray();
    DemoBinaryTree();
    DemoHash();
    demoHeap();
    return 0;
}

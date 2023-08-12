#ifndef __DEMO_H__
#define __DEMO_H__

void DemoArray();
void DemoBinaryTree();
void DemoHash();

#endif 

#include <iostream> 
#include "demo.h"
#include "heap.h"
using namespace std;

int main() {
    DemoArray();
    DemoBinaryTree();
    DemoHash();
    CHeap<XTraitIntIntAsc> h;
    h.insert(5, "Hello");
    h.insert(10, "World");
    h.insert(2, "!");
    cout << h[1] << endl; 
    cout << h.pop().getData() << endl; 
    cout << h.size() << endl; 
    return 0;
}

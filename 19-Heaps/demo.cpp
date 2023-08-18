#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "demo.h"
#include "array.h"
#include "matrix.h"
#include "keynode.h"
#include "xtrait.h"
#include "foreach.h"
#include "heap.h"

using namespace std;

template <typename T, int N>
void DemoArray() {
    ofstream output("demoarray.txt");
    output << "DemoArray()" << endl;
    output << "-----------------------------------" << endl;
    CArray<T> v1("Luceropalomino");
    CArray<T> v2("Luceropalomino");
    output << v1 << endl;
    output << v2 << endl;
    cout << "DemoArray finished!" << endl;

    using TraitStringString = ArrayTrait<string, string, std::less<NodeArray<string, string>&>>;
    using TraitStringString = XTrait<string, string, std::less<KeyNode<string, string>&>>;
    CArray<TraitStringString> vx("Luceropalomino");
    vx.insert("Luceropalomino", "Luceropalomino");
    vx.insert("Luceropalomino", "Luceropalomino");
    vx.insert("Luceropalomino", "Luceropalomino");
    output << vx << endl;
    cout << "CArray TraitStringString:" << endl;
    for (auto x : vx) {
        cout << x << endl;
    }
}

void DemoReverseIterators() {
    // Código de demostración de iteradores inversos
}

void DemoHeap() {
    cout << "Hello from DemoHeap()" << endl;
    cout << "Hello from DemoHeap()" << endl << endl;

    cout << "--------------------------------Max-Heap---------------------------------" << endl;
    CHeap<XTraitIntIntAsc> maxHeap("Luceropalomino");
    maxHeap.insert(20, 1);
    maxHeap.insert(50, 1);
    maxHeap.insert(14, 1);
    maxHeap.insert(18, 1);
    maxHeap.insert(500, 1);
    maxHeap.insert(24, 1);
    maxHeap.insert(1, 1);
    maxHeap.insert(1000, 1);
    maxHeap.sort();
    cout << "Max-Heap before pop_back()" << endl;
    maxHeap.print(cout);
    cout << "back element: " << maxHeap.pop() << endl;
    cout << "Max-Heap after pop_back()" << endl;
    maxHeap.print(cout);

    cout << "--------------------------------Min-Heap---------------------------------" << endl;
    CHeap<XTraitIntIntDesc> minHeap("Luceropalomino");
    minHeap.insert(1.2, 1);
    minHeap.insert(12.7, 1);
    minHeap.insert(0.9, 1);
    minHeap.insert(0.1, 1);
    minHeap.insert(500, 1);
    minHeap.insert(24, 1);
    minHeap.insert(15.2, 1);
    minHeap.insert(100, 1);
    minHeap.sort();
    cout << "Min-Heap before pop_back()" << endl;
    minHeap.print(cout);
    cout << "back element: " << minHeap.pop() << endl;
    cout << "Min-Heap after pop_back()" << endl;
    minHeap.print(cout);
}

void DemoBinaryTree() {
    // Código de demostración de árbol binario
}

int main() {
    DemoArray<int, 5>();
    DemoReverseIterators();
    DemoHeap();
    DemoBinaryTree();

    return 0;
}
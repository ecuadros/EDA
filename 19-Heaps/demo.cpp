#include "demo.h"
#include "array.h"
#include "matrix.h"
#include "keynode.h"
#include "xtrait.h"
#include "foreach.h"
#include "heap.h"
using namespace std;

template <typename T, int N>
void DemoArray()
{
    ofstream of("result.txt");
    Array<T, N> v1;
    for (int i = 0; i < N; ++i)
    {
        v1[i] = i;
    }
    Array<T, N> v2 = v1;
    v2[0] = 100;
    of << v1 << endl;
    of << v2 << endl;
    cout << "DemoArray finished!" << endl;

    using TraitStringString = Trait<XNode<string, string>, greater<NodeArray<string, string>>>;
    Array<TraitStringString> vx("Lucero Palomino");
    vx.insert("John", "Doe");
    vx.insert("Jane", "Smith");
}

void DemoReverseIterators()
{
    // Código de demostración de iteradores inversos
}

void DemoHeap()
{
    cout << "Hello from DemoHeap()" << endl;
    cout << "Hello from DemoHeap()" << endl << endl;

    cout << "--------------------------------Max-Heap---------------------------------" << endl;
    Heap<TraitIntIntAsc> maxHeap("Lucero Palomino");
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
    Heap<TraitIntIntDesc> minHeap("Lucero Palomino");
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

void DemoBinaryTree()
{
    // Código de demostración del árbol binario
}
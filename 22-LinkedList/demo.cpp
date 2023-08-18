#include "array.h"
#include "matrix.h"
#include "foreach.h"
#include "linkedlist.h"
#include "doublelinkedlist.h"
#include <fstream>
using namespace std;

template <typename T, int N>
void DemoArray() {
    ofstream of("array.txt");
    T v1[N];
    for (auto i = 0; i < N; i++) {
        v1[i] = i * 2;
        of << v1[i] << endl;
    }
    Array<T, N> v2(v1);
    of << v2 << endl;
    cout << "DemoArray finished!" << endl;

    using TraitStringString = ArrayTrait<string, string, std::less<KeyNode<string, string>>>;
    CArray<TraitStringString> vx("John Doe");
    vx.insert("John", "Doe");
    vx.insert("Jane", "Smith");
    vx.insert("Open", "AI");
    vx.insert("Hello", "World");
    cout << "CArray: " << vx << endl;
}

void DemoHash() {
    cout << "Hello from DemoHash()" << endl;
}

template <typename Container>
void demoLinkedList(Container& mylist) {
    using TX = typename Container::KeyType;
    using INT = typename Container::ValueType;

    TX keys[10] = {5, 24, 1, 2, 5, 0, 14, 18, 25, 22};
    INT linkedValues[10] = {-5, -24, -1, -2, -5, -0, -14, -18, -25, -22};

    cout << "Inserting:       ";
    for (auto x = 0; x < 10; x++) {
        cout << "{" << keys[x] << ":" << linkedValues[x] << "}" << ", ";
        mylist.insert(keys[x], linkedValues[x]);
    }
    cout << endl;
    cout << "Lista en orden: ";
    foreach(mylist, printLine<typename Container::NodeType>);
    cout << endl;

    cout << "-------------Reading from test.txt-------------" << endl;
    ifstream test("test.txt");
    test >> mylist;
    cout << "Printing using << : " << mylist << endl;
}

void demoLinkedListSorted() {
    cout << "-----------------------------------demoLinkedListSorted------------------------------------" << endl;

    using TraitAsc = LLTrait<int, int, std::less<NodeLinkedList<int, int>>>;
    using TraitDesc = LLTrait<int, int, std::greater<NodeLinkedList<int, int>>>;

    cout << "---------------------Ascending list------------------" << endl;
    LinkedList<TraitAsc> myAscList;
    demoLinkedList(myAscList);

    cout << endl << endl;

    cout << "--------------------Descending list-------------------" << endl;
    LinkedList<TraitDesc> myDescList;
    demoLinkedList(myDescList);

    cout << endl << endl << endl;
}

template <typename Container>
void demoDoubleLinkedList(Container& mylist) {
    using TX = typename Container::KeyType;
    using INT = typename Container::ValueType;

    TX keys[10] = {5, 24, 1, 2, 5, 0, 14, 18, 25, 22};
    INT linkedValues[10] = {-5, -24, -1, -2, -5, -0, -14, -18, -25, -22};

    cout << "Inserting:       ";
    for (auto x = 0; x < 10; x++) {
        cout << "{" << keys[x] << ":" << linkedValues[x] << "}" << ", ";
        mylist.insert(keys[x], linkedValues[x]);
    }
    cout << endl;
    cout << "Lista en orden : ";
    foreach(mylist, printLine<typename Container::NodeType>);
    cout << endl;

    cout << "Lista invertida: ";
    foreach_reverse(mylist, printLine<typename Container::NodeType>);
    cout << endl;

    cout << "-------------Reading from test.txt-------------" << endl;
    ifstream test("test.txt");
    test >> mylist;
    cout << "Printing using << : " << mylist << endl;
}

void demoDoubleLinkedListSorted() {
    cout << "-----------------------------------demoDoubleLinkedListSorted------------------------------------" << endl;

    using TraitAsc = DLLTrait<int, int, std::less<NodeDLL<int, int>>>;
    using TraitDesc = DLLTrait<int, int, std::greater<NodeDLL<int, int>>>;

    cout << "---------------------Ascending double list------------------" << endl;
    DoubleLinkedList<TraitAsc> myAscList;
    demoDoubleLinkedList(myAscList);

    cout << endl << endl;
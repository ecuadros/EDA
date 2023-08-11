#include <iostream> // cout
#include "demo.h"
#include "array.h"
using namespace std;

void DemoArray() {
  cout << "Hello from DemoArray()" << endl;
  for (auto i = 0; i < 10; i++) {
    insert(i);
  }
  print();
}

void DemoBinaryTree() {
  cout << "Hello from DemoBinaryTree()" << endl;
}

void DemoHash() {
  cout << "Hello from DemoHash()" << endl;
}
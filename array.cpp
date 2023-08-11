#include <iostream>
#include "array.h"
using namespace std;

int v[100], vcount = 0;

void insert(int val){
    // Check if the array is full.
    if (vcount == 100) {
        cout << "Array is full. Unable to insert " << val << endl;
        return;
    }

    // Insert the value into the array.
    v[vcount] = val;
    vcount++;
    cout << "Val=" << val << "inserted" <<endl;
}

void print(){
    if (vcount == 0) {
        cout << "Array is empty." << endl;
        return;
    }

    for(auto i = 0; i < vcount ; ++i )
        cout << "v[" << i << "]=" << v[i] << endl;
}

int main() {
    // Insert some values into the array.
    insert(10);
    insert(20);
    insert(30);

    // Print the contents of the array.
    print();

    return 0;
}

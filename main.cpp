#include <map>
#include <vector>
#include <iostream>
#include <string>
#include "demo.h"

using namespace std;
using demoFn = void (*)();

// g++ -pthread -std=c++17   -o main main.cpp demo.cpp
// g++ -pthread -std=c++20   -o main main.cpp demo.cpp
// g++ -pthread -std=c++2b   -o main main.cpp demo.cpp
// g++ -pthread -std=gnu++2b -o main main.cpp demo.cpp
// g++ -pthread -std=c++17 main.cpp demo.cpp -o main.exe && .\main.exe DemoArray
int main(int argc, char **argv)
{
    map<string, demoFn> map;
    map["DemoArray"] = DemoArray;
    map["DemoIterators"] = DemoIterators;
    map["DemoReverseIterators"] = DemoReverseIterators;
    map["DemoDynamicMatrixes"] = DemoDynamicMatrixes;
    map["DemoHeap"] = DemoHeap;
    map["DemoBTree"] = DemoBTree;
    map["DemoBinaryTree"] = DemoBinaryTree;
    map["DemoAVL"] = DemoAVL;
    map["DemoHash"] = DemoHash;
    map["DemoPreandPostIncrement"] = DemoPreandPostIncrement;
    map["DemoSmartPointers"] = DemoSmartPointers;
    map["DemoLinkedList"] = DemoLinkedList;
    map["DemoDoublyLinkedList"] = DemoDoublyLinkedList;

    if(argc > 1) {
        string demo = argv[1];
        if(map.find(demo) != map.end()) {
            try{
                map[demo]();
            } catch(exception &e) {
                cout << "Error: " << e.what() << endl;
            }
            return 0;
        }
    }

    vector<string> keys;
    for (const auto entry : map)
    {
        keys.push_back(entry.first);
    }

    for (int i = 0; i < keys.size(); i++)
    {
        cout << i << ". " << keys[i] << endl;
    }

    cout << "Select demo: " << endl;
    int option = 0;
    cin >> option;
    if (option < 0 || option >= keys.size())
    {
        cout << "Invalid option" << endl;
        return 0;
    }

    map[keys[option]]();

    return 0;
}
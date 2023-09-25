#include <iostream>

#include "demo.h"
using namespace std;

// g++ -pthread -std=c++17   -o main main.cpp demo.cpp
// g++ -pthread -std=c++20   -o main main.cpp demo.cpp
// g++ -pthread -std=c++2b   -o main main.cpp demo.cpp
// g++ -pthread -std=gnu++2b -o main main.cpp demo.cpp
// cmake -DCMAKE_BUILD_TYPE=Debug ../..
int main()
{
    std::cout << "STARTING..." << std::endl;
    DemoBinaryTree();
    std::cout << std::endl << "END" << std::endl;
    return 0;
}
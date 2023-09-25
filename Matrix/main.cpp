#include <iostream>

#include "demo.h"
using namespace std;

// g++ -pthread -std=c++17   -o main main.cpp demo.cpp
// cmake -DCMAKE_BUILD_TYPE=Debug ../..
int main()
{
    std::cout << "STARTING..." << std::endl;
    DemoMatrix();
    std::cout << std::endl << "END" << std::endl;
    return 0;
}
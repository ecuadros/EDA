#include <iostream> // cout
#include <fstream>  // ofstream, ifstream
#include <cmath>
#include <memory>
#include <functional>

#include "Matrix.h"
#include "demo.h"
using namespace std;

void DemoMatrix()
{
    Matrix<int> mat(2, 3);
    mat(0, 0) = 1;
    mat(0, 1) = 2;
    mat(0, 2) = 3;
    mat(1, 0) = 4;
    mat(1, 1) = 5;
    mat(1, 2) = 6;

    std::cout << "Matrix:" << std::endl;
    mat.print();
    std::cout << std::endl;

    // Iterate through the matrix using the forward iterator
    std::cout << "Iterator:" << std::endl;
    for (auto it = mat.begin(); it != mat.end(); ++it) {
        std::cout << *it << " ";
    }

    std::cout << std::endl;
}
#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <iostream>
#include <cassert>

template <typename T>
class Matrix
{
private:
    T **data;
    size_t rows;
    size_t cols;

public:
    Matrix(size_t getRows, size_t getCols) : rows(getRows), cols(getCols)
    {
        // Allocate memory for the matrix
        data = new T *[rows];
        for (size_t i = 0; i < rows; ++i)
        {
            data[i] = new T[cols];
        }
    }

    // Destructor to free memory
    ~Matrix()
    {
        for (size_t i = 0; i < rows; ++i)
        {
            delete[] data[i];
        }
        delete[] data;
    }

    size_t getRows() const
    {
        return rows;
    }

    size_t getCols() const
    {
        return cols;
    }

    // Access individual elements in the matrix
    T &operator()(size_t row, size_t col)
    {
        if (row >= rows || col >= cols)
        {
            throw std::out_of_range("Matrix indices out of range");
        }
        return data[row][col];
    }

    // Addition of two matrices
    Matrix operator+(const Matrix &other) const
    {
        if (rows != other.rows || cols != other.cols)
        {
            throw std::invalid_argument("Matrix dimensions must match for addition");
        }

        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                result(i, j) = data[i][j] + other(i, j);
            }
        }
        return result;
    }

    // Subtraction of two matrices
    Matrix operator-(const Matrix &other) const
    {
        if (rows != other.rows || cols != other.cols)
        {
            throw std::invalid_argument("Matrix dimensions must match for subtraction");
        }

        Matrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                result(i, j) = data[i][j] - other(i, j);
            }
        }
        return result;
    }

    // Multiplication of two matrices
    Matrix operator*(const Matrix &other) const
    {
        if (cols != other.rows)
        {
            throw std::invalid_argument("Number of columns in the first matrix must match the number of rows in the second matrix for multiplication");
        }

        Matrix result(rows, other.cols);
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < other.cols; ++j)
            {
                for (size_t k = 0; k < cols; ++k)
                {
                    result(i, j) += data[i][k] * other(k, j);
                }
            }
        }
        return result;
    }

    // Print the matrix
    void print() const
    {
        for (size_t i = 0; i < rows; ++i)
        {
            for (size_t j = 0; j < cols; ++j)
            {
                std::cout << data[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    class IteratorForward
    {
    private:
        Matrix<T> &matrix;
        size_t currentRow;
        size_t currentCol;

    public:
        IteratorForward(Matrix<T> &mat, size_t row = 0, size_t col = 0)
            : matrix(mat), currentRow(row), currentCol(col) {}

        T &operator*()
        {
            return matrix(currentRow, currentCol);
        }

        IteratorForward &operator++()
        {
            if (currentCol < matrix.getCols() - 1)
            {
                currentCol++;
            }
            else
            {
                currentCol = 0;
                currentRow++;
            }
            return *this;
        }

        bool operator==(const IteratorForward &other) const
        {
            return &matrix == &other.matrix && currentRow == other.currentRow && currentCol == other.currentCol;
        }

        bool operator!=(const IteratorForward &other) const
        {
            return !(*this == other);
        }
    };

    // Function to create and return an IteratorForward object
    IteratorForward begin()
    {
        return IteratorForward(*this, 0, 0);
    }

    // Function to create and return an IteratorForward object representing the end
    IteratorForward end()
    {
        return IteratorForward(*this, rows, 0);
    }
};

#endif
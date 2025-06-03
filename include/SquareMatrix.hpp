#pragma once

#include <cmath>
#include "Complex.hpp"
#include "DynamicArray.hpp"
#include "Exceptions.hpp"
#include "Vector.hpp"

template<typename T>
class SquareMatrix {
private:
    int size;
    DynamicArray<DynamicArray<T>> elements;

public:
    explicit SquareMatrix(int n) : size(n), elements(n) {
        for (int i = 0; i < n; ++i) {
            DynamicArray<T> row(n);
            for (int j = 0; j < n; ++j) {
                row.Set(j, T());
            }
            elements.Set(i, row);
        }
    }
    SquareMatrix(const SquareMatrix<T>& other) : size(other.size), elements(other.size) {
        for (int i = 0; i < size; ++i) {
            DynamicArray<T> row(other.size);
            for (int j = 0; j < size; ++j) {
                row.Set(j, other.Get(i, j));
            }
            elements.Set(i, row);
        }
    }

    SquareMatrix<T>& operator=(const SquareMatrix<T>& other) {
        if (this != &other) {
            size = other.size;
            elements = DynamicArray<DynamicArray<T>>(size);
            for (int i = 0; i < size; ++i) {
                DynamicArray<T> row(other.elements.Get(i));
                elements.Set(i, row);
            }
        }
        return *this;
    }

    int GetSize() const { return size; }

    T Get(int i, int j) const {
        if (i < 0 || i >= size || j < 0 || j >= size) {
            throw IndexOutOfRangeException("Index out of range");
        }
        return elements.Get(i).Get(j);
    }

    void Set(int i, int j, const T& value) {
        if (i < 0 || i >= size || j < 0 || j >= size) {
            throw IndexOutOfRangeException("Index out of range");
        }
        DynamicArray<T> row = elements.Get(i);
        row.Set(j, value);
        elements.Set(i, row);
    }

    SquareMatrix<T> operator+(const SquareMatrix<T>& other) const {
        if (size != other.size) {
            throw InvalidArgumentException("Matrices must have the same size for addition");
        }

        SquareMatrix<T> result(size);
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                result.Set(i, j, Get(i, j) + other.Get(i, j));
            }
        }
        return result;
    }

    SquareMatrix<T> operator*(const T& scalar) const {
        SquareMatrix<T> result(size);
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                result.Set(i, j, Get(i, j) * scalar);
            }
        }
        return result;
    }

    double Norm() const {
        T sum = T();
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                sum = sum + Get(i, j) * Get(i, j);
            }
        }
        return std::sqrt(std::abs(sum));
    }

    void MultiplyRow(int row, const T& scalar) {
        if (row < 0 || row >= size) {
            throw IndexOutOfRangeException("Invalid row index");
        }
        for (int j = 0; j < size; ++j) {
            Set(row, j, Get(row, j) * scalar);
        }
    }

    void MultiplyColumn(int col, const T& scalar) {
        if (col < 0 || col >= size) {
            throw IndexOutOfRangeException("Invalid column index");
        }
        for (int i = 0; i < size; ++i) {
            Set(i, col, Get(i, col) * scalar);
        }
    }

    void AddRowMultiple(int destRow, int srcRow, const T& scalar) {
        if (destRow < 0 || destRow >= size || srcRow < 0 || srcRow >= size) {
            throw IndexOutOfRangeException("Invalid row index");
        }
        for (int j = 0; j < size; ++j) {
            Set(destRow, j, Get(destRow, j) + Get(srcRow, j) * scalar);
        }
    }

    void AddColumnMultiple(int destCol, int srcCol, const T& scalar) {
        if (destCol < 0 || destCol >= size || srcCol < 0 || srcCol >= size) {
            throw IndexOutOfRangeException("Invalid column index");
        }
        for (int i = 0; i < size; ++i) {
            Set(i, destCol, Get(i, destCol) + Get(i, srcCol) * scalar);
        }
    }

    void SwapRows(int row1, int row2) {
        if (row1 < 0 || row1 >= size || row2 < 0 || row2 >= size) {
            throw IndexOutOfRangeException("Invalid row index");
        }
        for (int j = 0; j < size; ++j) {
            T temp = Get(row1, j);
            Set(row1, j, Get(row2, j));
            Set(row2, j, temp);
        }
    }

    void SwapColumns(int col1, int col2) {
        if (col1 < 0 || col1 >= size || col2 < 0 || col2 >= size) {
            throw IndexOutOfRangeException("Invalid column index");
        }
        for (int i = 0; i < size; ++i) {
            T temp = Get(i, col1);
            Set(i, col1, Get(i, col2));
            Set(i, col2, temp);
        }
    }
};
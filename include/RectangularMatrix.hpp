#pragma once

#include <cmath>
#include "Complex.hpp"
#include "DynamicArray.hpp"
#include "Exceptions.hpp"
#include "Vector.hpp"

template<typename T>
class RectangularMatrix {
private:
    int rows;
    int cols;
    DynamicArray<DynamicArray<T>> elements;

public:
    RectangularMatrix(int rows, int cols) : rows(rows), cols(cols), elements(rows) {
        for (int i = 0; i < rows; ++i) {
            DynamicArray<T> row(cols);
            for (int j = 0; j < cols; ++j) {
                row.Set(j, T());
            }
            elements.Set(i, row);
        }
    }

    RectangularMatrix(const RectangularMatrix<T>& other) 
        : rows(other.rows), cols(other.cols), elements(other.rows) {
        for (int i = 0; i < rows; ++i) {
            DynamicArray<T> row(other.elements.Get(i));
            elements.Set(i, row);
        }
    }

    RectangularMatrix<T>& operator=(const RectangularMatrix<T>& other) {
        if (this != &other) {
            rows = other.rows;
            cols = other.cols;
            elements = DynamicArray<DynamicArray<T>>(rows);
            for (int i = 0; i < rows; ++i) {
                DynamicArray<T> row(other.elements.Get(i));
                elements.Set(i, row);
            }
        }
        return *this;
    }

    int GetRows() const { return rows; }
    int GetCols() const { return cols; }

    T Get(int i, int j) const {
        if (i < 0 || i >= rows || j < 0 || j >= cols) {
            throw IndexOutOfRangeException("Index out of range");
        }
        return elements.Get(i).Get(j);
    }

    void Set(int i, int j, const T& value) {
        if (i < 0 || i >= rows || j < 0 || j >= cols) {
            throw IndexOutOfRangeException("Index out of range");
        }
        DynamicArray<T> row = elements.Get(i);
        row.Set(j, value);
        elements.Set(i, row);
    }

    RectangularMatrix<T> operator+(const RectangularMatrix<T>& other) const {
        if (rows != other.rows || cols != other.cols) {
            throw InvalidArgumentException("Matrices must have the same dimensions for addition");
        }

        RectangularMatrix<T> result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.Set(i, j, Get(i, j) + other.Get(i, j));
            }
        }
        return result;
    }


    RectangularMatrix<T> operator*(const T& scalar) const {
        RectangularMatrix<T> result(rows, cols);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                result.Set(i, j, Get(i, j) * scalar);
            }
        }
        return result;
    }

    double Norm() const {
        T sum = T();
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                sum = sum + Get(i, j) * Get(i, j);
            }
        }
        return std::sqrt(std::abs(sum));
    }

    void MultiplyRow(int row, const T& scalar) {
        if (row < 0 || row >= rows) {
            throw IndexOutOfRangeException("Invalid row index");
        }
        for (int j = 0; j < cols; ++j) {
            Set(row, j, Get(row, j) * scalar);
        }
    }

    void MultiplyColumn(int col, const T& scalar) {
        if (col < 0 || col >= cols) {
            throw IndexOutOfRangeException("Invalid column index");
        }
        for (int i = 0; i < rows; ++i) {
            Set(i, col, Get(i, col) * scalar);
        }
    }

    void AddRowMultiple(int destRow, int srcRow, const T& scalar) {
        if (destRow < 0 || destRow >= rows || srcRow < 0 || srcRow >= rows) {
            throw IndexOutOfRangeException("Invalid row index");
        }
        for (int j = 0; j < cols; ++j) {
            Set(destRow, j, Get(destRow, j) + Get(srcRow, j) * scalar);
        }
    }

    void AddColumnMultiple(int destCol, int srcCol, const T& scalar) {
        if (destCol < 0 || destCol >= cols || srcCol < 0 || srcCol >= cols) {
            throw IndexOutOfRangeException("Invalid column index");
        }
        for (int i = 0; i < rows; ++i) {
            Set(i, destCol, Get(i, destCol) + Get(i, srcCol) * scalar);
        }
    }

    void SwapRows(int row1, int row2) {
        if (row1 < 0 || row1 >= rows || row2 < 0 || row2 >= rows) {
            throw IndexOutOfRangeException("Invalid row index");
        }
        for (int j = 0; j < cols; ++j) {
            T temp = Get(row1, j);
            Set(row1, j, Get(row2, j));
            Set(row2, j, temp);
        }
    }

    void SwapColumns(int col1, int col2) {
        if (col1 < 0 || col1 >= cols || col2 < 0 || col2 >= cols) {
            throw IndexOutOfRangeException("Invalid column index");
        }
        for (int i = 0; i < rows; ++i) {
            T temp = Get(i, col1);
            Set(i, col1, Get(i, col2));
            Set(i, col2, temp);
        }
    }

    Vector<T> GetRow(int row) const {
        if (row < 0 || row >= rows) {
            throw IndexOutOfRangeException("Invalid row index");
        }
        Vector<T> result(cols);
        for (int j = 0; j < cols; ++j) {
            result.Set(j, Get(row, j));
        }
        return result;
    }

    Vector<T> GetColumn(int col) const {
        if (col < 0 || col >= cols) {
            throw IndexOutOfRangeException("Invalid column index");
        }
        Vector<T> result(rows);
        for (int i = 0; i < rows; ++i) {
            result.Set(i, Get(i, col));
        }
        return result;
    }
};
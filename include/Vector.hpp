#pragma once

#include <cmath>
#include "Complex.hpp"
#include "DynamicArray.hpp"
#include "Exceptions.hpp"

template<typename T>
class Vector {
private:
    DynamicArray<T> elements;

public:
    Vector() = default;
    
    explicit Vector(int size) : elements(size) {
        for (int i = 0; i < size; ++i) {
            elements.Set(i, T());
        }
    }
    
    Vector(const T* items, int count) : elements(items, count) {}
    Vector(const Vector<T>& other) : elements(other.elements) {}

    int GetSize() const {
        return elements.GetSize();
    }

    T Get(int index) const {
        return elements.Get(index);
    }

    void Set(int index, const T& value) {
        elements.Set(index, value);
    }

    Vector<T> operator+(const Vector<T>& other) const {
        if (GetSize() != other.GetSize()) {
            throw InvalidArgumentException("Vectors must have the same size for addition");
        }

        Vector<T> result(GetSize());
        for (int i = 0; i < GetSize(); ++i) {
            result.Set(i, Get(i) + other.Get(i));
        }
        return result;
    }

    Vector<T> operator*(const T& scalar) const {
        Vector<T> result(GetSize());
        for (int i = 0; i < GetSize(); ++i) {
            result.Set(i, Get(i) * scalar);
        }
        return result;
    }

    double Norm() const {
        T sum = T();
        for (int i = 0; i < GetSize(); ++i) {
            sum = sum + Get(i) * Get(i);
        }
        return std::sqrt(std::abs(sum));
    }

    T DotProduct(const Vector<T>& other) const {
        if (GetSize() != other.GetSize()) {
            throw InvalidArgumentException("Vectors must have the same size for dot product");
        }

        T result = T();
        for (int i = 0; i < GetSize(); ++i) {
            result = result + Get(i) * other.Get(i);
        }
        return result;
    }
};
#pragma once
#include <cmath>
#include <iostream>

class Complex
{
private:
    double r;
    double im;

public:
    Complex(double r = 0.0, double i = 0.0)
        : r(r)
        , im(i)
    {
    }

    double GetReal() const
    {
        return r;
    }

    double GetImag() const
    {
        return im;
    }

    double Modul() const
    {
        return std::sqrt(r * r + im * im);
    }

    Complex operator+(const Complex& other) const
    {
        return Complex(r + other.r, im + other.im);
    }

    Complex operator-(const Complex& other) const
    {
        return Complex(r - other.r, im - other.im);
    }

    Complex operator*(const Complex& other) const
    {
        return Complex(
            r * other.r - im * other.im,
            r * other.im + im * other.r
        );
    }

    bool operator==(const Complex& other) const
    {
        return r == other.r && im == other.im;
    }   

    bool operator<(const Complex& other) const
    {
        return Modul() < other.Modul();
    }

    void Print(std::ostream& os) const
    {
        os << GetReal();
        if (GetImag() >= 0)
        {
            os << "+";
        }
        os << GetImag() << "i";
    }
};

std::ostream& operator<<(std::ostream& os, const Complex& c) // (friend)
{
    c.Print(os);
    return os;
}
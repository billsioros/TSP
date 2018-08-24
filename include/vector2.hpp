
#pragma once

#include <iosfwd>       // std::istream & std::ostream

class Vector2
{
    double _x, _y;

public:

    Vector2() : _x(0.0), _y(0.0) {}
    Vector2(double _x, double _y) : _x(_x), _y(_y) {}
    Vector2(const Vector2& other) : _x(other._x), _y(other._y) {}

    // Access:
    double x() const { return _x; }
    double y() const { return _y; }

    // IO:
    friend std::ostream& operator<<(std::ostream&, const Vector2&);
    friend std::istream& operator>>(std::istream&, Vector2&);

    // Operations:
    Vector2& operator=(const Vector2&);
    friend Vector2 operator+(const Vector2&, const Vector2&);
    friend Vector2 operator-(const Vector2&, const Vector2&);
    friend double  operator*(const Vector2&, const Vector2&);
    friend Vector2 operator*(const Vector2&, double);
    friend Vector2 operator/(const Vector2&, double);

    Vector2& operator+=(const Vector2&);
    Vector2& operator-=(const Vector2&);
    Vector2& operator*=(double);
    Vector2& operator/=(double);

    friend bool operator< (const Vector2&, const Vector2&);
    friend bool operator> (const Vector2&, const Vector2&);
    friend bool operator==(const Vector2&, const Vector2&);
    friend bool operator!=(const Vector2&, const Vector2&);
};
